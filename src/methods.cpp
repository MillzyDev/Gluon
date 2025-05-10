#include "methods.hpp"

#include <unordered_map>
#include <mutex>
#include <span>
#include <string>

#include "abortion.hpp"
#include "arrays.hpp"
#include "classes.hpp"
#include "hashing.hpp"
#include "il2cpp_functions.hpp"

namespace Gluon::Methods {
    const MethodInfo *findMethodUnsafe(const Il2CppClass *klass, std::string_view methodName, int argsCount) {
        static std::unordered_map<std::pair<const Il2CppClass *, std::pair<std::string, decltype(MethodInfo::parameters_count)>>,
            const MethodInfo *, Gluon::Hashing::HashPair3> methodsCache;
        static std::mutex methodsCacheMutex;

        RET_DEFAULT_UNLESS(klass);

        // Check cache
        auto innerPair = std::pair<std::string, decltype(MethodInfo::parameters_count)>(methodName, argsCount);
        auto key = std::pair(klass, innerPair);
        methodsCacheMutex.lock();

        if (const auto itr = methodsCache.find(key); itr != methodsCache.end()) {
            methodsCacheMutex.unlock();
            return itr->second;
        }
        methodsCacheMutex.unlock();

        const MethodInfo *methodInfo = Gluon::Il2CppFunctions::class_get_method_from_name(klass, methodName.data(), argsCount);
        if (!methodInfo) {
            Gluon::Logger::error("Could not find method {} with {} parameters in class '{}'!", methodName.data(),
                argsCount, Gluon::Classes::getClassStandardName(klass).c_str());
            // TODO: LogMethods impl
            RET_DEFAULT_UNLESS(methodInfo);
        }

        methodsCacheMutex.lock();
        methodsCache.emplace(key, methodInfo);
        methodsCacheMutex.unlock();

        return methodInfo;
    }

    const MethodInfo *findMethodUnsafe(const std::string_view namespaze, const std::string_view className, const std::string_view methodName, const int argsCount) {
        return findMethodUnsafe(Gluon::Classes::getClassFromName(namespaze, className), methodName, argsCount);
    }

    const MethodInfo *findMethodUnsafe(Il2CppObject *instance, const std::string_view methodName, const int argsCount) {
        Gluon::Il2CppFunctions::initialise();
        const Il2CppClass *klass = RET_DEFAULT_UNLESS(Gluon::Il2CppFunctions::object_get_class(instance));
        return findMethodUnsafe(klass, methodName, argsCount);
    }


}