#pragma once

#include <spdlog/spdlog.h> 
#include <string>

class Logger {
public:
    static void init();

    template<typename... Args>
    static void info(const char* fmt, Args&&... args) {
        spdlog::info(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void debug(const char* fmt, Args&&... args) {
        spdlog::debug(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void warn(const char* fmt, Args&&... args) {
        spdlog::warn(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void error(const char* fmt, Args&&... args) {
        spdlog::error(fmt, std::forward<Args>(args)...);
    }
};
