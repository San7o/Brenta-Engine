#include "engine_logger.h"

using namespace ECS;

Types::LogLevel Logger::level = Types::LogLevel::WARNING;

void Logger::SetLogLevel(Types::LogLevel level) {
    Logger::level = level;
}

void Logger::Log(Types::LogLevel level, const std::string& message) {
    if (Logger::level <= level) {
        switch (level) {
            case Types::LogLevel::INFO:
                std::cout << "INFO: " << message << std::endl;
                break;
            case Types::LogLevel::WARNING:
                std::cout << "WARNING: " << message << std::endl;
                break;
            case Types::LogLevel::ERROR:
                std::cout << "ERROR: " << message << std::endl;
                break;
        }
    }
}
