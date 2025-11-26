#include <condition_variable>
#include <mutex>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include "classes.hpp"

#include "assemblies.hpp"
#include "il2cpp_functions.hpp"

#include "il2cpp-class-internals.h"

class HashPair {
public:
    template<class T1, class T2>
    size_t operator()(const std::pair<T1, T2> &pair) const {
        auto hash1 = std::hash<T1>{}(pair.first);
        auto hash2 = std::hash<T2>{}(pair.second);
        return hash1 ^ hash2;
    }
};

namespace Gluon::Classes {
    Il2CppClass *findClass(std::string_view namespaze, std::string_view klass) {
        static std::mutex mutex;
        static std::unordered_map<std::pair<std::string, std::string>, Il2CppClass *, HashPair> cache;

        Gluon::Il2CppFunctions::initialise();
        const auto key = std::pair<std::string, std::string>(namespaze, klass);
        {
            std::unique_lock lock(mutex);
            if (const auto it = cache.find(key); it != cache.end()) {
                return it->second;
            }
        }

        std::span<const Il2CppAssembly *> assemblies = Gluon::Assemblies::getAssemblies();
        for (const Il2CppAssembly *assembly : assemblies) {
            const Il2CppImage *image = Gluon::Assemblies::getImage(assembly);

            if (!image) {
                // TODO: log error
                continue;
            }

            if (Il2CppClass *result = Gluon::Il2CppFunctions::class_from_name(image, namespaze.data(), klass.data())) {
                std::unique_lock lock(mutex);
                cache.emplace(key, result);
                return result;
            }
        }

        // Check for nested class
        const std::size_t token = klass.find('/');
        if (token != std::string::npos) {

        }

        return nullptr;
    }

} // Gluon::Classes