#ifndef GLUON_WRAPPERS_STRING_HPP_
#define GLUON_WRAPPERS_STRING_HPP_

#include <string>

#include "exceptions.hpp"
#include "il2cpp_functions.hpp"
#include "type_check.hpp"
#include "type_concepts.hpp"

#include "il2cpp-object-internals.h"

// if system string exists, we can use it in StringW, but with a compile definition it can be disabled
#if !defined(NO_CODEGEN_WRAPPERS) && __has_include("System/String.hpp")
// forward declare
namespace System {
    class String;
}
#include "System/String.hpp"
#endif

struct Il2CppString;

namespace Gluon {
    namespace Detail {
        void convertString(char const* inp, char16_t* outp, int sz);
        std::size_t convertString(char16_t const* inp, char* outp, int isz, int osz);

        static std::string toString(Il2CppString* str) {
            std::string val(str->length * sizeof(wchar_t) + 1, '\0');
            const auto resSize = Gluon::Detail::convertString(str->chars, val.data(), str->length, val.size());
            val.resize(resSize);
            return val;
        }
        static std::u16string toU16String(Il2CppString* str) {
            return { str->chars, static_cast<std::size_t>(str->length) };
        }
        static std::wstring toWString(Il2CppString* str) {
            return { str->chars, str->chars + str->length };
        }
        static std::u16string_view toU16StringView(Il2CppString* inst) {
            return { inst->chars, inst->chars + inst->length };
        }
        static std::u16string_view toU16StringView(Il2CppString const* inst) {
            return { inst->chars, inst->chars + inst->length };
        }

        Il2CppString* allocateString(std::string_view str);
        Il2CppString* allocateString(std::u16string_view str);

        Il2CppString* stringAppend(Il2CppString const* lhs, Il2CppString const* rhs) noexcept;
        Il2CppString* stringAppend(Il2CppString const* lhs, std::u16string_view const rhs) noexcept;
        Il2CppString* stringAppend(Il2CppString const* lhs, std::string_view const rhs) noexcept;
        Il2CppString* stringAppend(std::string_view const lhs, Il2CppString const* rhs) noexcept;
        Il2CppString* stringAppend(std::u16string_view const lhs, Il2CppString const* rhs) noexcept;

        bool stringCompare(Il2CppString const* lhs, std::string_view const rhs) noexcept;
        bool stringCompare(Il2CppString const* lhs, std::u16string_view const rhs) noexcept;
        bool stringCompare(Il2CppString const* lhs, Il2CppString const* rhs) noexcept;

        bool stringLess(Il2CppString const* lhs, std::string_view const rhs) noexcept;
        bool stringLess(Il2CppString const* lhs, std::u16string_view const rhs) noexcept;
        bool stringLess(Il2CppString const* lhs, Il2CppString const* rhs) noexcept;

        bool stringStart(Il2CppString const* lhs, std::string_view const rhs) noexcept;
        bool stringStart(Il2CppString const* lhs, std::u16string_view const rhs) noexcept;
        bool stringStart(Il2CppString const* lhs, Il2CppString const* rhs) noexcept;

        bool stringEnd(Il2CppString const* lhs, std::string_view const rhs) noexcept;
        bool stringEnd(Il2CppString const* lhs, std::u16string_view const rhs) noexcept;
        bool stringEnd(Il2CppString const* lhs, Il2CppString const* rhs) noexcept;
    } // Detail

    // C# strings can only have 'int' max length.
    template <int sz>
    struct ConstString {
        // Manually allocated string, dtor destructs in place
        ConstString(const char (&st)[sz]) {
            length = sz - 1;
            Gluon::Detail::convertString(st, chars, sz - 1);
        }
        constexpr ConstString(const char16_t (&st)[sz]) noexcept {
            length = sz - 1;
            for (int i = 0; i < sz - 1; i++) {
                chars[i] = st[i];
            }
        }
        // Copies allowed? But should probably be avoided.
        ConstString(ConstString const&) = default;
        // Moves allowed
        ConstString(ConstString&&) = default;

        void init() noexcept {
            klass = Gluon::Il2CppFunctions::il2cppDefaults->string_class;
        }

        constexpr operator Il2CppString*() {
            if (!klass) {
                if (Gluon::Il2CppFunctions::initialised) {
                    klass = Gluon::Il2CppFunctions::il2cppDefaults->string_class;
                } else {
                    throw Gluon::Exceptions::UseBeforeInitError("Il2CppClass* must be initialized before conversion! Call il2cpp_functions::Init before this conversion!");
                }
            }
            return reinterpret_cast<Il2CppString*>(&klass);
        }

