#ifndef GLUON_TYPE_CHECK_HPP_
#define GLUON_TYPE_CHECK_HPP_

#include <concepts>
#include <span>
#include <string>

#include <cxxabi.h>

#include "abortion.hpp"
#include "gluon_config.hpp"
#include "il2cpp_functions.hpp"

#include "il2cpp-object-internals.h"

#define NEED_NO_BOX(type)                                                                   \
    template<>                                                                              \
    struct Gluon::TypeCheck::NeedBox<type> {                                                \
        constexpr static bool value = false;                                                \
    };

#define DEFINE_IL2CPP_DEFAULT_TYPE(type, fieldName)                                         \
    template <>                                                                             \
    struct GLUON_HIDDEN Gluon::TypeCheck::Il2CppNoArgClass<type> {                          \
        static inline Il2CppClass *get() {                                                  \
            Gluon::Il2CppFunctions::initialise();                                           \
            return Gluon::Il2CppFunctions::il2cppDefaults->fieldName##_class;               \
        }                                                                                   \
    }

#define DEFINE_IL2CPP_ARG_TYPE(type, namespaze, name)                                       \
    template <>                                                                             \
    struct GLUON_HIDDEN Gluon::TypeCheck::Il2CppNoArgClass<type> {                          \
        static inline Il2CppClass *get() {                                                  \
            return Gluon::Classes::getClassFromName(namespaze, name);                       \
        }                                                                                   \
    }

#define DEFINE_IL2CPP_ARG_TYPE_GENERIC_STRUCT(templateType, namespaze, name)                \
    template <>                                                                             \
    struct GLUON_HIDDEN Gluon::TypeCheck::Il2CppGenStructNoArgClass<templateType> {         \
        static inline Il2CppClass *get() {                                                  \
            static Il2CppClass *klass = Gluon::Classes::getClassFromName(namepaze, name);   \
            return klass;                                                                   \
        }                                                                                   \
    }

#define DEFINE_IL2CPP_ARG_TYPE_GENERIC_CLASS(templateType, namespaze, name)                 \
    template <>                                                                             \
    struct GLUON_HIDDEN Gluon::TypeCheck::Il2CppGenClassNoArgClass<templateType> {          \
        static Il2CppClass *klass = Gluon::Classes::getClassFromName(namespaze, name);      \
        return klass;                                                                       \
    };                                                                                      \
    template <>                                                                             \
    struct GLUON_HIDDEN Gluon::TypeCheck::NeedBoxGen<templateType> {                        \
        constexpr static bool value = false;                                                \
    };

namespace Gluon::Classes {
    extern Il2CppClass *makeGeneric(const Il2CppClass *klass, std::span<const Il2CppClass *const> args);
    extern Il2CppClass *makeGeneric(const Il2CppClass *klass, const Il2CppType **types, std::uint32_t count);
    extern Il2CppClass *getClassFromName(std::string_view namespaze, std::string_view name);
}

namespace Gluon::TypeCheck {
    template <typename T>
    constexpr bool hasGet = requires {
        T::get();
    };

    template <typename T>
    std::string typeName() {
        std::string name = typeid(T).name();

#if defined(__GNUG__) || defined(__clang__)
        int status;
        char *demangledName = abi::__cxa_demangle(name.c_str(), nullptr, nullptr, &status);
        if (!status) {
            name = demangledName;
            std::free(demangledName);
        }
#endif

        return name;
    }

    /**
     * @brief Describes whether the type T needs to be boxed or not (for instance method invokes).
     * This is true for all non-pointer types by default.
     * @tparam T The type to be boxed or not.
     */
    template <typename T>
    struct GLUON_HIDDEN NeedBox {
        constexpr static bool value = true;
    };

    /**
     * @brief Describes whether the type T needs to be boxed or not (for instance method invokes).
     * This is false for T*s by default.
     * @tparam T The type to be boxed or not.
     */
    template <typename T>
    struct GLUON_HIDDEN NeedBox<T *> {
        constexpr static bool value = false;
    };

