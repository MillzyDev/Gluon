#include <cstdint>

#include "il2cpp_data.hpp"

#include "il2cpp_functions.hpp"
#include "tracers.hpp"

#include "il2cpp-class-internals.h"

namespace Gluon {
    Il2CppDefaults *Il2CppData::_il2cppDefaults = nullptr;

    Il2CppDefaults *Il2CppData::getDefaults() {
        return _il2cppDefaults;
    }

    Il2CppDefaults *Il2CppData::traceIl2CppDefaults() {
        //  il2cpp_init
        //      Runtime::Init (2nd CALL)
        //          il2cpp_defaults (6th LEA) (as of Unity 6000.0.62f1)
        // This is likely to move around, in earlier versions of Unity 6, it may be at the 4th or 5th LEA
        const std::uint32_t *addr_Runtime_Init = Tracers::findNthCall<2>(ADDR_PTR(Gluon::Il2CppFunctions::il2cpp_init));
        std::uint32_t *addr_il2cpp_defaults = Tracers::findNthLea<6>(addr_Runtime_Init);
        return reinterpret_cast<Il2CppDefaults *>(addr_il2cpp_defaults);
    }

    void Il2CppData::initialise() {
        Il2CppFunctions::initialise();

        _il2cppDefaults = traceIl2CppDefaults();
    }


} // Gluon