        constexpr operator Il2CppString const*() const {
            if (!klass) {
                if (Gluon::Il2CppFunctions::initialised) {
                    // due to klass being initialized being essential to the functionality of this type, we agreed that ignoring the const here is warranted
                    // usually const casting is bad, but due to the reasons stated above we are doing it anyways
                    const_cast<ConstString<sz>*>(this)->klass = Gluon::Il2CppFunctions::il2cppDefaults->string_class;
                } else {
                    throw Gluon::Exceptions::UseBeforeInitError("Il2CppClass* must be initialized before conversion! Call il2cpp_functions::Init before this conversion!");
                }
            }
            return reinterpret_cast<Il2CppString const*>(&klass);
        }

        constexpr Il2CppString* operator->() {
            return operator Il2CppString*();
        }

        operator std::string() {
            std::string val((sz - 1) * 2 + 1, '\0');
            auto resSize = Gluon::Detail::convertString(chars, val.data(), sz - 1, val.size());
            val.resize(resSize);
            return val;
        }
        operator std::u16string() {
            return { chars, chars + length };
        }
        operator std::wstring() {
            return { chars, chars + length };
        }
        constexpr operator std::u16string_view() {
            return { chars, static_cast<std::size_t>(sz) };
        }

        template<typename Ptr>
        friend struct StringWrapper;

    private:
        void* klass = nullptr;
        void* monitor = nullptr;
        int length = 0;
        char16_t chars[sz] = {};
    };

    template <typename Ptr>
    struct StringWrapper {
        // Dynamically allocated string
        template <class T>
            requires(!std::is_convertible_v<T, Il2CppString*> && (std::is_constructible_v<std::u16string_view, T> || std::is_constructible_v<std::string_view, T>))
        StringWrapper(T str) noexcept : inst(Gluon::Detail::allocateString(str)) {}
        constexpr StringWrapper(void* ins) noexcept : inst(static_cast<Il2CppString*>(ins)) {}
        constexpr StringWrapper(Il2CppString* ins) noexcept : inst(ins) {}
        template <int sz>
        constexpr StringWrapper(ConstString<sz>& conststring) noexcept : inst(static_cast<Il2CppString*>(conststring)) {}

        template <typename U>
        constexpr StringWrapper(StringWrapper<U> const& s) noexcept : inst(static_cast<Il2CppString*>(s)) {}
        constexpr StringWrapper(std::nullptr_t npt) noexcept : inst(npt) {}
        constexpr StringWrapper() noexcept : inst(nullptr) {}

        constexpr void* convert() const noexcept {
            return const_cast<void*>(static_cast<void*>(inst));
        }
        constexpr operator Il2CppString const*() const noexcept {
            return inst;
        }
        constexpr operator Il2CppString*() const noexcept {
            return inst;
        }
        constexpr Ptr operator->() noexcept {
            return reinterpret_cast<Ptr>(inst);
        }

        constexpr Ptr operator->() const noexcept {
            return reinterpret_cast<Ptr const>(inst);
        }
        constexpr operator bool() const noexcept {
            return inst != nullptr;
        }

        constexpr bool operator==(std::nullptr_t rhs) const noexcept {
            return inst == rhs;
        }

        template <typename T>
            requires(std::is_constructible_v<std::u16string_view, T> || std::is_constructible_v<std::string_view, T> || std::is_same_v<T, StringWrapper>)
        StringWrapper& operator+=(T const& rhs) noexcept {
            if constexpr (std::is_same_v<T, StringWrapper>)
                inst = Gluon::Detail::stringAppend(inst, rhs.inst);
            else
                inst = Gluon::Detail::stringEnd(inst, rhs);
            return *this;
        }

        template <typename T>
            requires(std::is_constructible_v<std::u16string_view, T> || std::is_constructible_v<std::string_view, T> || std::is_same_v<T, StringWrapper>)
        StringWrapper operator+(T const& rhs) const noexcept {
            if constexpr (std::is_same_v<T, StringWrapper>)
                return Gluon::Detail::stringAppend(inst, rhs.inst);
            else
                return Gluon::Detail::stringEnd(inst, rhs);
        }

        template <typename T>
            requires(std::is_constructible_v<std::u16string_view, T> || std::is_constructible_v<std::string_view, T> || std::is_same_v<T, StringWrapper>)
        bool operator<(T const& rhs) const noexcept {
            if constexpr (std::is_same_v<T, StringWrapper>)
                return Gluon::Detail::stringLess(inst, rhs.inst);
            else
                return Gluon::Detail::stringLess(inst, rhs);
        }

