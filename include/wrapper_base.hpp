#ifndef GLUON_WRAPPER_BASE_HPP_
#define GLUON_WRAPPER_BASE_HPP_

#include <concepts>
#include <cstdint>
#include <type_traits>

#include "type_check.hpp"
#include "type_concepts.hpp"

namespace Gluon {
    struct Il2CppWrapperType {
        constexpr explicit Il2CppWrapperType(void* i) noexcept : instance(i) {}
        constexpr Il2CppWrapperType(Il2CppWrapperType const& other) = default;
        constexpr Il2CppWrapperType(Il2CppWrapperType && other) = default;
        constexpr Il2CppWrapperType& operator=(Il2CppWrapperType const& other) = default;
        constexpr Il2CppWrapperType& operator=(Il2CppWrapperType && other) = default;

        [[nodiscard]] constexpr void* convert() const noexcept {
            return instance;
        }

        Il2CppObject* operator ->() const noexcept { return const_cast<Il2CppObject*>(static_cast<Il2CppObject const*>(instance)); }
        explicit operator Il2CppObject*() const noexcept { return const_cast<Il2CppObject*>(static_cast<Il2CppObject const*>(instance)); }

    protected:
        void* instance;
    };
    static_assert(Gluon::TypeConcepts::HasIl2CppConversion<Il2CppWrapperType>);
}

NEED_NO_BOX(Gluon::Il2CppWrapperType)
DEFINE_IL2CPP_DEFAULT_TYPE(Gluon::Il2CppWrapperType, object);

#endif // GLUON_WRAPPER_BASE_HPP_