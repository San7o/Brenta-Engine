#include "utils_types.hpp"

#include <ostream>

namespace brenta
{

namespace types
{

std::ostream &operator<<(std::ostream &os, const log_level level)
{
    switch (level)
    {
    case log_level::DEBUG:
        os << "DEBUG";
        break;
    case log_level::INFO:
        os << "INFO";
        break;
    case log_level::WARNING:
        os << "WARNING";
        break;
    case log_level::ERROR:
        os << "ERROR";
        break;
    case log_level::OUTPUT:
        os << "OUTPUT";
        break;
    case log_level::DISABLED:
        os << "DISABLED";
        break;
    default:
        break;
    }

    return os;
}

} // namespace types

} // namespace brenta