    class NestedType { };

    template <typename T>
    struct Il2CppNoArgClass { };

    template <typename T>
    requires hasGet<Il2CppNoArgClass<T>>
    struct GLUON_HIDDEN Il2CppNoArgClass<T *> {
        static Il2CppClass *get() {
            static Il2CppClass *ptr = nullptr;

            if (ptr) {
                return ptr;
            }

            Gluon::Il2CppFunctions::initialise();

            auto *klass = RET_0_UNLESS(Il2CppNoArgClass<T>::get());
            RET_0_UNLESS(Gluon::Il2CppFunctions::class_is_valuetype(klass));

            ptr = Gluon::Il2CppFunctions::Class_GetPtrClass(klass);
            return ptr;
        }
    };

    static Il2CppClass *getGenericTemplateClass(Il2CppGenericClass *genericClass) {
#ifdef UNITY_2021
        return CRASH_UNLESS(Gluon::Il2CppFunctions::type_get_class_or_element_class(genericClass->type));
#else // UNITY_2021
        return CRASH_UNLESS(Gluon::Il2CppFunctions::MetadataCache_GetTypeInfoFromTypeDefinitionIndex(genericClass->type));
#endif // UNITY_2021
    }

    template <typename T>
    requires(std::is_base_of_v<NestedType, T> && T::__IL2CPP_IS_VALUE_TYPE)
    struct GLUON_HIDDEN Il2CppNoArgClass<T> {
        static Il2CppClass *get() {
            Gluon::Il2CppFunctions::initialise();

            Il2CppClass *declaring = Il2CppNoArgClass<typename T::declaringType>::get();
            Il2CppClass *classWithNested = declaring;

            if (declaring->generic_class) {
                classWithNested = getGenericTemplateClass(declaring->generic_class);
            }

            const std::string typeName = Gluon::TypeCheck::typeName<T>();

            void *itr = nullptr;
            Il2CppClass *found = nullptr;
            while (Il2CppClass *nested = Gluon::Il2CppFunctions::class_get_nested_types(classWithNested, &itr)) {
                if (typeName == nested->name) {
                    found = nested;
                    break;
                }
            }

            CRASH_UNLESS(found);

            if (declaring->generic_class) {
                const Il2CppGenericInst *genericInst = declaring->generic_class->context.class_inst;
                found = CRASH_UNLESS(Gluon::Classes::makeGeneric(found, genericInst->type_argv, genericInst->type_argc));
            }

            return found;
        }
    };

    template <typename T>
    requires(std::is_base_of_v<NestedType, T> && !T::__IL2CPP_IS_VALUE_TYPE)
    struct GLUON_HIDDEN Il2CppNoArgClass<T *> {
        static Il2CppClass *get() {
            Gluon::Il2CppFunctions::initialise();

            Il2CppClass *declaring = Il2CppNoArgClass<typename T::declaringType>::get();
            Il2CppClass *classWithNested = declaring;

            if (declaring->generic_class) {
                classWithNested = getGenericTemplateClass(declaring->generic_class);
            }

            std::string typeName = Gluon::TypeCheck::typeName<T>();
            if (const auto index = typeName.find_last_of(':'); index >= 0) {
                typeName = typeName.substr(index + 1);
            }

            void *itr = nullptr;
            Il2CppClass *found = nullptr;
            while (Il2CppClass *nested = Gluon::Il2CppFunctions::class_get_nested_types(classWithNested, &itr)) {
                if (typeName == nested->name) {
                    found = nested;
                    break;
                }
            }

            CRASH_UNLESS(found);

            if (declaring->generic_class) {
                const Il2CppGenericInst *genericInst = declaring->generic_class->context.class_inst;
                found = CRASH_UNLESS(Gluon::Classes::makeGeneric(found, genericInst->type_argv, genericInst->type_argc));
            }

            return found;
        }
    };

