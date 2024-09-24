#ifndef GLUON_ASSEMBLIES_HPP_
#define GLUON_ASSEMBLIES_HPP_

#include <cstdint>
#include <string>
#include <vector>

#include "il2cpp_functions.hpp"
#include "gluon_config.hpp"

#include "il2cpp-class-internals.h"

namespace Gluon::Assemblies {
    inline Il2CppDomain *getIl2CppDomain() {
        return Gluon::Il2CppFunctions::domain_get();
    }

    inline const Il2CppAssembly *openAssembly(const std::string &name) {
        return Gluon::Il2CppFunctions::domain_assembly_open(getIl2CppDomain(), name.c_str());
    }

    inline const std::vector<const Il2CppAssembly *> getAssemblies() {
        size_t size;
        const Il2CppAssembly **assemblies = Gluon::Il2CppFunctions::domain_get_assemblies(getIl2CppDomain(), &size);
        return std::vector<const Il2CppAssembly *>(assemblies, assemblies + size);
    }

    GLUON_API const Il2CppAssembly *getAssembly(const std::string &name);
}

#endif // GLUON_ASSEMBLIES_HPP_