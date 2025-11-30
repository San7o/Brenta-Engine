// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <fstream>
#include <iostream>
#include <oak/oak.hpp>
#include <string>

#define DEBUG(...) OAK_DEBUG(__VA_ARGS__);
#define INFO(...) OAK_INFO(__VA_ARGS__);
#define WARN(...) OAK_WARN(__VA_ARGS__);
#define ERROR(...) OAK_ERROR(__VA_ARGS__);
