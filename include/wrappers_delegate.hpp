#ifndef GLUON_WRAPPERS_DELEGATE_HPP_
#define GLUON_WRAPEPRS_DELEGATE_HPP_

#include "wrappers_object.hpp"

#include "il2cpp-object-internals.h"

namespace Gluon {
    struct MultiCastDelegate : Il2CppDelegate {
        Gluon::ArrayW<Il2CppDelegate *> delegates;
    };

    struct DelegateData : Il2CppObject {
        Il2CppReflectionType *targetType;
        Il2CppString *methodName;
        bool curiedFirstArg;
    };
}

#endif // GLUON_WRAPPERS_DELEGATE_HPP_