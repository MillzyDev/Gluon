#ifndef GLUON_METHODS_HPP_
#define GLUON_METHODS_HPP_

#include <array>
#include <cstdint>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include "il2cpp-api.h"
#include "il2cpp-tabledefs.h"

#include "classes.hpp"
#include "boxing.hpp"
#include "gluon_config.hpp"
#include "result.hpp"
#include "type_concepts.hpp"

namespace Gluon::Methods {
    /**
     * @brief Specifies how an IL2CPP object should be created.
     */
    enum struct CreationType {
        /**
         * @brief Created object is a "managed" object; can be GC'd
         */
        Temporary,
        /**
         * @brief Created object is manual, must be freed explicitly via delete.
         */
        Manual
    };

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
     * Manually creates an instance of the provided Il2CppClass *.
     * The created instance's type initialiser will NOT execute on another thread.
     * Must be freed using gc_free_specific
     * @param klass The Il2CppClass to create an instance of
     * @return The created instance, or nullptr if it failed for any reason.
     */
    Il2CppObject *createManual(const Il2CppClass *klass) noexcept;

    /**
     * Manually creates an instance of the provided Il2CppClass *.
     * The created instance's type initialiser will NOT execute on another thread.
     * Must be freed using gc_free_specific
     * Throws a Gluon::Exceptions::StackTraceException on failure
     * @param klass The Il2CppClass to create an instance of
     * @return The created instance, or nullptr if it failed for any reason.
     */
    Il2CppObject *createManualThrow(const Il2CppClass *klass);

    template <class T>
    Il2CppObject *toIl2CppObject(T &&arg) {
        Gluon::Il2CppFunctions::initialise();

        using Dt = std::decay_t<T>;
        if constexpr (std::is_same_v<Dt, Il2CppType *> || std::is_same_v<Dt, Il2CppClass *>) {
            return nullptr;
        }

        Il2CppClass *klass = RET_0_UNLESS(Gluon::Classes::extractClass(arg));
        return Gluon::Il2CppFunctions::value_box(klass, &arg);
    }

    template <class T>
    void *extractValue(T &&arg) {
        Gluon::Il2CppFunctions::initialise();

        using Dt = std::decay_t<T>;
        if constexpr (std::is_same_v<Dt, Il2CppType *> || std::is_same_v<Dt, Il2CppClass *>) {
            return nullptr;
        }
        else if constexpr (std::is_pointer_v<Dt>) {
            if constexpr (std::is_base_of_v<Il2CppObject, std::remove_pointer_t<Dt>>) {
                if (arg) {
                    Il2CppClass *klass = Gluon::Il2CppFunctions::object_get_class(reinterpret_cast<Il2CppObject *>(arg));
#ifdef UNITY_2021
                    if (klass && Gluon::Il2CppFunctions::class_is_valuetype(klass)) {
#else // UNITY_2021
                    if (klass && klass->is_valuetype) {
#endif // UNITY_2021
                        // arg is an Il2CppObject * of a value type; it needs unboxing
                        return Gluon::Il2CppFunctions::object_unbox(reinterpret_cast<Il2CppObject *>(arg));
                    }
                }
            }
            return arg;
        }
        else if constexpr (Gluon::TypeConcepts::HasIl2CppConversion<Dt>) {
            return arg.convert();
        }
        else {
            return const_cast<Dt *>(&arg);
        }
    }

        template <class T>
        void *extractTypeValue(T &arg) {
            using Dt = std::decay_t<T>;

            if constexpr (std::is_same_v<std::nullptr_t, T>) {
                return nullptr;
            }
            else if constexpr (Gluon::TypeConcepts::HasIl2CppConversion<T>) {
                return arg.convert();
            }
            else if constexpr (std::is_pointer_v<Dt>) {
                // For a pointer type, get the IL2CPP class and unbox
                auto *klass = CLASS_OF(Dt);
                if (klass && Gluon::Il2CppFunctions::class_is_valuetype(klass)) {
                    // Arg is an object of a value type and must be unboxed
                    return Gluon::Il2CppFunctions::object_unbox(reinterpret_cast<Il2CppObject *>(arg));
                }

                return arg;
            }
            else {
                return const_cast<Dt *>(&arg);
            }
        }

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
        return Gluon::Methods::findMethod(FindMethodInfo(klass, methodName, genericTypesSpan, argTypesSpan));
    }

