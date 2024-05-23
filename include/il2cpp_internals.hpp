#ifndef GLUON_IL2CPP_INTERNALS_HPP_
#define GLUON_IL2CPP_INTERNALS_HPP_

#include "gluon_config.hpp"
#include "il2cpp-class-internals.h"

namespace Gluon {
    struct BuiltinClasses {
        Il2CppClass *booleanClass;
        Il2CppClass *byteClass;
        Il2CppClass *sByteClass;
        Il2CppClass *charClass;
        Il2CppClass *singleClass;
        Il2CppClass *doubleClass;
        Il2CppClass *int16Class;
        Il2CppClass *uint16Class;
        Il2CppClass *int32Class;
        Il2CppClass *uint32Class;
        Il2CppClass *int64Class;
        Il2CppClass *uint64Class;
        Il2CppClass *objectClass;
        Il2CppClass *stringClass;
        Il2CppClass *voidClass;
        Il2CppClass *typeClass;
    };

    class GLUON_API Il2CppInternals {
    public:
        static bool initialised;
        static BuiltinClasses builtinClasses;

        /**
         * @warning DO NOT CALL BEFORE IL2CPP HAS BEEN INITIALISED.
         */
        static void initialise();
    };
}

#endif