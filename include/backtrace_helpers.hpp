#ifndef GLUON_BACKTRACE_HELPERS_HPP_
#define GLUON_BACKTRACE_HELPERS_HPP_

#include <cstdint>
#include <unwind.h>

#include "gluon_config.hpp"

namespace Gluon::BacktraceHelpers {
    struct BacktraceState {
        void **current;
        void **end;
        uint16_t skip;
    };

    GLUON_API _Unwind_Reason_Code unwindCallback(struct _Unwind_Context *context, void *arg);
    GLUON_API size_t captureBacktrace(void **buffer, uint16_t max, uint16_t skip = 0);
}

#endif