#pragma once

#ifndef ENGINE_LOGGER_H
#define ENGINE_LOGGER_H

#include <iostream>
#include <fstream>
#include <string>

namespace ECS {

namespace Types {

enum LogLevel {
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR,
    DISABLED
};

} // namespace Types

class Logger {
public:
    static Types::LogLevel level;
    static std::ofstream log_file;

    static void Init();
    static void SetLogLevel(Types::LogLevel level);
    static void SetLogFile(const std::string& file);
    static void Log(Types::LogLevel level, const std::string& message);
    static void Close();
};

} // namespace ECS

#endif
