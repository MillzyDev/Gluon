#ifndef GLUON_SIZE_CONCEPTS_HPP_
#define GLUON_SIZE_CONCEPTS_HPP_

#include <cstddef>

#include "type_concepts.hpp"

#define IL2CPP_SIZEOF(...) (Gluon::Il2CppSize<__VA_ARGS__>::value)
#define IL2CPP_SAFE(...) (Gluon::is_il2cpp_size_safe_v<__VA_ARGS__>)
#define IL2CPP_INSTANCE_SIZEOF(...) (Gluon::Il2CppInstanceSize<__VA_ARGS__>::value)

namespace Gluon {
    template <typename T>
    struct Il2CppSize {
        static constexpr std::size_t value = sizeof(T);
    };

    template <Gluon::TypeConcepts::Il2CppValueType T>
    struct Il2CppSize<T> {
        static constexpr std::size_t value = T::kValueTypeSize;
    };

    template <Gluon::TypeConcepts::Il2CppReferenceType T>
    struct Il2CppSize<T> {
        static constexpr std::size_t value = sizeof(void *);
    };

    template <typename T>
    static constexpr std::size_t il2cpp_size_v = Il2CppSize<T>::value;

    template <typename T>
    struct Il2CppInstanceSize;

    template <Gluon::TypeConcepts::Il2CppReferenceTypeWrapper T>
    struct Il2CppInstanceSize<T> {
        static constexpr std::size_t value = T::__IL2CPP_REFERENCE_TYPE_SIZE;
    };

    template <Gluon::TypeConcepts::Il2CppReferenceTypePointer T>
    struct Il2CppInstanceSize<T> {
        static constexpr std::size_t value = std::remove_pointer_t<T>::__Il2CPP_REFERENCE_TYPE_SIZE;
    };

    template <Gluon::TypeConcepts::Il2CppValueType T>
    struct Il2CppInstanceSize<T> {
        static constexpr std::size_t value = T::kValueTypeSize;
    };

    template <typename T>
    struct IsIl2CppSizeSafe {
        static constexpr bool value = il2cpp_size_v<T> == sizeof(T);
    };

    template <typename T>
    static constexpr bool is_il2cpp_size_safe_v = IsIl2CppSizeSafe<T>::Value;
} // Gluon

#endif // GLUON_SIZE_CONCEPTS_HPP_