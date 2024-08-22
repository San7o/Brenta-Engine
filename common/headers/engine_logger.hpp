/*
 * MIT License
 * 
 * Copyright (c) 2024 Giovanni Santini

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */ 

#pragma once

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

std::ostream& operator<<(std::ostream& os, const LogLevel level);

} // namespace Types

#define DEBUG(...) Logger::Log(Types::LogLevel::DEBUG, __VA_ARGS__)
#define INFO(...) Logger::Log(Types::LogLevel::INFO, __VA_ARGS__)
#define WARNING(...) Logger::Log(Types::LogLevel::WARNING, __VA_ARGS__)
#define ERROR(...) Logger::Log(Types::LogLevel::ERROR, __VA_ARGS__)

class Logger {
public:

static Types::LogLevel level;
static std::ofstream log_file;

static void Init();
static void SetLogLevel(Types::LogLevel level);
static void SetLogFile(const std::string& file);
static void Close();

/* Base cases */
template <typename T>
static void LogToStdout(T message) {
    std::cout << message << std::endl;
}
template <typename T>
static void LogToFile(T message) {
    log_file << message << std::endl;
}

template <typename T, typename ... Args>
static void LogToStdout(T message, Args... args) {
    std::cout << message;
    LogToStdout(args...);
}

template <typename T, typename ... Args>
static void LogToFile(T message, Args... args) {
    log_file << message;
    LogToFile(args...);
}

template <typename ... Args>
static void Log(Types::LogLevel level, Args... args) {
    LogToStdout(level, ": ", args...);
    if (Logger::log_file.is_open())
        LogToFile(level, ": ", args...);
}

};

} // namespace ECS
