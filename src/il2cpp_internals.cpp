#include "il2cpp_internals.hpp"

#include "il2cpp_functions.hpp"
#include "xref_helpers.hpp"
#include "il2cpp-class-internals.h"

namespace Gluon {
    bool Il2CppInternals::initialised = false;
    const Il2CppDefaults *Il2CppInternals::defaults;

    void Il2CppInternals::initialise() {
        if (initialised) {
            return;
        }
        initialised = true;

        // TODO: defaults type map, avoiding an xref trace because pain
    }
}