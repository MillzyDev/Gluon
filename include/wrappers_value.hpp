#ifndef GLUON_WRAPPERS_VALUE_HPP_
#define GLUON_WRAPPERS_VALUE_HPP_

#include <array>
#include <cstddef>

#include "il2cpp_functions.hpp"
#include "type_check.hpp"
#include "type_concepts.hpp"

namespace Gluon {
    template <std::size_t size>
    struct ValueTypeWrapper {
        static constexpr std::size_t kValueTypeSize = size;

        explicit constexpr ValueTypeWrapper(std::array<std::byte, kValueTypeSize> i) noexcept : instance(std::move(i)) {}

        [[nodiscard]] void *convert() const noexcept {
            return const_cast<void *>(static_cast<const void *>(instance.data()));
        }

        constexpr ValueTypeWrapper() = default;

        ~ValueTypeWrapper() = default;

        constexpr ValueTypeWrapper(ValueTypeWrapper &&) = default;

        constexpr ValueTypeWrapper &operator=(ValueTypeWrapper &&value) noexcept {
            instance = std::move(value.instance);
            return *this;
        }

        constexpr ValueTypeWrapper(ValueTypeWrapper const &) = default;

        constexpr ValueTypeWrapper &operator=(ValueTypeWrapper const &value) {
            instance = value.instance;
            return *this;
        }

        std::array<std::byte, size> instance;
    };

    struct ValueTypePtr {
        template <std::size_t size>
        explicit ValueTypePtr(Gluon::ValueTypeWrapper<size> &value) : instance(&value) {}

        explicit ValueTypePtr(void *other) : instance(&other) {};

        [[nodiscard]] void *convert() const {
            return instance;
        }

        void *instance;
    };

#pragma pack(push, valueType, 0)
    struct ValueType {};
#pragma pack(pop, valueType)
} // Gluon

template <>
struct Gluon::TypeCheck::Il2CppNoArgClass<Gluon::ValueTypePtr> {
    static inline Il2CppClass *get() {
        Il2CppClass *enumClass = Gluon::Classes::getClassFromName("System", "ValueType");
        static Il2CppClass *classPtr = Gluon::Il2CppFunctions::Class_GetPtrClass(enumClass);
        return classPtr;
    }
};

template <std::size_t size>
struct Gluon::TypeConcepts::ValueTypeTrait<Gluon::ValueTypeWrapper<size>> {
    static constexpr bool value = true;
};
#endif // GLUON_WRAPPERS_VALUE_HPP_