    template <typename T>
    struct GLUON_HIDDEN Il2CppArgClass {
        static Il2CppClass *get([[maybe_unused]] T arg) {
            if constexpr (hasGet<Il2CppNoArgClass<T>>) {
                return Il2CppNoArgClass<T>::get();
            }
            return nullptr;
        }
    };

    DEFINE_IL2CPP_DEFAULT_TYPE(int8_t, sbyte);
    DEFINE_IL2CPP_DEFAULT_TYPE(uint8_t, byte);
    DEFINE_IL2CPP_DEFAULT_TYPE(int16_t, int16);  // "short"
    DEFINE_IL2CPP_DEFAULT_TYPE(uint16_t, uint16);  // "ushort"
    DEFINE_IL2CPP_DEFAULT_TYPE(int32_t, int32);  // "int"
    DEFINE_IL2CPP_DEFAULT_TYPE(uint32_t, uint32);  // "uint"
    DEFINE_IL2CPP_DEFAULT_TYPE(int64_t, int64);  // "long"
    DEFINE_IL2CPP_DEFAULT_TYPE(uint64_t, uint64);  // "ulong"

    DEFINE_IL2CPP_DEFAULT_TYPE(float, single);
    DEFINE_IL2CPP_DEFAULT_TYPE(double, double);

