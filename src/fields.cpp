#include "fields.hpp"

#include <mutex>
#include <string>
#include <string_view>
#include <unordered_map>

#include "abortion.hpp"
#include "classes.hpp"
#include "gluon_logging.hpp"
#include "hashing.hpp"
#include "il2cpp_functions.hpp"

namespace Gluon::Fields {
    Il2CppClass *getFieldClass(FieldInfo *field) {
        const Il2CppType *type = RET_0_UNLESS(Gluon::Il2CppFunctions::field_get_type(field));
        return Gluon::Il2CppFunctions::class_from_il2cpp_type(type);
    }

    FieldInfo *findField(Il2CppClass *klass, std::string_view fieldName) {
        Gluon::Il2CppFunctions::initialise();
        RET_0_UNLESS(klass);

        static std::unordered_map<std::pair<Il2CppClass *, std::string>, FieldInfo *, Gluon::Hashing::HashPair> fieldCache;
        static std::mutex fieldCacheMutex;

        // check cache
        auto key = std::pair<Il2CppClass *, std::string>(klass, fieldName);

        fieldCacheMutex.lock();
        auto itr = fieldCache.find(key);
        if (itr != fieldCache.end()) {
            fieldCacheMutex.unlock();
            return itr->second;
        }
        fieldCacheMutex.unlock();

        FieldInfo *field = Gluon::Il2CppFunctions::class_get_field_from_name(klass, fieldName.data());
        if (!field) {
            Gluon::Logger::error("Could not find field {} in class {}", fieldName.data(),
                Gluon::Classes::getClassStandardName(klass).c_str());
            // TODO: log fields
            if (klass->parent != klass) {
                field = findField(klass->parent, fieldName);
            }
        }

        fieldCacheMutex.lock();
        fieldCache.emplace(key, field);
        fieldCacheMutex.unlock();

        return field;
    }


}