#ifndef GLUON_METHODS_HPP_
#define GLUON_METHODS_HPP_

#include <cstdint>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include "il2cpp-api.h"

#include "classes.hpp"
#include "gluon_config.hpp"

namespace Gluon::Methods {
    struct FindMethodInfo {
        Il2CppClass *klass = nullptr;
        std::string name;
        std::vector<const Il2CppClass *const> genericTypes;
        std::vector<const Il2CppType *const> argTypes;

        constexpr FindMethodInfo() = delete;
        constexpr FindMethodInfo(FindMethodInfo &&) = default;
        constexpr FindMethodInfo(FindMethodInfo const &) = delete;
        constexpr FindMethodInfo(Il2CppClass *klass, std::string_view name,
            std::span<const Il2CppClass *const> genericTypes, std::span<const Il2CppType *const> argTypes)
                : klass(klass), name(name), genericTypes(genericTypes.begin(), genericTypes.end()),
                  argTypes(argTypes.begin(), argTypes.end()) {}

        bool operator==(const FindMethodInfo & other) const {
            if (this->klass != other.klass) {
                return false;
            }

            if (this->name != other.name) {
                return false;
            }

            const bool isArgTypesEqual =
                this->argTypes.size() == other.argTypes.size() &&
                std::ranges::equal(this->argTypes, other.argTypes);
            if (!isArgTypesEqual) {
                return false;
            }

            const bool isGenericTypesEqual =
                this->genericTypes.size() == other.genericTypes.size() &&
                std::ranges::equal(this->genericTypes, other.genericTypes);
            if (!isGenericTypesEqual) {
                return false;
            }

            return true;
        }

        bool operator!=(FindMethodInfo const &) const = default;
    };

    /**
     * @brief Finds the first MethodInfo* described by the given Il2CppClass*, method name, and argument count.
     * Throws an Il2CppUtilException when: klass is null, or the method could not be found.
     * @return The found MethodInfo*
     * @param klass The Il2CppClass* to search for the method
     * @param methodName The il2cpp name of the method to find
     * @param argsCount The number of arguments to match (or -1 to not match at all)
     */
    GLUON_API const MethodInfo *findMethodUnsafe(const Il2CppClass *klass, std::string_view methodName, int argsCount);
    /**
     * @brief Find the first MethodInfo* of the class described by the namespace and className, described by the methodName, and argument count.
     * Throws an Il2CppUtilException when: the Il2CppClass* could not be found, or the method could not be found.
     * @return The found MethodInfo*
     * @param namespaze The namespace in which to search for the class
     * @param className The il2cpp name of the class to find
     * @param methodName The il2cpp name of the method to find
     * @param argsCount The number of arguments to match (or -1 to not match at all)
     */
    GLUON_API const MethodInfo *findMethodUnsafe(std::string_view namespaze, std::string_view className, std::string_view methodName, int argsCount);
    /**
     * @brief Find the first MethodInfo* on the given instance, described by the methodName, and argument count.
     * Throws an Il2CppUtilException when: instance is null, the Il2CppClass* could not be loaded, or the method could not be found.
     * @return The found MethodInfo*
     * @param instance The Il2CppObject* to search for the method
     * @param methodName The il2cpp name of the method to find
     * @param argsCount The number of arguments to match (or -1 to not match at all)
     */
    GLUON_API const MethodInfo *findMethodUnsafe(Il2CppObject *instance, std::string_view methodName, int argsCount);

    /**
     * Attempts to look for a method that best matches given the FindMethodInfo data
     * if no method is found, returns null
     * Look at methods.cpp for more details on how this resolution takes place
     */
    GLUON_API const MethodInfo *findMethod(FindMethodInfo &&info);

    // helper constructor
    template<typename T, typename TGenerics, typename TArgs>
    requires(!std::is_convertible_v<T, std::string_view> && std::is_constructible_v<std::span<const Il2CppClass *const>, TGenerics> && std::is_constructible_v<std::span<const Il2CppType *const>, TArgs>)
    const MethodInfo *findMethod(T &&instanceOrClass, std::string_view methodName, TGenerics &&genericTypes, TArgs&&argTypes) {
        Il2CppClass *klass = Gluon::Classes::extractClass(std::forward<T>(instanceOrClass));
        const auto genericTypesSpan = std::span<const Il2CppClass *const>(std::forward<TGenerics>(genericTypes));
        const auto argTypesSpan = std::span<const Il2CppType *const>(std::forward<TArgs>(argTypes));
        return findMethod(FindMethodInfo(klass, methodName, genericTypesSpan, argTypesSpan));
    }

