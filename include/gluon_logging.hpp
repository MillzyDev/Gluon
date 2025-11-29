#ifndef GLUON_LOGGING_HPP_
#define GLUON_LOGGING_HPP_

#include <memory>

#include "gluon_config.hpp"

#include "spdlog/logger.h"

namespace Gluon {
    void logBacktraceFull(void *const *stackTraceBuffer, uint16_t stackTraceSize);
    std::shared_ptr<spdlog::logger> getLogger();
} // Gluon

#endif // GLUON_LOGGING_HPP_