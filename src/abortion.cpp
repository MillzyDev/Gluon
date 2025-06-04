#include "abortion.hpp"

#include "exceptions.hpp"

namespace Gluon {
    void throwException(const std::string &message) {
        throw Gluon::Exceptions::StackTraceException(message);
    }

}