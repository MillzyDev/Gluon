#ifndef GLUON_IL2CPP_METHODS_HPP_
#define GLUON_IL2CPP_METHODS_HPP_

#include <algorithm>
#include <span>
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
        Manual,
    };

    struct FindMethodInfo {
        Il2CppClass *klass = nullptr;
        const std::string_view name;
        const std::span<const Il2CppClass *const> genericTypes;
        const std::span<const Il2CppType *const> argumentTypes;

        constexpr FindMethodInfo() = delete;
        constexpr FindMethodInfo(FindMethodInfo &&) = default;
        constexpr FindMethodInfo(const FindMethodInfo &) = default;
        constexpr FindMethodInfo(Il2CppClass *klass, const std::string_view name,
                                 const std::span<const Il2CppClass *const> genericTypes,
                                 const std::span<const Il2CppType *const> argumentTypes)
                                 : klass(klass), name(name), genericTypes(genericTypes), argumentTypes(argumentTypes)
                                 {};

        bool operator ==(const FindMethodInfo &o) const {
            if (this->klass != o.klass) {
                return false;
            }
            if (this->name != o.name) {
                return false;
            }

            auto argumentsTypeEquality = this->argumentTypes.size() == o.argumentTypes.size()
                    && std::equal(this->argumentTypes.begin(), this->argumentTypes.end(),
                                  o.argumentTypes.begin(),o.argumentTypes.end());
            if (!argumentsTypeEquality) {
                return false;
            }

            auto genericsTypeEquality = this->genericTypes.size() == o.genericTypes.size()
                                    && std::equal(this->genericTypes.begin(), this->genericTypes.end(),
                                                  o.genericTypes.begin(),o.genericTypes.end());
            if (!genericsTypeEquality) {
                return false;
            }

            return true;
        }

        bool operator !=(const FindMethodInfo &) const = default;
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

    const MethodInfo *findMethod(const FindMethodInfo &info);
}

#endif