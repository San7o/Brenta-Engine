// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifdef _WIN32

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <brenta/fswatcher.hpp>

namespace brenta
{

struct FsWatcherWindowsDirList
{
  HANDLE     hDir;
  WCHAR      pathW[MAX_PATH];
  char       path[MAX_PATH];
  BYTE       buffer[1024 * 64];
  DWORD      filter;
  OVERLAPPED overlapped;
  FsWatcherWindowsDirList *next;
};

struct FsWatcherWindows
{
  HANDLE iocp;
  FsWatcherWindowsDirList *dir_list;
};

} // namespace brenta

using namespace brenta;
  
FilesystemWatcher::~FilesystemWatcher()
{
  this->destroy();
}

bool FilesystemWatcher::init()
{
  FsWatcherWindows *fw_windows = new FsWatcherWindows();

  fw_windows->iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE,
                                            NULL,
                                            0,
                                            0);
  if (!fw_windows->iocp)
  {
    delete fw_windows;
    return false;
  }

  this->internal = static_cast<void*>(fw_windows);
  return true;
}

void FilesystemWatcher::destroy()
{
  if (!this->internal) return;
  
  FsWatcherWindows *fw_windows = static_cast<FsWatcherWindows*>(this->internal);
  
  FsWatcherWindowsDirList *it = fw_windows->dir_list;
  FsWatcherWindowsDirList *next = NULL;
  while (it)
  {
    next = it->next;
    CloseHandle(it->hDir);
    // Send exit signal
    PostQueuedCompletionStatus(fw_windows->iocp, 0, (ULONG_PTR)NULL, NULL);
    it = next;
  }

  CloseHandle(fw_windows->iocp);
  
  delete static_cast<FsWatcherWindows*>(this->internal);
  this->internal = nullptr;
}
  
bool FilesystemWatcher::add(const std::filesystem::path &path,
                            tenno::vector<Event> events)
{
  if (!this->internal) return false;
  
  FsWatcherWindows *fw_windows = static_cast<FsWatcherWindows*>(this->internal);

  DWORD filter = 0;
  for (auto& event : events)
  {
    switch(event)
    {
    case FilesystemWatcher::Event::Modify:
      filter |= FILE_NOTIFY_CHANGE_LAST_WRITE;
      break;
    default:
      break;
    }
  }

  // Convert to wide char string
  int len = MultiByteToWideChar(CP_UTF8, 0, path.string().c_str(), -1, NULL, 0);
  WCHAR pathW[MAX_PATH];
  MultiByteToWideChar(CP_UTF8, 0, path.string().c_str(), -1, pathW, len);
  
  wchar_t full_path[MAX_PATH];
  wchar_t *file_part = NULL;

  if (GetFullPathNameW(pathW,
                       MAX_PATH,
                       full_path,
                       &file_part) == 0)
    return false;

  FsWatcherWindowsDirList *fw_dir = new FsWatcherWindowsDirList();
  fw_dir->filter = filter;
  strcpy(fw_dir->path, path.string().c_str());
  
  if (file_part != NULL)
  {
    wcscpy(fw_dir->pathW, file_part);
    *file_part = L'\0';
  }
  
  HANDLE hDir = CreateFileW(full_path,
                            FILE_LIST_DIRECTORY,
                            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                            NULL,
                            OPEN_EXISTING,
                            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
                            NULL);
  if (hDir == INVALID_HANDLE_VALUE)
  {
    delete fw_dir;
    return false;
  }

  fw_dir->hDir = hDir;

  // Associate the handle with the port
  if (CreateIoCompletionPort(fw_dir->hDir,
                             fw_windows->iocp,
                             (ULONG_PTR) fw_dir,
                             0) == NULL)
  {
    delete fw_dir;
    return false;
  }

  // Tell windows to listen for changes on this directory
  if (ReadDirectoryChangesW(fw_dir->hDir,
                            fw_dir->buffer,
                            sizeof(fw_dir->buffer),
                            FALSE,
                            filter,
                            NULL,
                            &fw_dir->overlapped, NULL) == 0)
  {
    delete fw_dir;
    return false;
  }

  // Add dir to list of watched files
  FsWatcherWindowsDirList *it = fw_windows->dir_list;
  if (!it)
  {
    fw_windows->dir_list = fw_dir;
    return true;
  }
  while(it->next) { it = it->next; }
  it->next = fw_dir;
  return true;
}

bool FilesystemWatcher::add(const tenno::vector<std::filesystem::path> &paths,
                            tenno::vector<Event> events)
{
  bool out = true;
  for (auto& path : paths)
  {
    out |= this->add(path, events);
  }
  return out;
}

bool FilesystemWatcher::rm(const std::filesystem::path &path)
{
  if (!this->internal) return false;
  FsWatcherWindows *fw_windows = static_cast<FsWatcherWindows*>(this->internal);
  FsWatcherWindowsDirList **curr = &fw_windows->dir_list;

  while (*curr)
  {
    if ((*curr)->path == path)
    {
      FsWatcherWindowsDirList *to_remove = *curr;
      
      *curr = to_remove->next;

      CancelIoEx(to_remove->hDir, &to_remove->overlapped);
      CloseHandle(to_remove->hDir);
            
      return true;
    }
    curr = &((*curr)->next);
  }
  
  return false;
}

bool FilesystemWatcher::rm(const tenno::vector<std::filesystem::path> &paths)
{
  bool out = true;
  for (auto& path : paths)
  {
    out |= this->rm(path);
  }
  return out;
}

std::optional<std::filesystem::path> FilesystemWatcher::watch()
{
  if (!this->internal) return {};
  FsWatcherWindows *fw_windows = static_cast<FsWatcherWindows*>(this->internal);

  DWORD bytes;
  ULONG_PTR key;
  LPOVERLAPPED overlapped;

  while(GetQueuedCompletionStatus(fw_windows->iocp,
                                  &bytes,
                                  &key,
                                  &overlapped,
                                  INFINITE))
  {
    if (key == 0) return {}; // Global exit signal

    FsWatcherWindowsDirList* fw_dir = (FsWatcherWindowsDirList*)key;

    if (bytes == 0)
    {
      delete fw_dir;
      continue;
    }

    BYTE* pRaw = fw_dir->buffer;
    FILE_NOTIFY_INFORMATION* info = NULL;
    
    do
    {
      info = (FILE_NOTIFY_INFORMATION*)pRaw;
            
      if (wcsncmp(info->FileName, 
                  fw_dir->pathW, 
                  info->FileNameLength / sizeof(WCHAR)) == 0) 
        {
          ReadDirectoryChangesW(fw_dir->hDir,
                                fw_dir->buffer,
                                sizeof(fw_dir->buffer),
                                FALSE,
                                fw_dir->filter,
                                NULL, 
                                &fw_dir->overlapped,
                                NULL);
          
          return fw_dir->path;
        }

      pRaw += info->NextEntryOffset;

    } while (info->NextEntryOffset != 0);

    ReadDirectoryChangesW(fw_dir->hDir,
                          fw_dir->buffer,
                          sizeof(fw_dir->buffer),
                          FALSE,
                          fw_dir->filter, NULL, 
                          &fw_dir->overlapped,
                          NULL);
  }
  return {};
}


#endif // _WIN32
