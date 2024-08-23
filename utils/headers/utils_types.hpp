#pragma once

#include <ostream>

namespace Brenta
{

namespace Types
{

/**
 * @brief Log level
 *
 * The log level is used to filter the messages that are printed to the console
 * and to the log file. The log level can be set using the SetLogLevel method.
 * Onlt the messages with a Loglevel greater or equal to the set log level are
 * printed.
 */
enum LogLevel
{
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR,
    DISABLED
};

std::ostream &operator<< (std::ostream &os, const LogLevel level);

} // namespace Types

} // namespace Brenta
