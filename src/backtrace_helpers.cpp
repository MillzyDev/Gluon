#include <cstdint>
#include <unwind.h>

#include "backtrace_helpers.hpp"

namespace Gluon::BacktraceHelpers {
    _Unwind_Reason_Code unwindCallback(struct _Unwind_Context *context, void *arg) {
        BacktraceState *state = static_cast<BacktraceState *>(arg);
        uintptr_t pc = _Unwind_GetIP(context);

        if (pc) {
            if (state->current == state->end) {
                return _URC_END_OF_STACK;
            }
            if (state->skip == 0) {
                *state->current++ = reinterpret_cast<void *>(pc);
            }
            else {
                state->skip--;
            }
        }

        return _URC_NO_REASON;
    }

    size_t captureBacktrace(void **buffer, uint16_t max, uint16_t skip) {
        BacktraceState state{ buffer, buffer + max, skip };
        _Unwind_Backtrace(unwindCallback, &state);

        return state.current - buffer;
    }
}