#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

extern std::shared_ptr<spdlog::async_logger> _g_logger_;

extern void init_log();

#define LOGGER() \
    if(_g_logger_ == nullptr) init_log(); \
    _g_logger_

#define LOG_DEBUG(...) \
    LOGGER()->debug("[" + std::string(__FUNCTION__) + "]" + __VA_ARGS__);

#define LOG_INFO(...) \
    LOGGER()->info("[" + std::string(__FUNCTION__) + "]" + __VA_ARGS__);

#define LOG_WARN(...) \
    LOGGER()->warn("[" + std::string(__FUNCTION__) + "]" + __VA_ARGS__);

#define LOG_ERROR(...) \
    LOGGER()->error("[" + std::string(__FUNCTION__) + "]" + __VA_ARGS__);

#define LOG_FATAL(...) \
    LOGGER()->critical("[" + std::string(__FUNCTION__) + "]" + __VA_ARGS__);
