#include "log.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Piccolo {

std::shared_ptr<spdlog::async_logger> _g_logger_ = nullptr;

void init_log()
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);
    console_sink->set_pattern("[%H:%M:%S.%e][%^%l%$]%v");

    const spdlog::sinks_init_list sink_list = {console_sink};

    spdlog::init_thread_pool(8192, 1);

    _g_logger_ = std::make_shared<spdlog::async_logger>("muggle_logger1",
                                                        sink_list.begin(),
                                                        sink_list.end(),
                                                        spdlog::thread_pool(),
                                                        spdlog::async_overflow_policy::block);
    _g_logger_->set_level(spdlog::level::trace);

    spdlog::register_logger(_g_logger_);
}

}