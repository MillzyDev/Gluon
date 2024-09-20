#include <map>
#include <mutex>
#include <shared_mutex>
#include <sstream>

#include "methods.hpp"

#include "classes.hpp"
#include "gc.hpp"
#include "gluon_logging.hpp"
#include "hashing.hpp"
#include "il2cpp_functions.hpp"

#include "il2cpp-object-internals.h"
#include "il2cpp-tabledefs.h"

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

    Il2CppClass *getTypeClass(const Il2CppType *il2cppType, const MethodInfo *methodInfo) {
        if (il2cppType->type == IL2CPP_TYPE_CLASS || il2cppType->type == IL2CPP_TYPE_VALUETYPE) {
            return Gluon::Il2CppFunctions::MetadataCache_GetTypeInfoFromHandle(il2cppType->data.typeHandle);
        }

        if (il2cppType->type == IL2CPP_TYPE_VAR || il2cppType->type == IL2CPP_TYPE_MVAR) {
            GenericParameterIndex genericParam = Gluon::Il2CppFunctions::MetadataCache_GetGenericParameterIndexFromParameter(il2cppType->data.genericParameterHandle);

            const Il2CppGenericContainer *genericContainer = getGenericContainer(methodInfo);
            const Il2CppGenericInst *genericInst = methodInfo->genericMethod->context.method_inst;

            GenericParameterIndex genericParamIndex = genericParam - genericContainer->genericParameterStart;
            const Il2CppType *genericType = genericInst->type_argv[genericParamIndex];

            return getTypeClass(genericType, methodInfo);
        }

        if (il2cppType->type == IL2CPP_TYPE_GENERICINST) {
            Il2CppGenericClass *genericData = il2cppType->data.generic_class;
            return getTypeClass(genericData->type, methodInfo);
        }

        return nullptr;
    }

    std::int64_t calculateParamWeight(Il2CppClass *paramClass, Il2CppClass *expectedParamClass) {
        if (!paramClass || !expectedParamClass) {
            return 1;
        }

        std::span<const Il2CppClass *const> methodInterfaces(paramClass->implementedInterfaces, paramClass->interfaces_count);
        std::int64_t distance = 0;

        if (paramClass == expectedParamClass) {
            return distance;
        }

        bool isInterface = paramClass->flags & TYPE_ATTRIBUTE_INTERFACE;
        bool isExpectedInterface = expectedParamClass->flags & TYPE_ATTRIBUTE_INTERFACE;

        // method == interface, we expect concrete type; avoid
        if (isExpectedInterface && !isInterface) {
            return 1000;
        }

        if (isInterface) {
            // if interface, add lots of weight; choose concrete type instead
            distance += 5;
        }

        std::span<const Il2CppClass *const> expectedInterfaces = { expectedParamClass->implementedInterfaces, expectedParamClass->interfaces_count };

        // find all interfaces that intersect with our expected type
        std::vector<const Il2CppClass *> interfaceIntersections;
        interfaceIntersections.reserve(expectedInterfaces.size());
        std::set_intersection(expectedInterfaces.begin(), expectedInterfaces.end(), methodInterfaces.begin(), methodInterfaces.end(), std::back_inserter(interfaceIntersections));
        std::size_t interfaceSharing = interfaceIntersections.size();

        while (expectedParamClass && expectedParamClass != paramClass) {
            if (Gluon::Il2CppFunctions::class_is_assignable_from(paramClass, expectedParamClass)) {
                break;
            }

            expectedParamClass = expectedParamClass->parent;
            distance++;
        }

        // subtract distance by specificity of interface
        // since it allows specifity (this a real word?)
        distance -= interfaceSharing;

        return distance;
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
        // TODO
        static std::unordered_map<FindMethodInfo, const MethodInfo *> methodsCache;
        static std::shared_mutex methodsCacheMutex;

        Gluon::Il2CppFunctions::initialise();
        Il2CppClass *klass = info.klass;

        if (!klass) {
            return nullptr;
        }

        // check cache
        std::shared_lock sharedLock(methodsCacheMutex);
        auto it = methodsCache.find(info);
        if (it != methodsCache.end()) {
            return it->second;
        }

        std::vector<const MethodInfo *> matches;
        matches.reserve(1);

        const MethodInfo *target = nullptr;

        auto addMethodsToMatches = [&](Il2CppClass *targetKlass) {
            if (!targetKlass->initialized_and_no_error) {
                Gluon::Il2CppFunctions::Class_Init(targetKlass);
            }

            auto const methods = std::span(targetKlass->methods, targetKlass->method_count);
            for (auto const &current : methods) {
                if (info.name != current->name) {
                    continue;
                }

                bool isPerfect;
                if (!parameterMatch(current, std::span(info.genericTypes), std::span(info.argumentTypes), &isPerfect)) {
                    continue;
                }

                if (isPerfect) {
                    target = current;
                    break;
                }

                auto methodParams = std::span(current->parameters, current->parameters_count);

                matches.push_back(current);
            }
        };

        // now look for matches recursively
        Il2CppClass *targetKlass = info.klass;
        // if reached no parent or perfect target found; done
        while (targetKlass != nullptr && target == nullptr) {
            addMethodsToMatches(targetKlass);
            targetKlass = targetKlass->parent;
        }

        // resolve method overloads
        if (!target && !matches.empty()) {
            // only one method found; use it
            if (matches.size() == 1) {
                target = matches.front();
            }

            // multiple methods
            if (!target) {
                std::vector<std::pair<const MethodInfo *, std::int64_t>> weightMap;
                weightMap.reserve(matches.size());

                // iterate methods
                for (const auto &method : matches) {
                    // resolve overload
                    std::int64_t weight = 0;

                    // weight methods based on their distance to the expected parameters
                    for (std::size_t i = 0; i < info.argumentTypes.size(); i++) {
                        const Il2CppType *expectedParamType = info.argumentTypes[i];
                        const Il2CppType *methodParamType = method->parameters[i];

                        Il2CppClass *methodParamClass = getTypeClass(methodParamType, method);
                        Il2CppClass *expectedParamClass = getTypeClass(expectedParamType, method);

                        weight += calculateParamWeight(methodParamClass, expectedParamClass);
                    }

                    weightMap.emplace_back(method, weight);
                }

                // if no perfect match, look for lowest weighted
                if (!target) {
                    Gluon::Logger::warn("findMethod found multiple methods that match for {}.{}", Gluon::Classes::getClassStandardName(klass), info.name);

                    for (auto const &[method, weight] : weightMap) {
                        Gluon::Logger::warn("findMethod weight {} method {}", weight, method->name);
                        // TODO: LogMethod
                    }

                    target = std::min_element(weightMap.begin(), weightMap.end(),
                                              [](auto a, auto b) { return a.second < b.second; })->first;
                    Logger::warn("Using the following:");
                    CRASH_UNLESS(target);
                    // TODO: LogMethod
                }
            }
        }

        if (!target) {
            std::stringstream ss;
            ss << ((matches.size() > 1) ? "found multiple matches for" : "could not find");
            ss << " method " << info.name;

            bool first = true;
            // list generic types
            if (!info.genericTypes.empty()) {
                ss << "<";
                for (const Il2CppClass *type : info.genericTypes) {
                    if (!first) {
                        ss << ", ";
                    }

                    first = false;
                    ss << Gluon::Classes::getClassStandardName(type);
                }
                ss << ">";
            }

            // list params
            first = true;
            ss << "(";
            for (const Il2CppType *type : info.argumentTypes) {
                if (!first) {
                    ss << ", ";
                }

                first = false;
                ss << Gluon::Classes::getTypeSimpleName(type);
            }
            ss << ") in class '" << Gluon::Classes::getClassStandardName(klass) << "'!";
            Gluon::Logger::error("{}", ss.str().c_str());
            // TODO: LogMethods
        }

        // add to cache
        std::unique_lock uniqueLock(methodsCacheMutex);
        methodsCache.emplace(std::move(info), target);

        return target;
    }

    bool isConvertibleFrom(const Il2CppType *to, const Il2CppType *from, bool asArgs) {
        if (!to || !from) {
            return false;
        }

        if (asArgs) {
            if (to->byref) {
                if (!from->byref) {
                    // TODO: debug log
                    return false;
                }
            }
        }

        Gluon::Il2CppFunctions::initialise();

        Il2CppClass *classTo = Gluon::Il2CppFunctions::class_from_il2cpp_type(to);
        Il2CppClass *classFrom = Gluon::Il2CppFunctions::class_from_il2cpp_type(from);

        bool ret = (to->type == IL2CPP_TYPE_MVAR || Gluon::Il2CppFunctions::class_is_assignable_from(classTo, classFrom));
        if (!ret) {
            if (Gluon::Il2CppFunctions::class_is_enum(classTo)) {
                ret = isConvertibleFrom(Gluon::Il2CppFunctions::class_enum_basetype(classTo), from, asArgs);
            }
        }

        return ret;
    }

    void logMethod(const MethodInfo *method) {
        if (!method) {
            return;
        }

        Gluon::Il2CppFunctions::initialise();

        uint32_t flags = Gluon::Il2CppFunctions::method_get_flags(method, nullptr);

        std::stringstream flagStream;
        if (flags & METHOD_ATTRIBUTE_STATIC) {
            flagStream << "static ";
        }
        if (flags & METHOD_ATTRIBUTE_VIRTUAL) {
            flagStream << "virtual ";
        }
        if (flags & METHOD_ATTRIBUTE_ABSTRACT) {
            flagStream << "abstract ";
        }

        const std::string &flagStrRef = flagStream.str();
        const char *flagStr = flagStrRef.c_str();

        const Il2CppType *retType = Gluon::Il2CppFunctions::method_get_return_type(method);
        std::string retTypeStr = Gluon::Classes::getTypeSimpleName(retType);

        const char *methodName = Gluon::Il2CppFunctions::method_get_name(method);
        methodName = methodName ? methodName : "__noname__";

        std::stringstream paramStream;
        for (size_t i = 0; i < Gluon::Il2CppFunctions::il2cpp_method_get_param_count(method); i++) {
            if (i > 0) {
                paramStream << ", ";
            }

            auto *argType = Gluon::Il2CppFunctions::method_get_param(method, i);

            if (Gluon::Il2CppFunctions::type_is_byref(argType)) {
                paramStream << "out/ref ";
            }

            paramStream << Gluon::Classes::getTypeSimpleName(argType) << " ";

            const char *name = Gluon::Il2CppFunctions::method_get_param_name(method, i);
            paramStream << (name ? name : "__noname__");
        }

        const std::string &paramStrRef = paramStream.str();
        const char *paramStr = paramStrRef.c_str();

        Gluon::Logger::debug("{}{} {}({});", flagStr, retTypeStr, methodName, paramStr);
    }

    void logMethods(const Il2CppClass *klass, bool logParents) {
        if (!klass) {
            return;
        }

        if (klass->name) {
            Gluon::Il2CppFunctions::initialise();
            Gluon::Il2CppFunctions::Class_Init(const_cast<Il2CppClass *>(klass));
        }

        if (klass->method_count && !klass->methods) {
            Gluon::Logger::warn("Class is valid and claims to have methods, but methods pointer is null! class name: {}",
                                Gluon::Classes::getClassStandardName(klass).c_str());
            return;
        }

        if (logParents) {
            Gluon::Logger::info("class name: {}", Gluon::Classes::getClassStandardName(klass));
        }

        Gluon::Logger::debug("method_count: {}", klass->method_count);
        for (int i = 0; i < klass->method_count; i++) {
            if (klass->methods[i]) {
                Gluon::Logger::debug("Method {}:", i);
                logMethod(klass->methods[i]);
            }
            else {
                Gluon::Logger::warn("Method {} does not exist!", i);
            }
        }

        usleep(100); // 0.0001s
        if (logParents && klass->parent && (klass->parent != klass)) {
            logMethods(klass->parent, true);
        }
    }
}