    // no generic args
    template <typename T, typename TArgs>
    requires(!std::is_constructible_v<T, std::string_view>)
    const MethodInfo *findMethod(T &&instanceOrClass, std::string_view methodName, TArgs &&argTypes) {
        return findMethod(std::forward<T>(instanceOrClass), methodName, std::forward<TArgs>(argTypes));
    }

    // no args
    template <typename T>
    requires(!std::is_constructible_v<T, std::string_view>)
    const MethodInfo *findMethod(T &&instanceOrClass, std::string_view methodName) {
        return findMethod(std::forward<T>(instanceOrClass), methodName, std::span<const Il2CppClass *const>(), std::span<const Il2CppType *const>());
    }

    // generic and array args
    template <typename TGenerics, typename TArgs>
    const MethodInfo *findMethod(const std::string_view namespaze, const std::string_view className, std::string_view methodName, TGenerics &&genericTypes, TArgs &&argTypes) {
        Il2CppClass *klass = Gluon::Classes::getClassFromName(namespaze, className);
        return findMethod(klass, methodName, std::forward<TGenerics>(genericTypes), std::forward<TArgs>(argTypes));
    }

    // no generic args
    template <typename TArgs>
    const MethodInfo *findMethod(const std::string_view namespaze, const std::string_view className, std::string_view methodName, TArgs &&argTypes) {
        Il2CppClass *klass = Gluon::Classes::getClassFromName(namespaze, className);
        return findMethod(klass, methodName, std::span<const Il2CppClass *const>(), std::forward<TArgs>(argTypes));
    }

    // no args
    inline const MethodInfo *findMethod(const std::string_view namespaze, const std::string_view className, std::string_view methodName) {
        Il2CppClass *klass = Gluon::Classes::getClassFromName(namespaze, className);
        return findMethod(klass, methodName, std::span<const Il2CppClass *const>(), std::span<const Il2CppType *const>());
    }

    bool isConvertibleFrom(const Il2CppType *to, const Il2CppType *from, bool asArgs);

    inline const Il2CppGenericContainer *getGenericContainer(const MethodInfo *method) {
        if (!method->is_generic) {
            SAFE_ABORT("METHOD IS NOT GENERIC");
        }

        // pyrocynical lol
        if (method->is_inflated) {
#if UNITY_2021
            return reinterpret_cast<const Il2CppGenericContainer *>(method->genericMethod->methodDefinition->genericContainerHandle);
#else // UNITY_2021
            return method->genericMethod->methodDefinition->genericContainer;
#endif // UNITY_2021
        }

#if UNITY_2021
        return reinterpret_cast<const Il2CppGenericContainer *>(method->genericContainerHandle);
#else // UNITY_2021
            return method->genericContainer;
#endif // UNITy_2021
    }

    GLUON_API const MethodInfo *resolveMethodWithSlot(Il2CppClass *klass, std::uint16_t slot) noexcept;
    GLUON_API const MethodInfo *resolveVTableSlot(Il2CppClass *klass, Il2CppClass *declaringClass, std::uint16_t slot) noexcept;
    GLUON_API const MethodInfo *resolveVTableSlot(Il2CppClass *klass, std::string_view declaringNamespace, std::string_view declaringClassName, std::uint16_t slot) noexcept;

    template <typename T, typename... TArgs>
    requires(!std::is_same_v<T, Il2CppClass *>)
    const MethodInfo *resolveVTableSlot(T &&instance, TArgs &&...args) noexcept {
        return resolveVTableSlot(Gluon::Classes::extractClass(instance), args...);
    }

