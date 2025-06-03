#ifndef GLUON_EXCEPTIONS_HPP_
#define GLUON_EXCEPTIONS_HPP_

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

        explicit StackTraceException(const std::string_view message) : std::runtime_error(message.data()) { // NOLINT(*-pro-type-member-init)
            stackTraceSize = Gluon::BacktraceHelpers::captureBacktrace(stackTraceBuffer, kStackTraceMaxSize, 0);
        }

        GLUON_API void logBacktrace() const;

        [[nodiscard]] const char *what() const noexcept override {
            logBacktrace();
            return std::runtime_error::what();
        }
    };

    struct NullException final : public StackTraceException {
        using StackTraceException::StackTraceException;

        explicit NullException(const std::string_view message) : StackTraceException(message) {}
    };

    struct BadCastException final : public StackTraceException {
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

    struct ResultException final : StackTraceException {
        explicit ResultException(const std::string_view message) : StackTraceException(message) {}
    };

    struct RunMethodException final : std::runtime_error {
        constexpr static uint16_t kStackTraceSize = 256;

        const Il2CppException *exception;
        const MethodInfo *info;
        void *stackTraceBuffer[kStackTraceSize];
        uint16_t stackTraceSize;

        RunMethodException(const std::string_view message, const MethodInfo *info) __attribute__((noinline)) : std::runtime_error(
                                                                                                                   message.data()), exception(nullptr), info(info) {
            stackTraceSize = Gluon::BacktraceHelpers::captureBacktrace(stackTraceBuffer, kStackTraceSize, 0);
        }

        RunMethodException(const Il2CppException *exception, const MethodInfo *info) __attribute__((noinline)) : std::runtime_error(
                                                                                                                     exceptionToString(exception)), exception(exception), info(info) {
            stackTraceSize = Gluon::BacktraceHelpers::captureBacktrace(stackTraceBuffer, kStackTraceSize, 0);
        }

        GLUON_API void logBacktrace() const;

        [[noreturn]] void rethrow() const {
            raiseException(exception);
        }

        [[nodiscard]] virtual const char *what() const noexcept override {
            logBacktrace();
            return std::runtime_error::what();
        }
    };

    /// @brief Represents an exception thrown from usage of an Array.
    struct ArrayException final : Gluon::Exceptions::StackTraceException {
        void* arrayInstance;
        ArrayException(void* instance, const std::string_view msg) : Gluon::Exceptions::StackTraceException(msg.data()), arrayInstance(instance) {}
    };
}

#endif // GLUON_EXCEPTIONS_HPP_