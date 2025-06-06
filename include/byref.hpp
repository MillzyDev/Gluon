#ifndef GLUON_BYREF_HPP_
#define GLUON_BYREF_HPP_

#include "type_check.hpp"
#include "type_concepts.hpp"

#define BYREF(...) (Gluon::ByRef(__VA_ARGS__))

namespace Gluon {
    template <class T>
    struct ByRef {
        constexpr ByRef(T &value) noexcept : heldReference(value) {}

        explicit constexpr ByRef(void *) noexcept : heldReference(*reinterpret_cast<T*>(value)) {}

        T &heldReference;

        constexpr T *operator->() noexcept {
            return &heldReference;
        }

        constexpr T &operator*() noexcept {
            return heldReference;
        }

        constexpr T const &operator*() const noexcept {
            return heldReference;
        }

        constexpr const T *operator->() const noexcept {
            return &heldReference;
        }

        Gluon::ByRef<T> &operator=(T const &&other) {
            heldReference = other;
            return *this;
        }

        [[nodiscard]] constexpr void *convert() const noexcept {
            return static_cast<void *>(&heldReference);
        }

        static_assert(sizeof(T *) == sizeof(void *));
    };

    // this feels really out of place, but ive got no idea where i should put it
    template <typename T>
    concept HasNoArgClass = requires {
        { Gluon::TypeCheck::Il2CppNoArgClass<T>::get() } -> Gluon::TypeConcepts::ConvertibleTo<Il2CppClass *>;
    };
} // Gluon

MARK_GEN_REF_T(Gluon::ByRef);

template <typename T>
struct Gluon::TypeCheck::Il2CppNoArgType<Gluon::ByRef<T>> {
    static const Il2CppType *get() {
        if constexpr (Gluon::HasNoArgClass<T>) {
            static const Il2CppType *type = &Gluon::TypeCheck::Il2CppNoArgClass<T>::get()->this_arg;
            return type;
        }
        else {
            return Gluon::TypeCheck::Il2CppNoArgType<T>::get();
        }
    }
};

template <typename T>
struct Gluon::TypeCheck::Il2CppArgType<Gluon::ByRef<T>> {
    static const Il2CppType *get([[maybe_unused]] ByRef<T> arg) {
        return Gluon::TypeCheck::Il2CppNoArgType<ByRef<T>>::get();
    }
};

#endif // GLUON_BYREF_HPP_