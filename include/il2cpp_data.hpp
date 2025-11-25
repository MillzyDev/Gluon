#ifndef GLUON_IL2CPP_DATA_HPP_
#define GLUON_IL2CPP_DATA_HPP_

#include "gluon_config.hpp"

#include "il2cpp-class-internals.h"

namespace Gluon {
    class GLUON_API Il2CppData {
    private:
        static Il2CppDefaults *_il2cppDefaults;

        static Il2CppDefaults *traceIl2CppDefaults();

    public:
        static Il2CppDefaults *getDefaults();

        static void initialise();
    }; // Il2CppData
} // Gluon

#endif // GLUON_IL2CPP_DATA_HPP_
