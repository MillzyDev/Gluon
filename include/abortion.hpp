#ifndef GLUON_ABORTION_HPP_
#define GLUON_ABORTION_HPP_

#include <cstdint>
#include <type_traits>
#include <unistd.h>

#include "backtrace_helpers.hpp"
#include "gluon_logging.hpp"

#define CRASH_UNLESS(...) Gluon::crashUnless(__VA_ARGS__, __PRETTY_FUNCTION__, __FILE__, __LINE__)
#define SAFE_ABORT() Gluon::safeAbort(__PRETTY_FUNCTION__, __FILE__, __LINE__)
#define SAFE_ABORT_MSG(...) Gluon::safeAbortMsg(__PRETTY_FUNCTION__, __FILE__, __LINE__, __VA_ARGS__)
#define THROW_UNLESS(...) Gluon::throwUnless(__VA_ARGS__, __PRETTY_FUNCTION__, __FILE__, __LINE__)

#define RET_UNLESS(retval, ...) ({      \
    auto&& __temp__ = (__VA_ARGS__);    \
    if (!__temp__) {                    \
        return retval;                  \
    }                                   \
    Gluon::unwrapOptionals(__temp__); })

#define RET_V_UNLESS(...) RET_UNLESS(, __VA_ARGS__)
#define RET_DEFAULT_UNLESS(...) RET_UNLESS({ }, __VA_ARGS__)
#define RET_0_UNLESS(...) RET_DEFAULT_UNLESS(__VA_ARGS__)
#define RET_NULLOPT_UNLESS(...) RET_DEFAULT_UNLESS(__VA_ARGS__)

// For use in fire-if-compiled asserts e.g. static_assert(false_t<T>, "message")
template <class...> constexpr std::false_type false_t{};

namespace Gluon {
    template <class, template <class, class...> class>
    struct IsInstance : public std::false_type {};

    template <class...Ts, template <class, class...> class U>
    struct IsInstance<U<Ts...>, U> : public std::true_type {};

    inline void logBacktrace() {
        constexpr uint16_t kStackTraceMaxSize = 256;
        void *stackTraceBuffer[kStackTraceMaxSize];
        uint16_t stackTraceSize;
        stackTraceSize = Gluon::BacktraceHelpers::captureBacktrace(stackTraceBuffer, kStackTraceMaxSize);
        Gluon::Logger::logBacktraceFull(stackTraceBuffer, stackTraceSize);
    }

    [[noreturn]] inline void safeAbort(const char *func, const char *file, int line, uint16_t frameCount = 512) {
        for (int i = 0; i < 2; i++) {
            usleep(100000); // 0.1s
            Gluon::Logging::Logger::warn("Aborting in {} at {}:{}", func, file, line);
        }
        logBacktrace();
        Gluon::Logger::flush();
        usleep(100000L); // 0.1s
        std::terminate();
    }

    template<typename... TArgs>
    [[noreturn]] inline void safeAbortMsg(const char *func, const char *file, int line, std::format_string<TArgs...> fmt, TArgs &&...args) {
        for (int i = 0; i < 2; i++) {
            usleep(100000L); // 0.1s
            Gluon::Logger::warn("Aborting in {} at {}:{}", func, file, line);
            Gluon::Logger::warn(fmt, std::forward<TArgs>(args)...);
        }
        logBacktrace();
        Gluon::Logger::flush();
        usleep(100000L); // 0.1s
        std::terminate();
    }

    template<class TArg>
    auto &&unwrapOptionals(TArg &&arg) {
        if constexpr (IsInstance<std::decay_t<TArg>, std::optional>::value) {
            return *arg;
        }
        else {
            return arg;
        }
    }

    template<class TArg>
    auto crashUnless(TArg &&arg, const char *func, const char *file, int line) {
        if (!arg) {
            Gluon::safeAbort(func, file, line);
        }
        return unwrapOptionals(arg);
    }

    [[noreturn]] void throwException(const std::string &message);

    template <class TArg>
    auto throwUnless(TArg &&arg, const char *func, const char *file, int line) {
        if (!arg) {
            throwException(std::format("Throwing in {} at {}:{}", func, file, line));
        }

        return unwrapOptionals(arg);
    }
}

#endif // GLUON_ABORTION_HPP_