// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef _WIN32

#include <brenta/fswatcher.hpp>
#include <sys/inotify.h>

using namespace brenta;

#define _FSWATCHER_BUFFER_LEN  (1024 * (sizeof(struct inotify_event) + 16))

struct FsWatcherUnixWdList
{
  int wd;
  uint32_t mask;
  std::filesystem::path pathname;
  FsWatcherUnixWdList *next;
};
  
struct FsWatcherUnix
{
  int fd;
  FsWatcherUnixWdList *wd_list;  
};

FilesystemWatcher::~FilesystemWatcher()
{
  this->destroy();
}

bool FilesystemWatcher::init()
{
  FsWatcherUnix* fw = new FsWatcherUnix();
  fw->fd = inotify_init();
  if (fw->fd == -1)
    return false;

  this->internal = static_cast<void*>(fw);
  return true;
}

void FilesystemWatcher::destroy()
{
  if (!this->internal) return;
  
  FsWatcherUnix *fw_unix = static_cast<FsWatcherUnix*>(this->internal);
  
  close(fw_unix->fd);

  FsWatcherUnixWdList *it = fw_unix->wd_list;
  FsWatcherUnixWdList *next = NULL;
  while (it)
  {
    next = it->next;
    delete it;
    it = next;
  }
  
  delete static_cast<FsWatcherUnix*>(this->internal);
  this->internal = nullptr;
}
  
bool FilesystemWatcher::add(const std::filesystem::path &path,
                            tenno::vector<Event> events)
{
  if (!this->internal) return false;
  
  FsWatcherUnix *fw_unix = static_cast<FsWatcherUnix*>(this->internal);
  uint32_t mask = 0;

  for (auto event : events)
  {
    switch(event)
    {
    case Event::Modify: mask |= IN_MODIFY; break;
    default:
      break;
    }
  }
  int wd = inotify_add_watch(fw_unix->fd, path.c_str(), mask);
  if (wd == -1)
    return false;

  FsWatcherUnixWdList* wd_item = new FsWatcherUnixWdList();
  wd_item->wd       = wd;
  wd_item->pathname = path;
  wd_item->mask     = mask;
  wd_item->next     = NULL;

  if (!fw_unix->wd_list)
  {
    fw_unix->wd_list = wd_item;
    return true;
  }
  
  FsWatcherUnixWdList *it = fw_unix->wd_list;
  while(it->next) { it = it->next; }
  it->next = wd_item;
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
  FsWatcherUnix *fw_unix = static_cast<FsWatcherUnix*>(this->internal);

  FsWatcherUnixWdList *prev = NULL;
  FsWatcherUnixWdList *it = fw_unix->wd_list;
  while(it && it->pathname != path)
  { prev = it; it = it->next; }

  if (!it)
    return true;
  
  int ret = inotify_rm_watch(fw_unix->fd, it->wd);
  if (ret == -1)
    return -1;

  if (!prev)
    fw_unix->wd_list = it->next;
  else
    prev->next = it->next;
  
  delete it;
  return true;
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
  FsWatcherUnix *fw_unix = static_cast<FsWatcherUnix*>(this->internal);

  char buff[_FSWATCHER_BUFFER_LEN] = {0};
  ssize_t bytes = read(fw_unix->fd, &buff, sizeof(buff));
  if (bytes == -1 || bytes == 0)
    return {};

  for (char *ptr = buff; ptr < buff + bytes; )
  {
    struct inotify_event *ev = (struct inotify_event *) ptr;

    FsWatcherUnixWdList *it = fw_unix->wd_list;
    while (it && ev->wd != it->wd) { it = it->next; }
    if (it)
      return it->pathname;
    
    ptr += sizeof(struct inotify_event) + ev->len;
  }

  return {};
}

#endif // _WIN32
