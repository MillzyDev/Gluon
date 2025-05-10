#ifndef GLUON_CLASSES_HPP_
#define GLUON_CLASSES_HPP_

#include <string>

#include "gluon_config.hpp"
#include "il2cpp-class-internals.h"

namespace Gluon::Classes {
    GLUON_API const char *getTypeSimpleName(const Il2CppType *type);

    GLUON_API std::string getClassStandardName(const Il2CppClass *klass, bool generics = true);

    GLUON_API Il2CppClass *findClassFromName(std::string_view namespaze, std::string_view name);

    GLUON_API Il2CppReflectionType *getSystemType(const Il2CppClass *klass);
    GLUON_API Il2CppReflectionType *getSystemType(const Il2CppType *type);
}

#endif // GLUON_CLASSES_HPP_