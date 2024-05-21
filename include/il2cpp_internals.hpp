#ifndef GLUON_IL2CPP_INTERNALS_HPP_
#define GLUON_IL2CPP_INTERNALS_HPP_

#include "gluon_config.hpp"
#include "il2cpp-class-internals.h"

namespace Gluon {
    class GLUON_API Il2CppInternals {
    public:
        static bool initialised;
        static const Il2CppDefaults *defaults;

        /**
         * @warning DO NOT CALL BEFORE IL2CPP HAS BEEN INITIALISED.
         */
        static void initialise();
    };
}

#endif