        template <int sz>
        bool operator==(ConstString<sz> const& rhs) const noexcept {
            return Gluon::Detail::stringCompare(inst, rhs.operator Il2CppString const*());
        }

        template <typename T>
        bool operator==(StringWrapper<T> const& rhs) const noexcept {
            return Gluon::Detail::stringCompare(inst, static_cast<Il2CppString const*>(rhs));
        }

        template <typename T>
            requires(std::is_constructible_v<std::u16string_view, T> || std::is_constructible_v<std::string_view, T> || std::is_same_v<T, StringWrapper>)
        bool operator==(T const& rhs) const noexcept {
            if constexpr (std::is_same_v<T, StringWrapper>)
                return Gluon::Detail::stringCompare(inst, rhs.inst);
            else
                return Gluon::Detail::stringCompare(inst, rhs);
        }

        template <typename T>
            requires(std::is_constructible_v<std::u16string_view, T> || std::is_constructible_v<std::string_view, T> || std::is_same_v<T, StringWrapper>)
        bool starts_with(T const& rhs) const noexcept {
            if constexpr (std::is_same_v<T, StringWrapper>)
                return Gluon::Detail::stringStart(inst, rhs.inst);
            else
                return Gluon::Detail::stringStart(inst, rhs);
        }

        template <typename T>
            requires(std::is_constructible_v<std::u16string_view, T> || std::is_constructible_v<std::string_view, T> || std::is_same_v<T, StringWrapper>)
        bool ends_with(T const& rhs) const noexcept {
            if constexpr (std::is_same_v<T, StringWrapper>)
                return Gluon::Detail::stringStart(inst, rhs.inst);
            else
                return Gluon::Detail::stringEnd(inst, rhs);
        }

        using iterator = Il2CppChar*;
        using const_iterator = Il2CppChar const*;

        iterator begin() {
            return inst->chars;
        }
        const_iterator begin() const {
            return inst->chars;
        }
        iterator end() {
            return inst->chars + inst->length;
        }
        const_iterator end() const {
            return inst->chars + inst->length;
        }
        operator std::span<Il2CppChar>() {
            return { begin(), end() };
        }
        operator std::span<Il2CppChar const> const() const {
            return { begin(), end() };
        }

        Il2CppChar const& operator[](size_t const& idx) const {
            return inst->chars[idx];
        }
        Il2CppChar& operator[](size_t const& idx) {
            return inst->chars[idx];
        }
        operator std::string() const {
            return Gluon::Detail::toString(inst);
        }
        operator std::u16string() const {
            return Gluon::Detail::toU16String(inst);
        }
        operator std::wstring() const {
            return Gluon::Detail::toWString(inst);
        }
        operator std::u16string_view() {
            return Gluon::Detail::toU16StringView(inst);
        }
        operator std::u16string_view const() const {
            return Gluon::Detail::toU16StringView(inst);
        }

    private:
        Il2CppString* inst;
    };

    template <typename T, typename Ptr>
    requires(!std::is_constructible_v<T, Gluon::StringWrapper<Ptr>> && (std::is_constructible_v<std::u16string_view, T> || std::is_constructible_v<std::string_view, T>))
    Gluon::StringWrapper<Ptr> operator+(T const lhs, Gluon::StringWrapper<Ptr> const& rhs) noexcept {
        return Gluon::Detail::stringAppend(lhs, rhs.operator Il2CppString const*());
    }

    // if system string exists, we can use it in StringW, but with a compile definition it can be disabled
#if !defined(NO_CODEGEN_WRAPPERS) && __has_include("System/String.hpp")
    // put using statement
    using StringW = StringWrapper<System::String*>;
#else
    using StringW = Gluon::StringWrapper<Il2CppString*>;
#endif
} // Gluon

MARK_GEN_REF_T(Gluon::StringWrapper);
MARK_REF_PTR_T(Il2CppString);

template <class Ptr>
struct GLUON_HIDDEN Gluon::TypeCheck::NeedBox<Gluon::StringWrapper<Ptr>> {
    constexpr static bool value = false;
};

static_assert(sizeof(Gluon::StringW) == sizeof(void*));
static_assert(Gluon::TypeConcepts::HasIl2CppConversion<Gluon::StringW>);
DEFINE_IL2CPP_DEFAULT_TYPE(Gluon::StringW, string);
NEED_NO_BOX(Gluon::StringW);

template <typename Ptr>
auto format_as(Gluon::StringWrapper<Ptr> s) {
    if (!s) return std::string("StringW(null)");
    return static_cast<std::string>(s);
}

#endif // GLUON_WRAPPERS_STRING_HPP_