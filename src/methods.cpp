#include <map>
#include <mutex>
#include <shared_mutex>

#include "methods.hpp"

#include "classes.hpp"
#include "gc.hpp"
#include "gluon_logging.hpp"
#include "hashing.hpp"
#include "il2cpp_functions.hpp"

#include "il2cpp-object-internals.h"

namespace Gluon::Methods {
    Il2CppObject *createManual(const Il2CppClass *klass) noexcept {
        if (!klass) {
            Gluon::Logging::Logger::error("Cannot create a manual object on a null class!");
            return nullptr;
        }

        if (!klass->initialized) {
            Gluon::Logging::Logger::error("Cannot create an object that does not have an initialised class: {:x}", reinterpret_cast<uintptr_t>(klass));
            return nullptr;
        }

        auto *object = reinterpret_cast<Il2CppObject *>(Gluon::Gc::gcAllocSpecific(klass->instance_size));
        if (!object) {
            Gluon::Logging::Logger::error("Failed to allocate GC specific area for instance size: {}", klass->instance_size);
            return nullptr;
        }

        object->klass = const_cast<Il2CppClass *>(klass);

        if (klass->has_cctor && !klass->cctor_finished_or_no_cctor && !klass->cctor_started) {
            object->klass->cctor_started = true;
            // TODO: Find and run cctor
            object->klass->cctor_finished_or_no_cctor = true;
        }

        return object;
    }

    Il2CppObject *createManualThrow(const Il2CppClass *klass) {

    }

    const MethodInfo *findMethodUnsafe(const Il2CppClass *klass, std::string_view methodName, int argsCount) {
        // cache
        static std::unordered_map<std::pair<const Il2CppClass *, std::pair<std::string, uint8_t>>, const MethodInfo *, Gluon::Hashing::HashPair3> unsafeMethodsCache;
        static std::mutex unsafeMethodsCacheMutex;

        Gluon::Il2CppFunctions::initialise();

        if (!klass) {
            return nullptr;
        }

        // check cache
        auto innerPair = std::pair<std::string, uint8_t>(methodName, argsCount);
        auto key = std::pair<const Il2CppClass *, decltype(innerPair)>(klass, innerPair);
        unsafeMethodsCacheMutex.lock();

        auto it = unsafeMethodsCache.find(key);
        if (it != unsafeMethodsCache.end()) {
            unsafeMethodsCacheMutex.unlock();
            return it->second;
        }
        unsafeMethodsCacheMutex.unlock();

        // try get from IL2CPP
        const MethodInfo *methodInfo = Gluon::Il2CppFunctions::class_get_method_from_name(klass, methodName.data(), argsCount);
        if (!methodInfo) {
            Gluon::Logging::Logger::error("Could not fund method {} with {} parameters in class '{}'.", methodName.data(),
                                          argsCount, Gluon::Classes::getClassStandardName(klass).c_str());
            // TODO: LogMethods (requires implementation)
            return nullptr;
        }

        unsafeMethodsCacheMutex.lock();
        unsafeMethodsCache.emplace(key, methodInfo);
        unsafeMethodsCacheMutex.unlock();

        return methodInfo;
    }

    const MethodInfo *findMethod(const FindMethodInfo &info) {
        static std::unordered_map<FindMethodInfo, const MethodInfo *> methodsCache;
        static std::shared_mutex methodsCacheMutex;

        Gluon::Il2CppFunctions::initialise();
        Il2CppClass *klass = info.klass;

        if (!klass) {
            return nullptr;
        }

        // TODO: oh god
    }
}