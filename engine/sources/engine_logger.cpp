#include "engine_logger.h"
#include <chrono>
#include <iomanip>
#include <ctime>

using namespace ECS;

Types::LogLevel Logger::level = Types::LogLevel::WARNING;
std::ofstream Logger::log_file;

void Logger::SetLogLevel(Types::LogLevel level) {
    Logger::level = level;
}

void Logger::Init() {
    SetLogFile("logs/log.txt");
}

void Logger::Close() {
    Logger::log_file.close();
}

void Logger::SetLogFile(const std::string& file) {
    Logger::log_file.open(file, std::ios::app);
    if (!Logger::log_file.is_open()) {
        std::cout << "Error: Could not open log file" << std::endl;
    }

    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_time_t);
    log_file << "----------" <<
             std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S")
             << "----------" << std::endl;
    
}

namespace ECS::Types {
std::ostream& operator<<(std::ostream& os, const LogLevel level) {
    switch (level) {
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
        case LogLevel::DISABLED:
            os << "DISABLED";
            break;
        default:
            break;
    }

    return os;
}

} // namespace Types
