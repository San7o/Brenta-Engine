#pragma once

#ifndef ENGINE_LOGGER_H
#define ENGINE_LOGGER_H

#include <iostream>
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
    static void SetLogLevel(Types::LogLevel level);
    static void Log(Types::LogLevel level, const std::string& message);
};

} // namespace ECS

#endif
