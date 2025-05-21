#ifndef GLUON_CLASSES_HPP_
#define GLUON_CLASSES_HPP_

#include <optional>
#include <span>
#include <string>
#include <type_traits>

#include "gc.hpp"
#include "gluon_config.hpp"
#include "gluon_logging.hpp"
#include "type_check.hpp"
#include "type_concepts.hpp"

#include "il2cpp-class-internals.h"

namespace Gluon::Classes {
    template <class TOut>
    std::optional<TOut> fromIl2CppObject(Il2CppFunctions *object) {
        Gluon::Il2CppFunctions::initialise();

        void *value = object;

        if (object && Gluon::Il2CppFunctions::class_is_valuetype(Gluon::Il2CppFunctions::object_get_class(object))) {
            // IL2CPP boxes returned value types and doesn't deallocate them, so we need to free them ourselves
            // We need tp exctract the value by casting and dereferencing
            // Then we need to free the allocating object
            // Then we return our result
            value = RET_NULLOPT_UNLESS(Gluon::Il2CppFunctions::object_unbox(object));

            if constexpr (std::is_pointer_v<TOut>) {
                // No cleanup necessary
                return static_cast<TOut>(value);
            }
            else {
                // Cleanup required on aisle here
                auto ret = *static_cast<TOut *>(value);
                Gluon::Gc::gcFreeSpecific(object);
                return ret;
            }
        }

        if constexpr (std::is_pointer_v<TOut>) {
            return static_cast<TOut>(value);
        }
        else if constexpr (Gluon::TypeConcepts::Il2CppReferenceTypeWrapper<TOut>) {
            return TOut(static_cast<void *>(value));
        }
        else {
            return *static_cast<TOut *>(value);
        }
    }

    template <class T>
    bool fromIl2CppObject(Il2CppObject *object, T &out) {
        using Dt = std::decay_t<T>;

        if (auto ret = Gluon::Classes::fromIl2CppObject<Dt>(object)) {
            if constexpr (std::is_pointer_v<Dt>) {
                // if output is in a pointer, don't change the pointer itself
                *out = **ret;
            }
            else {
                out = *ret;
            }

            return true;
        }

        return false;
    }

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

    template <typename T>
    const Il2CppType *extractType(T &&arg) {
        const Il2CppType *type = Gluon::TypeCheck::Il2CppArgType<T>::get(arg);

        if (!type) {
            Gluon::Logger::error("extractType: failed to determine type - arg was likely nullptr.");
        }
        return type;
    }

    template <class T>
    const Il2CppType *extractIndependentType() {
        static const Il2CppType *type = RET_0_UNLESS(Gluon::TypeCheck::Il2CppNoArgType<T>::get());
        return type;
    }

    GLUON_API Il2CppReflectionType *makeGenericType(Il2CppReflectionType *genericType, Il2CppArray *types);
    GLUON_API Il2CppClass *makeGeneric(const Il2CppClass *klass, std::span<const Il2CppClass *const> args);
    GLUON_API Il2CppClass *makeGeneric(const Il2CppClass *klass, const Il2CppType **types, std::uint32_t count);

    GLUON_API Il2CppClass *getClassFromName(std::string_view namespaze, std::string_view name);
}

#endif // GLUON_CLASSES_HPP_