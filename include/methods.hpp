#ifndef GLUON_METHODS_HPP_
#define GLUON_METHODS_HPP_

#include <classes.hpp>
#include <cstdint>
#include <string_view>

#include "il2cpp-api.h"

#include "classes.hpp"
#include "gluon_config.hpp"

namespace Gluon::Methods {
    GLUON_API const MethodInfo *findMethodUnsafe(const Il2CppClass *klass, std::string_view methodName, int argsCount);
    GLUON_API const MethodInfo *findMethodUnsafe(std::string_view namespaze, std::string_view className, std::string_view methodName, int argsCount);
    GLUON_API const MethodInfo *findMethodUnsafe(Il2CppObject *instance, std::string_view methodName, int argsCount);

    // TODO IMPL
    GLUON_API const MethodInfo *resolveVTableSlot(Il2CppClass *klass, Il2CppClass *declaringClass, std::uint16_t slot) noexcept;
    GLUON_API const MethodInfo *resolveVTableSlot(Il2CppClass *klass, std::string_view declaringNamespace, std::string_view declaringClassName, std::uint16_t slot) noexcept;

    template <typename T, typename... TArgs>
    requires(!std::is_same_v<T, Il2CppClass *>)
    const MethodInfo *resolveVTableSlot(T &&instance, TArgs &&...args) noexcept {
        return resolveVTableSlot(Gluon::Classes::extractClass(instance, args...));
    }
}

#endif // GLUON_METHODS_HPP_