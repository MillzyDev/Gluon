#include "exceptions.hpp"

namespace Gluon::Exceptions {


    void StackTraceException::logBacktrace() const {
        // TODO
    }

    const char *StackTraceException::what() const noexcept {
        logBacktrace();
        return std::runtime_error::what();
    }

}