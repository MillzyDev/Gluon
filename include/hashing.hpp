#ifndef GLUON_HASHING_HPP_
#define GLUON_HASHING_HPP_

#include <cstdint>
#include <utility>

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
}

#endif