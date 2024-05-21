#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "assemblies.hpp"

#include "il2cpp-class-internals.h"

namespace Gluon::Assemblies {
    const Il2CppAssembly *getAssembly(const std::string &name) {
        static std::map<std::string, const Il2CppAssembly *> assemblyIndex;

        auto itr = assemblyIndex.find(name);
        if (itr != assemblyIndex.end()) {
            return itr->second;
        }

        assemblyIndex[name] = openAssembly(name);
        return assemblyIndex[name];
    }
}