#pragma once

#include <memory>
#include <spdlog/spdlog.h>

namespace Log {
    class Log {
    public:
        static void Init();
        inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

// Core log macros
#define MY_CORE_TRACE(...)      ::Log::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MY_CORE_INFO(...)       ::Log::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MY_CORE_WARN(...)       ::Log::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MY_CORE_ERROR(...)      ::Log::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MY_CORE_FATAL(...)      ::Log::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define MY_TRACE(...)           ::Log::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MY_INFO(...)            ::Log::Log::GetClientLogger()->info(__VA_ARGS__)
#define MY_WARN(...)            ::Log::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MY_ERROR(...)           ::Log::Log::GetClientLogger()->error(__VA_ARGS__)
#define MY_FATAL(...)           ::Log::Log::GetClientLogger()->fatal(__VA_ARGS__)