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

void Logger::Log(Types::LogLevel level, const std::string& message) {
    if (!log_file.is_open()) {
        std::cout << "Error: Log file is not open" << std::endl;
        return;
    }
    if (Logger::level <= level) {
        switch (level) {
            case Types::LogLevel::DEBUG:
                std::cout << "DEBUG: " << message << std::endl;
                log_file << "DEBUG: " << message << std::endl;
                break;
            case Types::LogLevel::INFO:
                std::cout << "INFO: " << message << std::endl;
                log_file << "INFO: " << message << std::endl;
                break;
            case Types::LogLevel::WARNING:
                std::cout << "WARNING: " << message << std::endl;
                log_file << "WARNING: " << message << std::endl;
                break;
            case Types::LogLevel::ERROR:
                std::cout << "ERROR: " << message << std::endl;
                log_file << "ERROR: " << message << std::endl;
                break;
            default:
                break;
        }
    }
}
