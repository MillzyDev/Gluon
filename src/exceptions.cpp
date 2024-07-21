#include "exceptions.hpp"

#include "abortion.hpp"
#include "il2cpp_functions.hpp"
#include "gluon_logging.hpp"

namespace Gluon::Exceptions {

    void StackTraceException::logBacktrace() const {
        Gluon::Logger::logBacktraceFull(this->stackTraceBuffer, this->stackTraceSize);
    }

    std::string exceptionToString(const Il2CppException *exception) noexcept {
        Gluon::Il2CppFunctions::initialise();

        constexpr std::size_t kExceptionMessageSize = 4096;

        char message[kExceptionMessageSize];
        Gluon::Il2CppFunctions::format_exception(exception, message, kExceptionMessageSize);

        return message;
    }

    [[noreturn]] void raiseException(const Il2CppException *exception) {
        CRASH_UNLESS(exception);
        Gluon::Il2CppFunctions::raise_exception(const_cast<Il2CppException *>(exception));
        __builtin_unreachable();
    }
}