#ifndef GLUON_TYPE_CONCEPTS_HPP_
#define GLUON_TYPE_CONCEPTS_HPP_

#include <concepts>
#include <type_traits>

#define MARK_REF_T(...)                                                                                                          \
    template<> struct GLUON_HIDDEN Gluon::TypeConcepts::RefTypeTrait<__VA_ARGS__> { static constexpr bool value = true; };       \
    template<> struct GLUON_HIDDEN Gluon::TypeConcepts::ValueTypeTrait<__VA_ARGS__> { static constexpr bool value = false; };    \
    template<> struct GLUON_HIDDEN Gluon::TypeConcepts::RefPtrTypeTrait<__VA_ARGS__> { static constexpr bool value = false; }

#define MARK_REF_PTR_T(...)                                                                                                      \
    template<> struct GLUON_HIDDEN Gluon::TypeConcepts::RefTypeTrait<__VA_ARGS__> { static constexpr bool value = false; };      \
    template<> struct GLUON_HIDDEN Gluon::TypeConcepts::ValueTypeTrait<__VA_ARGS__> { static constexpr bool value = false; };    \
    template<> struct GLUON_HIDDEN Gluon::TypeConcepts::RefPtrTypeTrait<__VA_ARGS__> { static constexpr bool value = true; }

#define MARK_VAL_T(...)                                                                                                          \
    template<> struct GLUON_HIDDEN Gluon::TypeConcepts::RefTypeTrait<__VA_ARGS__> { static constexpr bool value = false; };      \
    template<> struct GLUON_HIDDEN Gluon::TypeConcepts::ValueTypeTrait<__VA_ARGS__> { static constexpr bool value = true; };     \
    template<> struct GLUON_HIDDEN Gluon::TypeConcepts::RefPtrTypeTrait<__VA_ARGS__> { static constexpr bool value = false; }

#define MARK_GEN_REF_T(...)                                                                                                      \
    template<> struct GLUON_HIDDEN Gluon::TypeConcepts::GenRefTypeTrait<__VA_ARGS__> { static constexpr bool value = true; };    \
    template<> struct GLUON_HIDDEN Gluon::TypeConcepts::GenValueTypeTrait<__VA_ARGS__> { static constexpr bool value = false; }; \
    template<> struct GLUON_HIDDEN Gluon::TypeConcepts::GenRefPtrTypeTrait<__VA_ARGS__> { static constexpr bool value = false; }

#define MARK_GEN_REF_PTR_T(...)                                                                                                   \
    template<> struct GLUON_HIDDEN Gluon::TypeConcepts::GenRefTypeTrait<__VA_ARGS__> { static constexpr bool value = false; };     \
    template<> struct GLUON_HIDDEN Gluon::TypeConcepts::GenValueTypeTrait<__VA_ARGS__> { static constexpr bool value = false; };  \
    template<> struct GLUON_HIDDEN Gluon::TypeConcepts::GenRefPtrTypeTrait<__VA_ARGS__> { static constexpr bool value = true; }

#define MARK_GEN_VAL_T(...)                                                                                                          \
    template<> struct GLUON_HIDDEN Gluon::TypeConcepts::GenRefTypeTrait<__VA_ARGS__> { static constexpr bool value = true; };       \
    template<> struct GLUON_HIDDEN Gluon::TypeConcepts::GenValueTypeTrait<__VA_ARGS__> { static constexpr bool value = false; };    \
    template<> struct GLUON_HIDDEN Gluon::TypeConcepts::GenRefPtrTypeTrait<__VA_ARGS__> { static constexpr bool value = false; }

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
    concept Il2CppValueTypeRequirements = requires (T const &t) {
        requires std::is_same_v<decltype(t.instance), std::array<std::byte, sizeof(t.instance)>>;
        requires std::is_constructible_v<T, decltype(t.instance)>;
        requires Gluon::TypeConcepts::value_marker_check_v<T, true>;
    };

    template <class T>
    struct ValueTypeTrait {
        static constexpr bool value = false;
    };

    template <Il2CppValueTypeRequirements T>
    struct ValueTypeTrait<T> {
        static constexpr bool value = true;
    };

    template <template <class...> class T>
    struct GenValueTypeTrait {
        static constexpr bool value = false;
    };

    template <class T>
    struct ValueDecompose {
        static constexpr bool value = false;
    };

    template <template <class...> class T, class ...TArgs>
    struct ValueDecompose<T<TArgs...>> {
        static constexpr bool value = Gluon::TypeConcepts::GenValueTypeTrait<T>::value;
    };

    template <class T>
    concept Il2CppValueType = Gluon::TypeConcepts::ValueDecompose<T>::value || Gluon::TypeConcepts::ValueTypeTrait<T>::value;

    template <class T, class Enable = void>
    struct GLUON_HIDDEN IsValueType : std::integral_constant<
        bool, (std::is_arithmetic_v<T> || std::is_enum_v<T> || std::is_pointer_v<T> || std::is_standard_layout_v<T>) && !std::is_base_of_v<Il2CppObject, T>
    > {};

    template <class T>
    constexpr bool is_value_type_v = Gluon::TypeConcepts::IsValueType<T>::value;

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