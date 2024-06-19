#ifndef GLUON_HASHING_HPP_
#define GLUON_HASHING_HPP_

#include <cstdint>
#include <utility>

#include "methods.hpp"

namespace Gluon::Hashing {
    class HashPair {
    public:
        template<class T1, class T2>
        size_t operator()(const std::pair<T1, T2> &pair) const {
            auto hash1 = std::hash<T1>{}(pair.first);
            auto hash2 = std::hash<T2>{}(pair.second);
            return hash1 ^ hash2;
        }
    };

    class HashPair3 {
    public:
        template<class T1, class T2, class T3>
        size_t operator()(const std::pair<T1, std::pair<T2, T3>> &pair) const {
            auto hash1 = std::hash<T1>{}(pair.first);
            auto hash2 = HashPair{}(pair.second);
            return hash1 ^ hash2;
        }
    };

    template<class T>
    void hashCombine(std::size_t  &seed, T value) {
        seed ^= std::hash<T>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template<class T>
    std::size_t hashSequence(const T &sequence) noexcept {
        std::size_t seed = sequence.size();
        for (auto i : sequence) {
            hashCombine(seed, i);
        }
        return seed;
    }
}

namespace std {
    template<class T> struct hash<std::vector<T>> {
        std::size_t operator()(std::vector<T> const& seq) const noexcept {
            return Gluon::Hashing::hashSequence(seq);
        }
    };

    template <class T>
    struct hash<std::span<T>> {
        std::size_t operator()(std::span<T> const& seq) const noexcept {
            return Gluon::Hashing::hashSequence(seq);
        }
    };

    template <>
    struct hash<Gluon::Methods::FindMethodInfo> {
        std::size_t operator()(Gluon::Methods::FindMethodInfo const& info) const noexcept {
            auto hashPtr = std::hash<void*>{};

            auto hashSeqClass = std::hash<span<const Il2CppClass* const>>{};
            auto hashSeqType = std::hash<span<const Il2CppType* const>>{};

            auto hashStr = std::hash<std::string_view>{};

            return hashPtr(info.klass) ^ hashStr(info.name) ^ hashSeqType(info.argumentTypes) ^ hashSeqClass(info.genericTypes);
        }
    };
}

#endif