    /**
     * Returns whether a given MethodInfo's parameters match the Il2CppType vector
     * @param isIdenticalOut is true if every parameter type matches identically. Can be null
     */
    template <std::size_t genericsCount, std::size_t argsCount>
    bool parameterMatch(const MethodInfo *method, const std::span<const Il2CppClass *const, genericsCount> genericTypes,
                        const std::span<const Il2CppType *const, argsCount> argTypes,
                        const std::optional<bool *> isIdenticalOut) {

        Gluon::Il2CppFunctions::initialise();

        if (method->parameters_count != argTypes.size()) {
            Gluon::Logger::warn("Potential method had wrong number of parameters {} (expected {})", method->parameters_count, argTypes.size());
            return false;
        }

        const Il2CppGenericContainer *genericContainer;

        std::int32_t containerSize = 0;
        if (method->is_generic) {
            genericContainer = getGenericContainer(method);
            containerSize = genericContainer->type_argc;
        }

        if (containerSize != genericTypes.size()) {
            Gluon::Logger::warn("Potential method had wrong number of generics {} (expected {})", containerSize, genericTypes.size());
            Gluon::Logger::warn("is generic: {} is inflated: {}", method->is_generic, method->is_inflated);
            return false;
        }

        bool isIdentical = true;
        bool matches = true;

        for (std::uint8_t i = 0; i < method->parameters_count; i++) {
            const Il2CppType *parameterType = method->parameters[i];

            if (argTypes[i] == nullptr) {
                Gluon::Logger::warn("Arg type {} is null. Method: {:x}", i, reinterpret_cast<std::uintptr_t>(method));
                // TODO: Log method
                continue;
            }

            if (parameterType->type == IL2CPP_TYPE_MVAR) {
                if (containerSize == 0) {
                    Gluon::Logger::warn("No generic arguments to extract parameter index {}", i);
                    continue;
                }

                GenericParameterIndex genericParameterIndex =
                    Gluon::Il2CppFunctions::MetadataCache_GetGenericParameterIndexFromParameter(parameterType->data.genericParameterHandle)
                    // ReSharper disable once CppLocalVariableMightNotBeInitialized
                    - genericContainer->genericParameterStart;

                if (genericParameterIndex < 0) {
                    Gluon::Logger::warn("Extracted invalid generic parameter index {} from parameter {}", genericParameterIndex, i);
                    continue;
                }

                if (genericParameterIndex >= containerSize) {
                    Gluon::Logger::warn(
                        "parameterMatch was not supplied enough generic types to determine the type of parameter {}"
                        "(had {}, needed {})",
                        i, containerSize, genericParameterIndex);
                    continue;
                }

                const Il2CppClass *klass = genericTypes[i];
                parameterType = parameterType->byref ? &klass->this_arg : &klass->byval_arg;
            }

            // parameters are identical if every parameter matches exactly
            isIdentical &= parameterType == argTypes[i];

            if (!isConvertibleFrom(parameterType, argTypes[i])) {
                matches = false;
                break;
            }
        }

        // write to out
        if (isIdenticalOut.has_value()) {
            *isIdenticalOut.value() = isIdentical;
        }

        return matches;
    }

    template <std::size_t argsCount>
    bool parameterMatch(const MethodInfo *method, const std::span<const Il2CppType *const, argsCount> argTypes,
                        const std::optional<bool *> isIdenticalOut) {
        return parameterMatch<0, argsCount>(method, std::span<const Il2CppClass *const, 0>(), argTypes, isIdenticalOut);
    }

    template <bool strictEqual = false, std::size_t genericsCount, std::size_t argsCount>
    bool parameterMatch(const MethodInfo *method, std::array<const Il2CppClass *, genericsCount> const &genericTypes,
                        std::array<const Il2CppType *, argsCount> const &argTypes, std::optional<bool *> isIdenticalOut) {
        return parameterMatch<genericsCount, argsCount>(method, genericTypes, argTypes, isIdenticalOut);
    }

    template <bool strictEqual = false, size_t argsCount>
    bool parameterMatch(const MethodInfo *method, std::array<const Il2CppType *, argsCount> const &argTypes,
                        std::optional<bool *> isIdenticalOut) {
        return parameterMatch<0, argsCount>(method, std::span<const Il2CppClass *const, 0>(), argTypes, isIdenticalOut);
    }
}

#endif // GLUON_METHODS_HPP_