#include <span>
#include <vector>

#include "assemblies.hpp"

#include "il2cpp_functions.hpp"

#include "il2cpp-class-internals.h"

namespace Gluon::Assemblies {
    std::span<const Il2CppAssembly *> getAssemblies() {
        Gluon::Il2CppFunctions::initialise();

        Il2CppDomain *domain = Gluon::Il2CppFunctions::domain_get(); // technically not used and could be passed as nullptr

        std::size_t size; // \/ \/ \/ \/ "undefined behaviour" causer - best hope that vector doesn't get modified TwT
        const Il2CppAssembly **assemblies = Gluon::Il2CppFunctions::domain_get_assemblies(domain, &size);

        // ReSharper disable once CppDFALocalValueEscapesFunction
        // technically undefined behaviour
        // the vector this (sort of) addresses is in static storage and only gets populated once tho so :P
        return std::span(assemblies, size);
    }

    std::vector<const Il2CppClass *> getAssemblyClasses(const Il2CppAssembly *assembly) {
        const Il2CppImage *image = Gluon::Il2CppFunctions::assembly_get_image(assembly);
        std::size_t size = Gluon::Il2CppFunctions::image_get_class_count(image);

        std::vector<const Il2CppClass *> vec;
        Gluon::Il2CppFunctions::class_from_name()
    }
} // Gluon::Assemblies