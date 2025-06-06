#include "wrappers_array.hpp"
#include "wrappers_object.hpp"
#include "wrappers_disposal.hpp"
#include "wrappers_list.hpp"
#include "wrappers_string.hpp"
#include "wrappers.hpp"

#include "classes.hpp"
#include "type_check.hpp"

#include "byref.hpp"
#include "wrappers_enum.hpp"
#include "wrappers_value.hpp"

namespace Gluon::Detail {
    template <class Facet>
    struct DeletableFacet : Facet {
        template <class... Args>
        explicit DeletableFacet(Args&&... args) : Facet(std::forward<Args>(args)...) {}
        ~DeletableFacet() {}
    };

    // Note that char is actually required here over char8_t-- this is due to NDK not having a char8_t specialization for this yet.
    DeletableFacet<std::codecvt<char16_t, char8_t, std::mbstate_t>> conv;

    template <class T>
    concept IsSpecialised = std::is_same_v<char16_t, typename T::intern_type>;
    static_assert(IsSpecialised<std::codecvt<char16_t, char8_t, std::mbstate_t>>);

    void convertString(char const* inp, char16_t* outp, const int sz) {
        std::mbstate_t state;
        char8_t const* from_next;
        char16_t* to_next;
        conv.in(state, reinterpret_cast<char8_t const*>(inp), reinterpret_cast<char8_t const*>(inp + sz), from_next, outp, outp + sz, to_next);
    }
    std::size_t convertString(char16_t const* inp, char* outp, const int isz, const int osz) {
        std::mbstate_t state;
        char16_t const* from_next;
        char8_t* to_next;
        if (auto convOut = conv.out(state, inp, inp + isz, from_next, reinterpret_cast<char8_t*>(outp), reinterpret_cast<char8_t*>(outp + osz), to_next); convOut != std::codecvt_base::ok) {
            throw convOut;
        }
        return static_cast<std::size_t>(to_next - reinterpret_cast<char8_t *>(outp));
    }

    Il2CppString* allocateString(const std::string_view str) {
        Gluon::Il2CppFunctions::initialise();

        if (str.data() == nullptr) {
            return Gluon::Il2CppFunctions::string_new_len("", 0);
        }

        return Gluon::Il2CppFunctions::string_new_len(str.data(), str.size());
    }
    Il2CppString* allocateString(const std::u16string_view str) {
        Gluon::Il2CppFunctions::initialise();
        if (str.data() == nullptr) {
            return Gluon::Il2CppFunctions::string_new_len("", 0);
        }

        return Gluon::Il2CppFunctions::string_new_utf16((Il2CppChar const*)str.data(), str.size());
    }

    Il2CppString* CreateString(int length) {
        static MethodInfo const* methodInfo = Gluon::Methods::findMethod(CLASS_OF(Il2CppString*), "CreateString",
                                                                       std::array<Il2CppType const*, 2>{ Gluon::Classes::extractIndependentType<Il2CppChar>(), Gluon::Classes::extractIndependentType<int>() });
        return CRASH_UNLESS(Gluon::Methods::runMethodOpt<Il2CppString*, false>(nullptr, methodInfo, static_cast<Il2CppChar>('\0'), length).value());
    }

    Il2CppString* stringAppend(Il2CppString const* lhs, Il2CppString const* rhs) noexcept {
        if (!lhs && !rhs) return nullptr;

        Gluon::Il2CppFunctions::initialise();
        if (!lhs && rhs)
            return Gluon::Il2CppFunctions::string_new_utf16(rhs->chars, rhs->length);
        else if (lhs && !rhs)
            return Gluon::Il2CppFunctions::string_new_utf16(lhs->chars, lhs->length);
        else {
            const size_t fullLength = lhs->length + rhs->length;
            Il2CppString* result = CreateString(fullLength);
            memcpy(result->chars, lhs->chars, lhs->length * sizeof(Il2CppChar));
            Il2CppChar* pastFirstString = result->chars + lhs->length;
            memcpy(pastFirstString, rhs->chars, rhs->length * sizeof(*rhs->chars));
            return result;
        }
    }

    Il2CppString* stringAppend(Il2CppString const* lhs, std::u16string_view const rhs) noexcept {
        if (lhs) {
            const int fullLength = lhs->length + rhs.size();
            Il2CppString* result = CreateString(fullLength);
            memcpy(result->chars, lhs->chars, lhs->length * sizeof(Il2CppChar));
            Il2CppChar* pastFirstString = result->chars + lhs->length;
            static_assert(sizeof(*pastFirstString) == sizeof(*rhs.data()));
            memcpy(pastFirstString, rhs.data(), rhs.size() * sizeof(*rhs.data()));
            return result;
        } else {
            return allocateString(rhs);
        }
    }

