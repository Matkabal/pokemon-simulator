#include "logger/Logger.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <filesystem>

void Logger::init() {
    // Só cria se ainda não existir
    if (!spdlog::get("file_logger")) {
        auto logger = spdlog::rotating_logger_mt("file_logger", "../log/log.txt", 1024 * 1024 * 5, 3);
        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::debug);
        spdlog::flush_on(spdlog::level::info);
    }
}
