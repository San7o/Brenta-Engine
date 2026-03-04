// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <tenno/vector.hpp>

#include <filesystem>
#include <optional>

namespace brenta
{

//
// Filesystem Watcher
// ------------------
//
// Watch for events on files in the filesystem. Make sure you
// initialize and destroy this object before using it.
//
// The API is really simple, you `add(..)` and `rm(..)` the paths
// you want to monitor, then call `watch()` and wait for events.
// Watch will return a path where an event occurred. If it does
// not return anything, then this object was destroyed or an error
// was encountered while waiting.
//
class FilesystemWatcher
{
public:

  enum Event
  {
    Modify,
  };

  FilesystemWatcher() = default;
  ~FilesystemWatcher();

  bool init();
  void destroy();
  
  bool add(const std::filesystem::path &path,
           tenno::vector<Event> events);
  bool add(const tenno::vector<std::filesystem::path> &paths,
           tenno::vector<Event> events);
  
  bool rm(const std::filesystem::path &path);
  bool rm(const tenno::vector<std::filesystem::path> &path);

  // Blocks and returns the path that received a notification
  std::optional<std::filesystem::path> watch();

private:

  void* internal = nullptr;  // used internally
  
};
  
} // namespace brenta
