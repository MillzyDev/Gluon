#ifndef GLUON_WRAPPERS_DISPOSAL_HPP_
#define GLUON_WRAPPERS_DISPOSAL_HPP_

#include <type_traits>

#include "methods.hpp"

namespace Gluon {
    template<class T>
    concept has_dispose_vt = requires (T t) {
        t.Dispose();
    };

    template<class T>
    concept has_dispose_rt = requires (T t) {
        t->Dispose();
    };

    /// @brief Holds an instance that can be disposed.
    /// If holding a value type that can be copied, is copyable.
    /// If holding a pointer, cannot be copied.
    /// Note that the pointer should have a lifetime that is AT LEAST the lifetime of the Disposable instance.
    template<class T, bool = !std::is_pointer_v<T> && std::is_copy_constructible_v<T>>
    requires (!std::is_reference_v<T> && (has_dispose_vt<T> || has_dispose_rt<T> || std::is_convertible_v<T, Il2CppObject*>))
    struct Disposable {
        explicit Disposable(T t) : value(t) {}
        explicit operator T() {return value;}
        explicit operator T&() {return value;}
        T operator ->() {return value;}
        ~Disposable() {
            if constexpr (has_dispose_vt<T>) {
                value.Dispose();
            } else if constexpr (has_dispose_rt<T>) {
                value->Dispose();
            } else {
                if (!Gluon::Methods::runMethod<Il2CppObject*, false>(value, "Dispose")) {
                    SAFE_ABORT();
                }
            }
        }
    private:
        T value;
    };

    template<class T>
    requires (!std::is_reference_v<T> && (has_dispose_rt<T> || std::is_convertible_v<T, Il2CppObject*>))
    struct Disposable<T, false> : Disposable<T, true> {
        using Disposable<T, true>::Disposable;
        Disposable(Disposable const&) = delete;
        Disposable(Disposable&&) = default;
        Disposable& operator=(Disposable&&) = default;
        Disposable& operator=(Disposable const&) = default;
    };
}

#endif // GLUON_WRAPPERS_DISPOSAL_HPP_