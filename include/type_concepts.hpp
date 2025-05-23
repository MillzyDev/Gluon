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

    template <typename T>
    concept HasValueMarker = requires {
        { T::__IL2CPP_IS_VALUE_TYPE } -> ConvertibleTo<bool>;
    };

    template <typename T, bool check>
    struct ValueMarkerCheck {
        static constexpr bool value = false;
    };

    template <HasValueMarker T, bool check>
    constexpr bool value_marker_check_v = ValueMarkerCheck<T, check>::value;

    template <typename T>
    concept Il2CppReferenceTypeWrapperRequirements = requires(const T &type) {
        { type.convert() } -> ConvertibleTo<void *>;

        // ensure these constructors exist
        requires std::is_constructible_v<T, void *>;
        requires std::is_constructible_v<T, std::nullptr_t>;

        requires value_marker_check_v<T, false>;
    };

    /**
     * @brief Mark T explicitly as a reference type, default is false.
     */
    template <class T>
    struct RefTypeTrait;

    /**
     * @brief Anything that matches the type requirements should also be allowed to be a reference type.
     */
    template <Il2CppReferenceTypeWrapperRequirements T>
    struct RefTypeTrait<T> {
        static constexpr bool value = true;
    };

    /**
     * @brief Mark a generic T explicitly as reference type without requiring its TArgs to be fully realized, defaults to false.
     */
    template <template <class...> class T>
    struct GenRefTypeTrait;

    /**
     * @brief Non-generics should return false
     */
    template <class T>
    struct RefDecompose {
        static constexpr bool value = false;
    };

    /**
     * @brief Generics should fall back to the generic type trait
     */
    template <template <class...> class T, class ...TArgs>
    struct RefDecompose<T<TArgs...>> {
        static constexpr bool value = GenRefTypeTrait<T>::value;
    };

    /**
     * @brief Concept that lets us know whether what we are dealing with is a reference type or not
     */
    template <class T>
    concept Il2CppReferenceTypeWrapper = RefDecompose<T>::value || RefTypeTrait<T>::value;

    /**
     * @brief mark a T* explicitly as reference type, default is false
     */
    template <class T>
    struct RefPtrTypeTrait;

    /**
     * @brief if reference type is said to be true we can say this is true
     */
    template <class T>
    requires (value_marker_check_v<T, false>)
    struct RefPtrTypeTrait<T> {
        static constexpr bool value = true;
    };

    /**
     * @brief mark a generic T explicitly as reference type without requiring its TArgs to be fully realized, defaults to false
     */
    template <template <class...> class T>
    struct GenRefPtrTypeTrait;

    /**
     * @brief non generics should return false
     */
    template <class T>
    struct RefPtrDecompose {
        static constexpr bool value = false;
    };

    /**
     * @brief generics should fall back to the generic type trait
     */
    template <template <class...> class T, class ...TArgs>
    struct RefPtrDecompose<T<TArgs...>> {
        static constexpr bool value = GenRefPtrTypeTrait<T>::value;
    };

    template <class T>
    concept Il2CppReferenceTypePointer = std::is_pointer_v<T> && (RefPtrDecompose<std::remove_pointer_t<T>>::value || RefPtrTypeTrait<std::remove_pointer_t<T>>::value);


}

#endif