    DEFINE_IL2CPP_DEFAULT_TYPE(bool, boolean);
    DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppChar, char);

    DEFINE_IL2CPP_DEFAULT_TYPE(void, void);
    DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppObject*, object);
    NEED_NO_BOX(Il2CppObject);
    DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppString*, string);
    NEED_NO_BOX(Il2CppString);
    DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppArray*, array);
    NEED_NO_BOX(Il2CppArray);
    DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppReflectionType*, systemtype);
    NEED_NO_BOX(Il2CppReflectionType);

    NEED_NO_BOX(Il2CppClass);

    template <>
    struct GLUON_HIDDEN Il2CppArgClass<Il2CppClass *> {
        static Il2CppClass *get(Il2CppClass *arg) {
            return arg;
        }
    };

    template <>
    struct GLUON_HIDDEN Il2CppArgClass<Il2CppType *> {
        static Il2CppClass *get(Il2CppType *arg) {
            RET_0_UNLESS(arg);
            Gluon::Il2CppFunctions::initialise();
            return Gluon::Il2CppFunctions::class_from_il2cpp_type(arg);
        }
    };

    template <typename T>
    struct GLUON_HIDDEN Il2CppArgClass<T *> {
        static Il2CppClass *get(T *arg) {
            if constexpr (hasGet<Il2CppNoArgClass<T>>) {
                Il2CppClass *elementClass = Il2CppNoArgClass<T>::get();
                Gluon::Il2CppFunctions::initialise();
                return Gluon::Il2CppFunctions::Class_GetPtrClass(elementClass);
            }

            if (arg) {
                Gluon::Il2CppFunctions::initialise();

                if (Il2CppClass *klass = Gluon::Il2CppFunctions::object_get_class(reinterpret_cast<Il2CppObject*>(arg));
                    klass && klass->klass == klass) {
                    return klass;
                    }
            }

            if constexpr (hasGet<Il2CppNoArgClass<T *>>) {
                return Il2CppNoArgClass<T *>::get();
            }

            return nullptr;
        }

    };

    template <template <typename ...ST> class S>
    struct Il2CppGenStructNoArgClass;

    template <template <typename ...ST> class S>
    struct Il2CppGenClassNoArgClass;

    template <template <typename ...ST> class S>
    struct NeedBoxGen {
        constexpr static bool value = true;
    };

    template <typename ...TArgs, template <typename ...ST> class S>
    requires hasGet<Il2CppGenStructNoArgClass<S>>
    struct GLUON_HIDDEN Il2CppNoArgClass<S<TArgs...>> {
        static Il2CppClass *get() {
            static Il2CppClass *genericClass = nullptr;

            if (genericClass) {
                return genericClass;
            }

            Il2CppClass *klass = Il2CppGenStructNoArgClass<S>::get();
            std::array<const Il2CppClass*, sizeof...(TArgs)> const types{ Il2CppNoArgClass<TArgs>::get()... };
            genericClass = Classes::makeGeneric(klass, types);

            return genericClass;
        }
    };

    template <typename ...TArgs, template <typename ...ST> class S>
    struct GLUON_HIDDEN NeedBox<S<TArgs...>> {
        constexpr static bool value = NeedBoxGen<S>::value;
    };

    template <typename ...TArgs, template <typename ...ST> class S>
    struct GLUON_HIDDEN NeedBox<S<TArgs...> *> {
        constexpr static bool value = false;
    };

    template <typename ...TArgs, template <typename ...ST> class S>
    requires (S<TArgs...>::__IL2CPP_IS_VALUE_TYPE && hasGet<Il2CppNoArgClass<typename S<TArgs...>::declaringType>>)
    struct GLUON_HIDDEN Il2CppNoArgClass<S<TArgs...>> {
        static Il2CppClass *get() {
            static Il2CppClass *found = nullptr;

            if (found) {
                return found;
            }

            // Resolve declaring type
            Il2CppClass *declaring = Il2CppNoArgClass<typename S<TArgs...>::declaringType>::get();
            Il2CppClass *classWithNested = declaring;

            if (declaring->generic_class) {
                // Class::GetNestedTypes doesn't work on generic instances, so get the generic template instead
                classWithNested = getGenericTemplateClass(declaring->generic_class);
            }

            const std::string typeName(S<TArgs...>::NESTED_NAME);

            void *itr = nullptr;
            while (Il2CppClass *nested = Gluon::Il2CppFunctions::class_get_nested_types(classWithNested, &itr)) {
                if (typeName == nested->name) {
                    found = nested;
                    break;
                }
            }

            CRASH_UNLESS(found);
            if (declaring->generic_class) {
                const Il2CppGenericInst *genericInst = declaring->generic_class->context.class_inst;
                found = CRASH_UNLESS(Gluon::Classes::makeGeneric(found, genericInst->type_argv, genericInst->type_argc));
            }

            return found;
        }
    };

    template <typename ...TArgs, template <typename ...ST> class S>
    requires (!S<TArgs...>::__IL2CPP_IS_VALUE_TYPE && hasGet<Il2CppNoArgClass<typename S<TArgs...>::declaringType>>)
    struct GLUON_HIDDEN Il2CppNoArgClass<S<TArgs...> *> {
        static Il2CppClass *get() {
            static Il2CppClass *found = nullptr;

            if (found) {
                return found;
            }

            // Resolve declaring type
            Il2CppClass *declaring = Il2CppNoArgClass<typename S<TArgs...>::declaringType>::get();
            Il2CppClass *classWithNested = declaring;

            if (declaring->generic_class) {
                // Class::GetNestedTypes doesn't work on generic instances, so get the generic template instead
                classWithNested = getGenericTemplateClass(declaring->generic_class);
            }

            std::string typeName(S<TArgs...>::NESTED_NAME);

            void *itr = nullptr;
            while (Il2CppClass *nested = Gluon::Il2CppFunctions::class_get_nested_types(classWithNested, &itr)) {
                if (typeName == nested->name) {
                    found = nested;
                    break;
                }
            }

            CRASH_UNLESS(found);
            if (declaring->generic_class) {
                const Il2CppGenericInst *genericInst = declaring->generic_class->context.class_inst;
                found = CRASH_UNLESS(Gluon::Classes::makeGeneric(found, genericInst->type_argv, genericInst->type_argc));
            }

            return found;
        }
    };

    template <typename ...TArgs, template <typename ...ST> class S>
    struct GLUON_HIDDEN Il2CppNoArgClass<S<TArgs...> *> {
        static Il2CppClass *get() {
            static Il2CppClass *genericInst = nullptr;

            if (genericInst) {
                return genericInst;
            }

            Il2CppClass *genericTemplate;
            bool isStruct = false;

            if constexpr (hasGet<Il2CppGenClassNoArgClass<S>>) {
                genericTemplate = Il2CppGenClassNoArgClass<S>::get();
            }
            else if constexpr (hasGet<Il2CppGenStructNoArgClass<S>>) {
                genericTemplate = Il2CppGenStructNoArgClass<S>::get();
                isStruct = true;
            }
            else {
                static_assert(false_t<S<TArgs...>>);
            }

            auto genericArgs = std::array<const Il2CppClass*, sizeof...(TArgs)>({ Il2CppNoArgClass<TArgs>::get()... });
            genericInst = Gluon::Classes::makeGeneric(genericTemplate, std::span(genericArgs));

            if (isStruct) {
                Gluon::Il2CppFunctions::initialise();
                genericInst = Gluon::Il2CppFunctions::Class_GetPtrClass(genericInst);
            }

            return genericInst;
        }
    };

    // TODO: arg type stuff
    template <typename T>
    struct GLUON_HIDDEN Il2CppNoArgType {
        static const Il2CppType *get() {
            const Il2CppClass *klass = Il2CppNoArgClass<T>::get();
            if (klass) {
                return &klass->byval_arg;
            }
            return nullptr;
        }
    };

    template <typename T>
    struct GLUON_HIDDEN Il2CppNoArgType<T *> {
        static const Il2CppType *get() {
            const Il2CppClass *klass = Il2CppNoArgClass<T *>::get();
            if (klass) {
                return &klass->byval_arg;
            }
            return nullptr;
        }
    };

    template <typename T>
    struct GLUON_HIDDEN Il2CppNoArgType<T &&> {
        static const Il2CppType *get() {
            const Il2CppClass *klass = Il2CppNoArgClass<T>::get();
            if (klass) {
                return &klass->byval_arg;
            }
            return nullptr;
        }
    };

    template <typename T>
    struct GLUON_HIDDEN Il2CppNoArgType<T &> {
        static const Il2CppType *get() {
            const Il2CppClass *klass = Il2CppNoArgClass<T>::get();
            if (klass) {
                return &klass->byval_arg;
            }
            return nullptr;
        }
    };

    template <typename T>
    struct GLUON_HIDDEN Il2CppNoArgType<T const &> {
        static const Il2CppType *get() {
            return Il2CppNoArgClass<T>::get();
        }
    };

    template <typename T>
    struct GLUON_HIDDEN Il2CppArgType {};

    template <typename T>
    struct GLUON_HIDDEN Il2CppArgType<T &> {
        static const Il2CppType *get(T &arg) {
            // Pure reference type is not the same as ByRef<T>. Thus, use the byval version.
            // Therefore, the only way to get the byref type match for any expression is to use a ByRef.
            const Il2CppClass *klass = Il2CppArgClass<T>::get(arg);
            return &klass->byval_arg;
        }
    };

    template <typename T>
    struct GLUON_HIDDEN Il2CppArgType<T *> {
        static const Il2CppType *get(T *arg) {
            // A pointer could be passed in explicitly. In such a case, get the class of the pointer and return it non-byref.
            const Il2CppClass *klass = Il2CppArgClass<T *>::get(arg);
            return &klass->byval_arg;
        }
    };

    template <typename T>
    struct GLUON_HIDDEN Il2CppArgType<const T &> {
        static const Il2CppType *get(const T& arg) {
            // A method cannot store a result back to a const ref. It is not a C# ref.
            const Il2CppClass *klass = Il2CppArgClass<T>::get(arg);
            return &klass->byval_arg;
        }
    };

    template <typename T>
    struct GLUON_HIDDEN Il2CppArgType<T &&> {
        static const Il2CppType *get(T &&arg) {
            const Il2CppClass *klass = Il2CppArgClass<T>::get(arg);
            return &klass->byval_arg;
        }
    };
}

#endif // GLUON_TYPE_CHECK_HPP_