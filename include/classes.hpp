#ifndef GLUON_CLASSES_HPP_
#define GLUON_CLASSES_HPP_

#include <span>
#include <string>

#include "gluon_config.hpp"
#include "type_check.hpp"

#include "il2cpp-class-internals.h"

namespace Gluon::Classes {
    GLUON_API const char *getTypeSimpleName(const Il2CppType *type);

    GLUON_API std::string getClassStandardName(const Il2CppClass *klass, bool generics = true);

    GLUON_API Il2CppReflectionType *getSystemType(const Il2CppClass *klass);
    GLUON_API Il2CppReflectionType *getSystemType(const Il2CppType *type);

    template <typename T>
    Il2CppClass *extractClass(T &&arg) {
        using decayT = std::decay_t<T>;
        using argClass = Gluon::TypeCheck::Il2CppArgClass<decayT>;

        Il2CppClass *klass = argClass::get(arg);
        if (!klass) {
            Gluon::Logger::error("Failed to determine class - Tip: pass the Il2CppType* or Il2CppClass* of the argument instead of nullptr.");
        }

        return klass;
    }

    GLUON_API Il2CppReflectionType *makeGenericType(Il2CppReflectionType *genericType, Il2CppArray *types);
    GLUON_API Il2CppClass *makeGeneric(const Il2CppClass *klass, std::span<const Il2CppClass *const> args);
    GLUON_API Il2CppClass *makeGeneric(const Il2CppClass *klass, const Il2CppType **types, std::uint32_t count);

    GLUON_API Il2CppClass *getClassFromName(std::string_view namespaze, std::string_view name);
}

#endif // GLUON_CLASSES_HPP_