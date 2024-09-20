#ifndef GLUON_IL2CPP_METHODS_HPP_
#define GLUON_IL2CPP_METHODS_HPP_

#include <algorithm>
#include <span>
#include <string_view>

#include "il2cpp_functions.hpp"
#include "il2cpp-object-internals.h"
#include "il2cpp-class-internals.h"
#include "vm/GlobalMetadataFileInternals.h"

struct Il2CppGenericContainer;

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
    GLUON_API Il2CppObject *createManual(const Il2CppClass *klass) noexcept;

    /**
     * @brief Creates a manual instance of the provided Il2CppClass *.
     * Type initialisers will NOT execute on other threads!
     * The created instance must be explicitly freed.
     *
     * @param klass The Il2CppClass to instantiate.
     * @return The created instance.
     */
    GLUON_API Il2CppObject *createManualThrow(const Il2CppClass *klass);

    GLUON_API const MethodInfo *findMethodUnsafe(const Il2CppClass *klass, std::string_view methodName, int argsCount);

    GLUON_API const MethodInfo *findMethod(const FindMethodInfo &info);

    GLUON_API bool isConvertibleFrom(const Il2CppType *to, const Il2CppType *from, bool asArgs = true);

    inline const Il2CppGenericContainer *getGenericContainer(const MethodInfo *method) {
        if (!method->is_generic) {
            SAFE_ABORT_MSG("METHOD IS NOD GENERIC");
        }

        if (method->is_inflated) {
            // pyrocynical lol
            const Il2CppGenericMethod *genericMethod = method->genericMethod;
            return reinterpret_cast<const Il2CppGenericContainer *>(genericMethod->methodDefinition->genericContainerHandle);
        }
        else {
            return reinterpret_cast<const Il2CppGenericContainer *>(method->genericContainerHandle);
        }
    }

    template<size_t genericsSize, size_t argsSize>
    bool parameterMatch(const MethodInfo *method, const std::span<const Il2CppClass *const, genericsSize> genericTypes,
                        std::span<const Il2CppType* const, argsSize> const argTypes, std::optional<bool *> isIdenticalOut) {
        Gluon::Il2CppFunctions::initialise();

        if (method->parameters_count != argTypes.size()) {
            Gluon::Logger::warn("Potential method match had wrong number of parameters {} (expected {})", method->parameters_count, argTypes.size());
            return false;
        }

        const Il2CppGenericContainer *genericContainer;

        int32_t genericsCount = 0;
        if (method->is_generic) {
            genericContainer = getGenericContainer(method);
            genericsCount = genericContainer->type_argc;
        }

        if (genericsCount != genericTypes.size()) {
            Gluon::Logger::warn("Potential method had wrong number of generics {} (expected {})", genericsCount, genericTypes.size());
            Gluon::Logger::warn("is generic {}, is inflated {}", method->is_generic, method->is_inflated);
        }

        bool isIdentical = true;
        bool matches = true;

        for (int i = 0; i < method->parameters_count; i++) {
            const Il2CppType *paramType = method->parameters[i];

            if (argTypes[i] == nullptr) {
                Gluon::Logger::warn("Arg type {} is null. Method: {:p}", i, reinterpret_cast<const void *>(method));
                // TODO: il2cpp_utils::LogMethod
                continue;
            }

            if (paramType->type == IL2CPP_TYPE_MVAR) {
                if (genericsCount == 0) {
                    Gluon::Logger::warn("No generic args to extract paramIdz {}", i);
                    continue;
                }

                auto genericIndex = Gluon::Il2CppFunctions::MetadataCache_GetGenericParameterIndexFromParameter(paramType->data.genericParameterHandle) - genericContainer->genericParameterStart;

                if (genericIndex < 0) {
                    Gluon::Logger::warn("Extracted invalid genericIndex {} from parameter {}", genericIndex, i);
                    continue;
                }

                if (genericIndex >= genericsCount) {
                    Gluon::Logger::warn("parameterMatch was no supplied enough genericTypes to determine type of parameter {} "
                                        "(had {}, needed {})!",
                                        i, genericsCount, genericIndex);
                    continue;
                }

                auto *klass = genericTypes[genericIndex];
                paramType = (paramType->byref) ? &klass->this_arg : &klass->byval_arg;
            }

            // identical if every parameter matches exactly
            isIdentical &= paramType == argTypes[i];

            if (!isConvertibleFrom(paramType, argTypes[i])) {
                matches = false;
                break;
            }
        }

        if (isIdenticalOut.has_value()) {
            *isIdenticalOut.value() = isIdentical;
        }

        return matches;
    }

    template<size_t argsSize>
    auto parameterMatch(const MethodInfo  *method, std::span<const Il2CppType *const, argsSize> const argTypes, std::optional<bool *> isIdenticalOut) {
        return parameterMatch<0, argsSize>(method, std::span<const Il2CppClass *const, 0>(), argTypes, isIdenticalOut);
    }

    template<bool strictEqual = false, size_t genericsSize, size_t argsSize>
    bool parameterMatch(const MethodInfo *method, std::array<const Il2CppClass *, genericsSize> const &genericTypes, std::array<const Il2CppType *, argsSize> const &argTypes, std::optional<bool *> isIdenticalOut) {
        return parameterMatch<genericsSize, argsSize>(method, genericTypes, argTypes, isIdenticalOut);
    }

    template<bool strictEqual = false, size_t size>
    bool parameterMatch(const MethodInfo *method, std::array<const Il2CppType *, size> const &argTypes, std::optional<bool *> isIdenticalOut) {
        return parameterMatch<0, size>(method, std::span<const Il2CppClass *const, 0>(), argTypes, isIdenticalOut);
    }

    /**
     * Logs information about the given method as a debug log
     * @param method Method to log
     */
    GLUON_API void logMethod(const MethodInfo *method);

    /**
     * Calls logMethod for all methods in a class
     * @param klass Class to log methods of
     * @param logParents Whether to log methods of superclasses
     */
    GLUON_API void logMethods(const Il2CppClass *klass, bool logParents = false);
}

#endif