    Il2CppString* stringAppend(Il2CppString const* lhs, std::string_view const rhs) noexcept {
        if (lhs) {
            const int fullLength = lhs->length + rhs.size();
            Il2CppString* result = CreateString(fullLength);
            memcpy(result->chars, lhs->chars, lhs->length * sizeof(Il2CppChar));
            Il2CppChar* pastFirstString = result->chars + lhs->length;
            convertString(rhs.data(), pastFirstString, rhs.size());
            return result;
        } else {
            return allocateString(rhs);
        }
    }

    Il2CppString* stringAppend(std::string_view const lhs, Il2CppString const* rhs) noexcept {
        if (rhs) {
            const int fullLength = rhs->length + lhs.size();
            Il2CppString* result = CreateString(fullLength);
            convertString(lhs.data(), result->chars, lhs.size());
            Il2CppChar* pastFirstString = result->chars + lhs.size();
            memcpy(pastFirstString, rhs->chars, rhs->length * sizeof(*pastFirstString));
            return result;
        } else {
            return allocateString(lhs);
        }
    }

    Il2CppString* stringAppend(std::u16string_view const lhs, Il2CppString const* rhs) noexcept {
        if (rhs) {
            const int fullLength = rhs->length + lhs.size();
            Il2CppString* result = CreateString(fullLength);
            static_assert(sizeof(Il2CppChar) == sizeof(*lhs.data()));
            memcpy(result->chars, lhs.data(), lhs.size() * sizeof(*lhs.data()));
            Il2CppChar* pastFirstString = result->chars + lhs.size();
            memcpy(pastFirstString, rhs->chars, rhs->length * sizeof(*pastFirstString));
            return result;
        } else {
            return allocateString(lhs);
        }
    }

    template <typename T>
        requires(std::is_same_v<T, std::string_view> || std::is_same_v<T, std::u16string_view>)
    bool stringCompare(Il2CppString const* lhs, T const rhs) noexcept {
        if (!lhs || lhs->length != static_cast<int>(rhs.size())) return false;

        Il2CppChar const* first = lhs->chars;
        auto const* second = rhs.data();
        Il2CppChar const* firstEnd = first + lhs->length;
        auto const* secondEnd = second + static_cast<int>(rhs.size());

        while (first != firstEnd && second != secondEnd) {
            if (*first != *second) return false;
            first++;
            ++second;
        }

        return first == firstEnd && second == secondEnd;
    }

    bool stringCompare(Il2CppString const* lhs, std::string_view const rhs) noexcept {
        return stringCompare<std::remove_const_t<decltype(rhs)>>(lhs, rhs);
    }

    bool stringCompare(Il2CppString const* lhs, std::u16string_view const rhs) noexcept {
        return stringCompare<std::remove_const_t<decltype(rhs)>>(lhs, rhs);
    }

    bool stringCompare(Il2CppString const* lhs, Il2CppString const* rhs) noexcept {
        if (lhs == rhs) return true;
        if (!lhs || !rhs || lhs->length != rhs->length) return false;

        Il2CppChar const* first = lhs->chars;
        Il2CppChar const* second = rhs->chars;
        Il2CppChar const* firstEnd = first + lhs->length;
        Il2CppChar const* secondEnd = second + rhs->length;

        while (first != firstEnd && second != secondEnd) {
            if (*first != *second) return false;
            first++;
            second++;
        }

        return first == firstEnd && second == secondEnd;
    }

    template <typename T>
        requires(std::is_same_v<T, std::string_view> || std::is_same_v<T, std::u16string_view>)
    bool stringLess(Il2CppString const* lhs, T const rhs) noexcept {
        if (!lhs) return true;

        Il2CppChar const* first = lhs->chars;
        auto const* second = rhs.data();
        Il2CppChar const* firstEnd = lhs->chars + lhs->length;
        auto const* secondEnd = rhs.data() + rhs.size();

        while (first != firstEnd && second != secondEnd) {
            if (*first == *second) {
                first++;
                ++second;
                continue;
            }
            return *first < *second;
        }
        // if we got here, and second is not second end, we had a shorter first, so it should be true
        // if second is the end, we are longer, so it should be false
        return second != secondEnd;
    }

