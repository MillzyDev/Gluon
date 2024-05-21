#ifndef GLUON_IL2CPP_CLASSES_HPP_
#define GLUON_IL2CPP_CLASSES_HPP_

#include <string>

#include "gluon_config.hpp"
#include "il2cpp-class-internals.h"

namespace Gluon::Classes {
    GLUON_API std::string getClassStandardName(const Il2CppClass *klass, bool generics = true);

    GLUON_API Il2CppClass *findClassFromName(std::string_view namespaze, std::string_view name);
}

#endif