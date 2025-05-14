#ifndef GLUON_TYPE_CONCEPTS_HPP_
#define GLUON_TYPE_CONCEPTS_HPP_

#include <concepts>
#include <type_traits>

namespace Gluon::TypeConcepts {
    template <typename T, typename U>
    concept ConvertibleTo = std::is_convertible_v<T, U>;

    /**
     * A concept depicting if a type is a wrapper type.
     */
    template <class T>
    concept HasIl2CppConversion = std::is_constructible_v<T, void *> && requires (T type) {
        { type.convert() } -> ConvertibleTo<void *>;
    };
}

#endif