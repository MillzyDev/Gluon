#ifndef GLUON_IL2CPP_EXCEPTIONS_HPP_
#define GLUON_IL2CPP_EXCEPTIONS_HPP_

#include <cstdint>
#include <format>
#include <stdexcept>
#include <string_view>
#include <string>

#include "backtrace_helpers.hpp"
#include "classes.hpp"

#include "il2cpp-class-internals.h"

namespace Gluon::Exceptions {
    struct StackTraceException : std::runtime_error {
        static constexpr uint16_t kStackTraceMaxSize = 256;

        void *stackTraceBuffer[kStackTraceMaxSize];
        uint16_t stackTraceSize;

        explicit StackTraceException(std::string_view message) : std::runtime_error(message.data()) { // NOLINT(*-pro-type-member-init)
            stackTraceSize = Gluon::BacktraceHelpers::captureBacktrace(stackTraceBuffer, kStackTraceMaxSize, 0);
        }

        GLUON_API void logBacktrace() const;

        [[nodiscard]] const char *what() const noexcept override {
            logBacktrace();
            return std::runtime_error::what();
        }
    };

    struct NullException : public StackTraceException {
        using StackTraceException::StackTraceException;

        NullException(std::string_view message) : StackTraceException(message) {}
    };

    struct BadCastException : public StackTraceException {
        using StackTraceException::StackTraceException;
        const Il2CppClass *klass;
        const Il2CppClass *targetClass;
        Il2CppObject *instance;

        BadCastException(const Il2CppClass *klass, const Il2CppClass *targetClass, Il2CppObject *instance)
        : StackTraceException(std::format("Failed to cast {} to {}", Gluon::Classes::getClassStandardName(klass, true),
                                          Gluon::Classes::getClassStandardName(targetClass, true))),
                                          klass(klass),
                                          targetClass(targetClass),
                                          instance(instance) {}
    };

    GLUON_API std::string exceptionToString(const Il2CppException *exception) noexcept;

    [[noreturn]] GLUON_API void raiseException(const Il2CppException *exception);


}

#endif