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

namespace Gluon::Classes {
    extern GLUON_API Il2CppClass *makeGeneric(const Il2CppClass *klass, const Il2CppType **types, std::uint32_t count);
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
}

#endif // GLUON_TYPE_CHECK_HPP_