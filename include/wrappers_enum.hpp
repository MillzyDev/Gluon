#ifndef GLUON_WRAPPERS_ENUM_HPP_
#define GLUON_WRAPPERS_ENUM_HPP_

#include <cstddef>

#include "classes.hpp"
#include "il2cpp_functions.hpp"
#include "type_check.hpp"
#include "type_concepts.hpp"
#include "wrappers_value.hpp"

#include "il2cpp-class-internals.h"

namespace Gluon {
    template <std::size_t size>
    requires (size == 0x0 || size == 0x1 || size == 0x2 || size == 0x4 || size == 0x8)
    struct EnumTypeWrapper : public ValueTypeWrapper<size> {
        static constexpr std::size_t kValueTypeSize = ValueTypeWrapper<size>::kValueTypeSize;

        using ValueTypeWrapper<size>::ValueTypeWrapper;

        constexpr EnumTypeWrapper() = default;
        ~EnumTypeWrapper() = default;

        constexpr EnumTypeWrapper(EnumTypeWrapper &&) = default;
        constexpr EnumTypeWrapper(const EnumTypeWrapper &) = default;

        constexpr EnumTypeWrapper &operator=(EnumTypeWrapper &&) = default;
        constexpr EnumTypeWrapper &operator=(const EnumTypeWrapper &) = default;
    };

    struct EnumPtr : public ValueTypePtr {
        template <std::size_t size>
        explicit EnumPtr(EnumTypeWrapper<size> &enumType) : ValueTypePtr(enumType) {}

        explicit EnumPtr(void *i) : ValueTypePtr(i) {}
    };

#pragma pack(push, enumType, 0)
    struct EnumType {};
#pragma pack(pop, enumType)
} // Gluon

template <>
struct Gluon::TypeCheck::Il2CppNoArgClass<Gluon::EnumPtr> {
    static Il2CppClass *get() {
        Il2CppClass *enumClass = Gluon::Classes::getClassFromName("System", "Enum");
        static Il2CppClass *ptrClass = Gluon::Il2CppFunctions::Class_GetPtrClass(enumClass);
        return ptrClass;
    }
};

template <std::size_t size>
struct Gluon::TypeConcepts::ValueTypeTrait<Gluon::EnumTypeWrapper<size>> {
    static constexpr bool value = true;
};

#endif // GLUON_WRAPPERS_ENUM_HPP_