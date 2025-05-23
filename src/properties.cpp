#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>

#include "hashing.hpp"
#include "properties.hpp"

#include "classes.hpp"
#include "il2cpp_functions.hpp"

namespace Gluon::Properties {
    const PropertyInfo *findProperty(Il2CppClass *klass, std::string_view propertyName) {
        static std::unordered_map<std::pair<const Il2CppClass *, std::string>, const PropertyInfo *, Gluon::Hashing::HashPair> propertyCache;
        static std::mutex propertyCacheMutex;

        Gluon::Il2CppFunctions::initialise();
        RET_0_UNLESS(klass);

        // Check cache
        const auto key = std::pair<const Il2CppClass *, std::string>(klass, propertyName);
        propertyCacheMutex.lock();
        if (const auto itr = propertyCache.find(key); itr != propertyCache.end()) {
            propertyCacheMutex.unlock();
            return itr->second;
        }
        propertyCacheMutex.unlock();

        const PropertyInfo *property = Gluon::Il2CppFunctions::class_get_property_from_name(klass, propertyName.data());
        if (!property) {
            Gluon::Logger::error("Could not find property {} in class {}", propertyName.data(),
                Gluon::Classes::getClassStandardName(klass).c_str());
            // TODO: log properties

            if (klass->parent != klass) {
                property = Gluon::Properties::findProperty(klass->parent, propertyName);
            }
        }

        propertyCacheMutex.lock();
        propertyCache.emplace(key, property);
        propertyCacheMutex.unlock();
        return property;
    }

    const PropertyInfo *findProperty(const std::string_view namespaze, const std::string_view className,
        const std::string_view propertyName) {
        return Gluon::Properties::findProperty(Gluon::Classes::getClassFromName(namespaze, className), propertyName);
    }


}