#ifndef GLUON_IL2CPP_INTERNALS_HPP_
#define GLUON_IL2CPP_INTERNALS_HPP_

#include "gluon_config.hpp"
#include "il2cpp-class-internals.h"

namespace Gluon {
    class GLUON_API Il2cppInternals {
    public:
        static const Il2CppDefaults *defaults;

        static void Init();
    };
}

#endif