    bool stringLess(Il2CppString const* lhs, std::string_view const rhs) noexcept {
        return stringLess<std::remove_const_t<decltype(rhs)>>(lhs, rhs);
    }

    bool stringLess(Il2CppString const* lhs, std::u16string_view const rhs) noexcept {
        return stringLess<std::remove_const_t<decltype(rhs)>>(lhs, rhs);
    }

    bool stringLess(Il2CppString const* lhs, Il2CppString const* rhs) noexcept {
        if (!lhs && !rhs) return false;
        if (!lhs) return true;
        if (!rhs) return false;

        Il2CppChar const* first = lhs->chars;
        Il2CppChar const* second = rhs->chars;
        Il2CppChar const* firstEnd = first + lhs->length;
        Il2CppChar const* secondEnd = second + rhs->length;

        while (first != firstEnd && second != secondEnd) {
            if (*first == *second) {
                first++;
                second++;
                continue;
            }
            return *first < *second;
        }
        // if we got here, and second is not second end, we had a shorter first, so it should be true
        // if second is the end, we are longer, so it should be false
        return second != secondEnd;
    }

    template <typename T>
        requires(std::is_same_v<T, std::string_view> || std::is_same_v<T, std::u16string_view>)
    bool stringStart(Il2CppString const* lhs, T const rhs) noexcept {
        if (!lhs || lhs->length < static_cast<decltype(lhs->length)>(rhs.size())) return false;

        Il2CppChar const* first = lhs->chars;
        auto const* second = rhs.data();
        auto const* secondEnd = second + rhs.size();

        while (second != secondEnd) {
            if (*first == *second) {
                first++;
                ++second;
                continue;
            }
            // we got a mismatch! return false;
            return false;
        }
        // if we got through the entire string it was all equal, return true
        return true;
    }

    bool stringStart(Il2CppString const* lhs, std::string_view const rhs) noexcept {
        return stringStart<std::remove_const_t<decltype(rhs)>>(lhs, rhs);
    }

    bool stringStart(Il2CppString const* lhs, std::u16string_view const rhs) noexcept {
        return stringStart<std::remove_const_t<decltype(rhs)>>(lhs, rhs);
    }

    bool stringStart(Il2CppString const* lhs, Il2CppString const* rhs) noexcept {
        // if either instance is nullptr, return false, if our length is smaller than prefix length, also return false
        if (!lhs || !rhs || lhs->length < rhs->length) return false;

        Il2CppChar const* first = lhs->chars;
        Il2CppChar const* second = rhs->chars;
        Il2CppChar const* secondEnd = second + lhs->length;

        while (second != secondEnd) {
            if (*first == *second) {
                first++;
                second++;
                continue;
            }
            // we got a mismatch! return false;
            return false;
        }
        // if we got through the entire string it was all equal, return true
        return true;
    }

    template <typename T>
        requires(std::is_same_v<T, std::string_view> || std::is_same_v<T, std::u16string_view>)
    bool stringEnd(Il2CppString const* lhs, T const rhs) noexcept {
        if (!lhs || lhs->length < static_cast<decltype(lhs->length)>(rhs.size())) return false;

        Il2CppChar const* first = lhs->chars + lhs->length - 1;
        auto const* secondBegin = rhs.data() - 1;
        auto const* second = secondBegin + rhs.size();

        while (second != secondBegin) {
            if (*first == *second) {
                first--;
                --second;
                continue;
            }
            // we got a mismatch! return false;
            return false;
        }
        // if we got through the entire string it was all equal, return true
        return true;
    }

    bool stringEnd(Il2CppString const* lhs, std::string_view const rhs) noexcept {
        return stringEnd<std::remove_const_t<decltype(rhs)>>(lhs, rhs);
    }

    bool stringEnd(Il2CppString const* lhs, std::u16string_view const rhs) noexcept {
        return stringEnd<std::remove_const_t<decltype(rhs)>>(lhs, rhs);
    }

    bool stringEnd(Il2CppString const* lhs, Il2CppString const* rhs) noexcept {
        if (!lhs || !rhs || lhs->length < rhs->length) return false;

        Il2CppChar const* first = lhs->chars + lhs->length - 1;
        Il2CppChar const* secondBegin = rhs->chars - 1;
        Il2CppChar const* second = secondBegin + rhs->length;

        while (second != secondBegin) {
            if (*first == *second) {
                first--;
                second--;
                continue;
            }
            // we got a mismatch! return false;
            return false;
        }
        // if we got through the entire string it was all equal, return true
        return true;
    }
}