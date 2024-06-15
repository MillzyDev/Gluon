#ifndef GLUON_ABORTION_HPP_
#define GLUON_ABORTION_HPP_

#include <cstdint>
#include <type_traits>
#include <unistd.h>

#include "gluon_logging.hpp"

#define CRASH_UNLESS(...) Gluon::crashUnless(__VA_ARGS__, __PRETTY_FUNCTION__, __FILE__, __LINE__)
#define SAFE_ABORT() Gluon::safeAbort(__PRETTY_FUNCTION__, __FILE__, __LINE__)

namespace Gluon {
    template <class, template <class, class...> class>
    struct IsInstance : public std::false_type {};

    template <class...Ts, template <class, class...> class U>
    struct IsInstance<U<Ts...>, U> : public std::true_type {};

    [[noreturn]] inline void safeAbort(const char *func, const char *file, int line, uint16_t frameCount = 512) {
        for (int i = 0; i < 2; i++) {
            usleep(100000); // 0.1s
            Gluon::Logging::Logger::warn("Aborting in {} at {}:{}", func, file, line);
        }
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
}

#endif