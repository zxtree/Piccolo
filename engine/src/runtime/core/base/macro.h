#pragma once

#include "runtime/core/base/log.h"

#include "runtime/function/global/global_context.h"

#include <chrono>
#include <thread>

#define PolitSleep(_ms) std::this_thread::sleep_for(std::chrono::milliseconds(_ms));

#define PolitNameOf(name) #name

#ifdef NDEBUG
#define ASSERT(statement)
#else
#define ASSERT(statement) assert(statement)
#endif
