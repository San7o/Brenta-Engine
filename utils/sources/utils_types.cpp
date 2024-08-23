#include "utils_types.hpp"

#include <ostream>

namespace Brenta
{

namespace Types
{

std::ostream &operator<< (std::ostream &os, const LogLevel level)
{
    switch (level)
    {
    case LogLevel::DEBUG:
        os << "DEBUG";
        break;
    case LogLevel::INFO:
        os << "INFO";
        break;
    case LogLevel::WARNING:
        os << "WARNING";
        break;
    case LogLevel::ERROR:
        os << "ERROR";
        break;
    case LogLevel::OUTPUT:
        os << "OUTPUT";
        break;
    case LogLevel::DISABLED:
        os << "DISABLED";
        break;
    default:
        break;
    }

    return os;
}

} // namespace Types

} // namespace Brenta
