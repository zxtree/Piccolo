#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/async.h>

#ifdef SPDLOG_ACTIVE_LEVEL
    #undef SPDLOG_ACTIVE_LEVEL
    #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif
#define SPDLOG_DEBUG_ON
#define SPDLOG_TRACE_ON

namespace Piccolo {

extern std::shared_ptr<spdlog::async_logger> _g_logger_;

extern void init_log();

// copy from spdlog/pattern_formatter-inl.h
static const char *basename(const char *filename)
{
    // if the size is 2 (1 character + null terminator) we can use the more efficient strrchr
    // the branch will be elided by optimizations
    if (sizeof(spdlog::details::os::folder_seps) == 2)
    {
        const char *rv = std::strrchr(filename, spdlog::details::os::folder_seps[0]);
        return rv != nullptr ? rv + 1 : filename;
    }
    else
    {
        const std::reverse_iterator<const char *> begin(filename + std::strlen(filename));
        const std::reverse_iterator<const char *> end(filename);

        const auto it = std::find_first_of(begin, end, std::begin(spdlog::details::os::folder_seps), std::end(spdlog::details::os::folder_seps) - 1);
        return it != end ? it.base() : filename;
    }
}


#define LOGGER() \
    if(Piccolo::_g_logger_ == nullptr) Piccolo::init_log(); \
    Piccolo::_g_logger_

#define LOG_FUNC_TAG "[" + std::string(__FUNCTION__) + "@" + std::string(Piccolo::basename(__FILE__)) + ":" + std::to_string(__LINE__) + "] "

#define LOG_DEBUG(...) \
    LOGGER()->debug(LOG_FUNC_TAG + __VA_ARGS__);

#define LOG_INFO(...) \
    LOGGER()->info(LOG_FUNC_TAG + __VA_ARGS__);

#define LOG_WARN(...) \
    LOGGER()->warn(LOG_FUNC_TAG + __VA_ARGS__);

#define LOG_ERROR(...) \
    LOGGER()->error(LOG_FUNC_TAG + __VA_ARGS__);

#define LOG_SYS_ERR(STR, CODE) \
    LOGGER()->error(LOG_FUNC_TAG + STR + std::system_category().message(CODE));

#define LOG_FATAL(...) \
    LOGGER()->critical(LOG_FUNC_TAG + __VA_ARGS__);
}