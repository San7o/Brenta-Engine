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
 * The above copyright notice and this permission notice shall be included in
 all
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

#include "utils_types.hpp"

#include <fstream>
#include <iostream>
#include <string>

#define DEBUG(...)                                                             \
    Brenta::Utils::Logger::Log (Brenta::Types::LogLevel::DEBUG, __VA_ARGS__)
#define INFO(...)                                                              \
    Brenta::Utils::Logger::Log (Brenta::Types::LogLevel::INFO, __VA_ARGS__)
#define WARNING(...)                                                           \
    Brenta::Utils::Logger::Log (Brenta::Types::LogLevel::WARNING, __VA_ARGS__)
#define ERROR(...)                                                             \
    Brenta::Utils::Logger::Log (Brenta::Types::LogLevel::ERROR, __VA_ARGS__)

namespace Brenta
{

namespace Utils
{

/**
 * @brief Logger class
 *
 * This class is used to log messages to the console and to a log file. The log
 * messages can be filtered by log level.
 */
class Logger
{
  public:
    /**
     * @brief Log level
     *
     * The log level is used to filter the messages that are printed to the
     * console and to the log file. The log level can be set using the
     * SetLogLevel method. Onlt the messages with a Loglevel greater or equal to
     * the set log level are printed.
     */
    static Brenta::Types::LogLevel level;
    /**
     * @brief Log file
     *
     * The log file is used to store the messages that are printed to the
     * console. The log file can be set using the SetLogFile method.
     */
    static std::ofstream log_file;

    /**
     * @brief Initialize the logger
     *
     * This method initializes the logger. It should be called before any other
     * method of the logger is called.
     */
    static void Init ();
    /**
     * @brief Set the log level
     *
     * This method sets the log level. The log level is used to filter the
     * messages that are printed to the console and to the log file. The log
     * level can be set to DEBUG, INFO, WARNING, ERROR, or DISABLED.
     *
     * @param level The log level
     */
    static void SetLogLevel (Brenta::Types::LogLevel level);
    /**
     * @brief Set the log file
     *
     * This method sets the log file. The log file is used to store the messages
     * that are printed to the console. The log file can be set to a file name.
     *
     * @param file The log file
     */
    static void SetLogFile (const std::string &file);
    /**
     * @brief Close the logger
     *
     * This method closes the logger. It should be called before the program
     * exits.
     */
    static void Close ();

    /**
     * @brief Log a message to stdout
     *
     * This method logs a message on stdout with the given log level. The
     * message can be any number of arguments of any type.
     *
     * @param level The log level
     * @param args The message
     */
    template <typename T, typename... Args>
    static void LogToStdout (T message, Args... args)
    {
        std::cout << message;
        LogToStdout (args...);
    }

    /**
     * @brief Log a message to the log file
     *
     * This method logs a message with the given log level. The message can be
     * any number of arguments of any type. The message is printed to the log
     * file if the log file is open.
     *
     * @param level The log level
     * @param args The message
     */
    template <typename T, typename... Args>
    static void LogToFile (T message, Args... args)
    {
        log_file << message;
        LogToFile (args...);
    }
    /**
     * @brief Log a message
     *
     * This method logs a message with the given log level. The message can be
     * any number of arguments of any type. The message is printed to the
     * console and to the log file if the log file is open.
     *
     * @param level The log level
     * @param args The message
     */
    template <typename... Args>
    static void Log (Brenta::Types::LogLevel level, Args... args)
    {
        if (Logger::level > level)
            return;
        LogToStdout (level, ": ", args...);
        if (Logger::log_file.is_open ())
            LogToFile (level, ": ", args...);
    }

  private:
    /* Base cases */
    template <typename T> static void LogToStdout (T message)
    {
        std::cout << message << std::endl;
    }
    template <typename T> static void LogToFile (T message)
    {
        log_file << message << std::endl;
    }
};

} // namespace Utils

} // namespace Brenta
