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