    // no generic args
    template <typename T, typename TArgs>
    requires(!std::is_constructible_v<T, std::string_view>)
    const MethodInfo *findMethod(T &&instanceOrClass, std::string_view methodName, TArgs &&argTypes) {
        return Gluon::Methods::findMethod(std::forward<T>(instanceOrClass), methodName, std::forward<TArgs>(argTypes));
    }

    // no args
    template <typename T>
    requires(!std::is_constructible_v<T, std::string_view>)
    const MethodInfo *findMethod(T &&instanceOrClass, std::string_view methodName) {
        return Gluon::Methods::findMethod(std::forward<T>(instanceOrClass), methodName, std::span<const Il2CppClass *const>(), std::span<const Il2CppType *const>());
    }

    // generic and array args
    template <typename TGenerics, typename TArgs>
    const MethodInfo *findMethod(const std::string_view namespaze, const std::string_view className, std::string_view methodName, TGenerics &&genericTypes, TArgs &&argTypes) {
        Il2CppClass *klass = Gluon::Classes::getClassFromName(namespaze, className);
        return Gluon::Methods::findMethod(klass, methodName, std::forward<TGenerics>(genericTypes), std::forward<TArgs>(argTypes));
    }

    // no generic args
    template <typename TArgs>
    const MethodInfo *findMethod(const std::string_view namespaze, const std::string_view className, std::string_view methodName, TArgs &&argTypes) {
        Il2CppClass *klass = Gluon::Classes::getClassFromName(namespaze, className);
        return Gluon::Methods::findMethod(klass, methodName, std::span<const Il2CppClass *const>(), std::forward<TArgs>(argTypes));
    }

    // no args
    inline const MethodInfo *findMethod(const std::string_view namespaze, const std::string_view className, std::string_view methodName) {
        Il2CppClass *klass = Gluon::Classes::getClassFromName(namespaze, className);
        return Gluon::Methods::findMethod(klass, methodName, std::span<const Il2CppClass *const>(), std::span<const Il2CppType *const>());
    }

    bool isConvertibleFrom(const Il2CppType *to, const Il2CppType *from, bool asArgs = true);

