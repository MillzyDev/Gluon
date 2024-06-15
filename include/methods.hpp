#ifndef GLUON_IL2CPP_METHODS_HPP_
#define GLUON_IL2CPP_METHODS_HPP_

#include <string_view>

#include "il2cpp_functions.hpp"
#include "il2cpp-object-internals.h"

namespace Gluon::Methods {
    enum struct CreationType {
        /**
         * @brief C# object, able to be garbage collected.
         */
        Temporary,
        /**
         * @brief Must be explicitly freed via delete.
         */
        Manual
    };

    /**
     * @brief Creates a manual instance of the provided Il2CppClass *.
     * Type initialisers will NOT execute on other threads!
     * The created instance must be explicitly freed.
     *
     * @param klass The Il2CppClass to instantiate.
     * @return The created instance. Is nullptr if failed to create.
     */
    Il2CppObject *createManual(const Il2CppClass *klass) noexcept;

    /**
     * @brief Creates a manual instance of the provided Il2CppClass *.
     * Type initialisers will NOT execute on other threads!
     * The created instance must be explicitly freed.
     *
     * @param klass The Il2CppClass to instantiate.
     * @return The created instance.
     */
    Il2CppObject *createManualThrow(const Il2CppClass *klass);

    const MethodInfo *findMethodUnsafe(const Il2CppClass *klass, std::string_view methodName, int argsCount);
}

#endif