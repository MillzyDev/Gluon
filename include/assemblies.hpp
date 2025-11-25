#ifndef GLUON_ASSEMBLIES_HPP_
#define GLUON_ASSEMBLIES_HPP_

#include <span>

#include "il2cpp_functions.hpp"

#include "il2cpp-class-internals.h"

namespace Gluon::Assemblies {
    std::span<const Il2CppAssembly *> getAssemblies();

    inline const Il2CppImage *getImage(const Il2CppAssembly *assembly) {
        return Gluon::Il2CppFunctions::assembly_get_image(assembly);
    }
} // Gluon::Assemblies

#endif // GLUON_ASSEMBLIES_HPP_
