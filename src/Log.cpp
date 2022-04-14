#include "Log.h"

#include "spdlog/sinks/basic_file_sink.h"

namespace Log {

    std::shared_ptr<spdlog::logger> Log::logger;

    void Log::Init() {
        spdlog::set_pattern("%^[%T] %v%$");
        logger = spdlog::basic_logger_mt("APP", "log.txt");
        logger->set_level(spdlog::level::trace);
    }
}