#include "Log.h"

#include "spdlog/sinks/basic_file_sink.h"

namespace Log {

    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    void Log::Init() {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_CoreLogger = spdlog::basic_logger_mt("CORE", "log.txt");
        s_CoreLogger->set_level(spdlog::level::trace);

        s_ClientLogger = spdlog::basic_logger_mt("APP", "log.txt");
        s_ClientLogger->set_level(spdlog::level::trace);
    }
}