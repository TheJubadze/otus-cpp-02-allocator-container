#pragma once

#include <memory>
#include <spdlog/spdlog.h>

namespace Log {
    class Log {
    public:
        static void Init();
        inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return logger; }

    private:
        static std::shared_ptr<spdlog::logger> logger;
    };
}

// Core log macros
#define MY_TRACE(...)      ::Log::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MY_INFO(...)       ::Log::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MY_WARN(...)       ::Log::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MY_ERROR(...)      ::Log::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MY_FATAL(...)      ::Log::Log::GetCoreLogger()->fatal(__VA_ARGS__)