    inline const Il2CppGenericContainer *getGenericContainer(const MethodInfo *method) {
        if (!method->is_generic) {
            SAFE_ABORT_MSG("METHOD IS NOT GENERIC");
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
        return Gluon::Methods::resolveVTableSlot(Gluon::Classes::extractClass(instance), args...);
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
        return Gluon::Methods::parameterMatch<0, argsCount>(method, std::span<const Il2CppClass *const, 0>(), argTypes, isIdenticalOut);
    }

    template <bool strictEqual = false, std::size_t genericsCount, std::size_t argsCount>
    bool parameterMatch(const MethodInfo *method, std::array<const Il2CppClass *, genericsCount> const &genericTypes,
                        std::array<const Il2CppType *, argsCount> const &argTypes, std::optional<bool *> isIdenticalOut) {
        return Gluon::Methods::parameterMatch<genericsCount, argsCount>(method, genericTypes, argTypes, isIdenticalOut);
    }

    template <bool strictEqual = false, size_t argsCount>
    bool parameterMatch(const MethodInfo *method, std::array<const Il2CppType *, argsCount> const &argTypes,
                        std::optional<bool *> isIdenticalOut) {
        return Gluon::Methods::parameterMatch<0, argsCount>(method, std::span<const Il2CppClass *const, 0>(), argTypes, isIdenticalOut);
    }

    template <typename TOut>
    using MethodResult = Gluon::Result<TOut, Gluon::Exceptions::RunMethodException>;

    template <class TOut = Il2CppObject *, bool checkTypes = true, class T, class ...TArgs>
    requires(!std::is_convertible_v<T, std::string_view> || std::is_same_v<T, std::nullptr_t>)
    MethodResult<TOut> runMethod(T &&wrappedInstance, const MethodInfo *method, TArgs &&...params) noexcept {
        if (!method) {
            return Gluon::Exceptions::RunMethodException("MethodInfo cannot be null!", nullptr);
        }

        if constexpr (checkTypes) {
            // only check args if TArgs is > 0
            if (method->parameters_count != sizeof...(TArgs)) {
                Gluon::Logger::warn("MethodInfo parameter count {} does not match actual parameter count {}",
                                    method->parameters_count, sizeof...(TArgs));
            }

            if constexpr (sizeof...(TArgs) > 0) {
                std::array<const Il2CppType *, sizeof...(TArgs)> types{ Gluon::Classes::extractType(params)... };

                if (!parameterMatch(method, types, std::nullopt)) {
                    return Gluon::Exceptions::RunMethodException("Parameters do not match", method);
                }
            }

            if constexpr (!std::is_same_v<TOut, void>) {
                if (const Il2CppType *outType = Gluon::Classes::extractIndependentType<TOut>();
                    outType && !isConvertibleFrom(outType, method->return_type, false)) {
                    Gluon::Logger::warn("User requested TOut {} does not match method's return type {}",
                                        Gluon::Classes::getTypeSimpleName(outType),
                                        Gluon::Classes::getTypeSimpleName(method->return_type));

                    return Gluon::Exceptions::RunMethodException(std::format("Return type of method is not convertible to {}",
                        Gluon::Classes::getTypeSimpleName(outType)), method);
                }
            }
        }

        const void *inst = Gluon::Methods::extractValue(wrappedInstance);

        bool isStatic = method->flags & METHOD_ATTRIBUTE_STATIC;
        if (!isStatic && !inst) {
            return Gluon::Exceptions::RunMethodException("Method is instance but instance is null", method);
        }

        Il2CppException *exception = nullptr;
        std::array<void *, sizeof...(params)> invokeParams{ extractTypeValue(params)... };

        Gluon::Il2CppFunctions::initialise();

        auto *ret = Gluon::Il2CppFunctions::runtime_invoke(method, inst, invokeParams.data(), &exception);

        if (exception) {
            return Gluon::Exceptions::RunMethodException(exception, method);
        }

        // void return
        if constexpr (std::is_same_v<void, TOut>) {
            return Gluon::Methods::MethodResult<TOut>();
        }

        if constexpr (checkTypes) {
            if (ret) {
                // This is done instead of extractType to avoid unboxing because we cannot be certain of the return type
                if (const Il2CppType *outType = Gluon::Classes::extractIndependentType<TOut>()) {
                    if (const Il2CppType *returnType = Gluon::Classes::extractType(ret);
                        !isConvertibleFrom(outType, returnType, false)) {
                        Gluon::Logger::warn("User requested TOut {} does not match method's return type {}",
                                        Gluon::Classes::getTypeSimpleName(outType),
                                        Gluon::Classes::getTypeSimpleName(returnType));
                    }
                }
            }
        }

        if constexpr (!std::is_same_v<void, TOut>) {
            if constexpr (Gluon::TypeCheck::NeedBox<TOut>::value) { // returned value is boxed
                auto retValue = Gluon::Boxing::unbox<TOut>(ret);
                Gluon::Il2CppFunctions::il2cpp_gc_free_fixed(retValue);
                return retValue;
            }
            else if constexpr (Gluon::TypeConcepts::Il2CppReferenceTypeWrapper<TOut>) { // returned value is a reference type
                return TOut(ret);
            }
            else { // probably a ref type pointer
                return static_cast<TOut>(static_cast<void *>(ret));
            }
        }
    }

    template <class TOut = Il2CppObject *, bool checkTypes = true, class T, class ...TArgs>
    MethodResult<TOut> runMethod(T &&classOrInstance, std::string_view name, TArgs &&...params) {
        std::array<const Il2CppType *, sizeof...(TArgs)> const types{ Gluon::Classes::extractType(params)... };
        const MethodInfo *method = RET_NULLOPT_UNLESS(Gluon::Methods::findMethod(classOrInstance, method, types));
        return Gluon::Methods::runMethod<TOut, checkTypes>(std::forward<T>(classOrInstance), method, std::forward<TArgs>(params)...);
    }

    template <class TOut = Il2CppObject *, bool checkTypes = true, class ...TArgs>
    MethodResult<TOut> runMethod(std::string_view namespaze, std::string_view className, std::string_view name, TArgs &&...params) {
        Il2CppClass *klass = RET_NULLOPT_UNLESS(Gluon::Classes::getClassFromName(namespaze, className));
        return Gluon::Methods::runMethod<TOut, checkTypes>(klass, name, params...);
    }

    template <class TOut = void, bool checkTypes = true, class ...TArgs>
    TOut runMethodRethrow(TArgs &&...params) {
        auto result = runMethod<TOut, checkTypes>(std::forward<TArgs>(params)... );

        if constexpr (!std::is_same_v<TOut, void>) {
            return result.getOrRethrow();
        }
        else if constexpr (std::is_same_v<TOut, void>) {
            return result.rethrow();
        }
    }

    template <class TOut = void, bool checkTypes = true, class ...TArgs>
    std::optional<Gluon::TypeOrMonostate<TOut>> runMethodOpt(TArgs &&...params) noexcept {
        auto result = runMethod<TOut, checkTypes>(std::forward<TArgs>(params)...);

        if (const auto exception = result.asOptionalException()) {
            Gluon::Logger::error("{}: failed with exception: {}",
                Gluon::Il2CppFunctions::method_get_name(exception.value()->info),
                Gluon::Exceptions::exceptionToString(exception.value()->ex).c_str());
            return std::nullopt;
        }

        return result.getResult();
    }

    /**
     * Allocates a new instance of a particular Il2CppClass *, either allowing it to be GC'd normally or manually controlled.
     * The Il2CppClass * is derived from the TOut template parameter.
     * The found constructor method will be cached.
     * Will throw either a Gluon::Exceptions::StackTraceException or Gluon::Exceptions::RunMethodException if errors occur.
     * @tparam TOut The type to create
     * @tparam creationType How the instance should be created
     * @tparam TArgs The arguments to call the constructor with
     * @param args The arguments to call the constructor with
     */
    template <class TOut, CreationType creationType = CreationType::Temporary, typename ...TArgs>
    TOut newSpecificUnsafe(TArgs &&...args) {
        static Il2CppClass *klass = CLASS_OF(TOut);

        Il2CppObject *object;

        if constexpr (creationType == CreationType::Temporary) {
            object = Gluon::Il2CppFunctions::object_new(klass);

            if (!object) {
                throw Gluon::Exceptions::StackTraceException("Failed to allocate new object via object_new");
            }
        }
        else {
            object = Gluon::Methods::createManualThrow(klass);
        }

        // Only need to extract based off of types, since we are assuming TOut is CLASS_OF-able anyway
        static const MethodInfo *ctor = findMethod(klass, ".ctor",
            std::array<const Il2CppType *, sizeof...(TArgs)>{ Gluon::Classes::extractIndependentType<std::decay_t<TArgs>>()... });

        if (!ctor) {
            throw Gluon::Exceptions::StackTraceException(std::format(
                "Failed tro find matching .ctor during construction of type: {}",
                Gluon::Classes::getClassStandardName(klass)
                ));
        }

        Gluon::Methods::runMethodRethrow<void, false>(object, ctor, std::forward<TArgs>(args)... );

        if constexpr (std::is_pointer_v<TOut>) {
            return reinterpret_cast<TOut>(object);
        }
        else if constexpr (Gluon::TypeConcepts::HasIl2CppConversion<TOut>) {
            // Handle construction for wrapper types, construct from void *s
            return TOut(reinterpret_cast<void *>(object));
        }
        else {
            static_assert(false_t<TOut>, "Cannot C# construct the provided value type that is not a wrapper type!");
        }
    }

    template<class T, class ...TArgs>
    concept CtorArgs = requires(T type, TArgs &&...args) {
        { T::New_ctor(std::forward<TArgs>(args)...) }; // TODO: look into changing this for our cordl fork
    };

    template <class TOut, CreationType creationType = CreationType::Temporary, typename ...TArgs>
    requires(CtorArgs<std::remove_pointer_t<TOut>, TArgs...>)
    TOut newSpecific(TArgs &&...args) {
        return Gluon::Methods::newSpecificUnsafe<TOut, creationType, TArgs...>(std::forward<TArgs>(args)...);
    }

}

#endif // GLUON_METHODS_HPP_