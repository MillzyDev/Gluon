#ifndef GLUON_BOXING_HPP_
#define GLUON_BOXING_HPP_

#include <type_traits>

#include "il2cpp_functions.hpp"
#include "type_check.hpp"
#include "type_concepts.hpp"

#include "il2cpp-object-internals.h"

namespace Gluon::Boxing {
    template <typename T>
    Il2CppObject *box(T);

    template <typename T>
    Il2CppObject *box(T *);

    template <>
    constexpr Il2CppObject *box<Il2CppObject *>(Il2CppObject *value) {
        return value;
    }

    template <typename T>
    requires (!std::is_pointer_v<T> && !std::is_base_of_v<Il2CppObject, T>)
    Il2CppObject *box(T *value) {
        return Gluon::Il2CppFunctions::value_box(CLASS_OF(T), value);
    }

    template <Gluon::TypeConcepts::HasIl2CppConversion T>
    requires (!std::is_base_of_v<Il2CppObject, T>)
    Il2CppObject *box(T value) {
        return Gluon::Il2CppFunctions::value_box(CLASS_OF(T), value.convert());
    }

    template <Gluon::TypeConcepts::HasIl2CppConversion T>
    requires (!std::is_base_of_v<Il2CppObject, T>)
    Il2CppObject *box(T *value) {
        return Gluon::Il2CppFunctions::value_box(CLASS_OF(T), value->convert());
    }

    template <typename T>
    T unbox(Il2CppObject *object) {
        return *static_cast<T *>(Gluon::Il2CppFunctions::object_unbox(object));
    }

    template <Gluon::TypeConcepts::Il2CppReferenceTypeWrapper T>
    T unbox(Il2CppObject *t) {
        return T(t);
    }

    template <Gluon::TypeConcepts::Il2CppReferenceTypePointer T>
    T unbox(Il2CppObject *value) {
        return reinterpret_cast<T>(value);
    }
}

#endif // GLUON_BOXING_HPP_