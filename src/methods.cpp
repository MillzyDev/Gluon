#include "methods.hpp"

#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <span>
#include <string>
#include <sstream>

#include "abortion.hpp"
#include "arrays.hpp"
#include "classes.hpp"
#include "hashing.hpp"
#include "il2cpp_functions.hpp"

#include "il2cpp-tabledefs.h"

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

    // get class from type and match Var/MVar
    Il2CppClass *getTypeClass(const Il2CppType *type, const MethodInfo *methodInfo) {
        if (type->type == IL2CPP_TYPE_CLASS || type->type == IL2CPP_TYPE_VALUETYPE) {
            return Gluon::Il2CppFunctions::MetadataCache_GetTypeInfoFromHandle(type->data.typeHandle);
        }

        if (type->type == IL2CPP_TYPE_VAR || type->type == IL2CPP_TYPE_MVAR) {
            const GenericParameterIndex genericParameterIndex =
                Gluon::Il2CppFunctions::MetadataCache_GetGenericParameterIndexFromParameter(type->data.genericParameterHandle);
            const Il2CppGenericContainer *genericContainer = getGenericContainer(methodInfo);
            const Il2CppGenericInst *genericInst = methodInfo->genericMethod->context.method_inst;;

            const Il2CppType *genericType = genericInst->type_argv[genericParameterIndex - genericContainer->genericParameterStart];
            return getTypeClass(genericType, methodInfo);
        }

        if (type->type == IL2CPP_TYPE_GENERICINST) {
            const Il2CppType *genericType = type->data.generic_class->type;
            return getTypeClass(genericType, methodInfo);
        }

        return nullptr;
    }

    std::int64_t calculateParameterWeight(Il2CppClass *methodParamClass, Il2CppClass *expectedParamClass) {
        if (!methodParamClass || !expectedParamClass) {
            return 1;
        }

        std::span<const Il2CppClass *const> methodInterfaces = { methodParamClass->implementedInterfaces, methodParamClass->interfaces_count };

        std::int64_t distance = 0;

        if (methodParamClass == expectedParamClass) {
            return distance;
        }

        const bool isMethodInterface = methodParamClass->flags & TYPE_ATTRIBUTE_INTERFACE;

        // method is an interface, we should STRONGLY prefer a concrete type - avoid at all costs
        if (const bool isExpectedInterface = expectedParamClass->flags & TYPE_ATTRIBUTE_INTERFACE; isExpectedInterface && !isMethodInterface) {
            return 1000;
        }

        if (isMethodInterface) {
            // if interface, just add lots of weight
            // prefer concrete types
            distance += 5;
        }

        std::span<const Il2CppClass *const> expectedInterfaces =
            { expectedParamClass->implementedInterfaces, expectedParamClass->interfaces_count };

        // find all interface that intersect with our expected type
        std::vector<const Il2CppClass *> interfaceIntersections;
        interfaceIntersections.reserve(expectedInterfaces.size());
        std::ranges::set_intersection(expectedInterfaces, methodInterfaces, std::back_inserter(interfaceIntersections));
        const std::size_t interfaceSharing = interfaceIntersections.size();

        while (expectedParamClass && expectedParamClass != methodParamClass) {
            if (!Gluon::Il2CppFunctions::class_is_assignable_from(methodParamClass, expectedParamClass)) {
                break;
            }

            expectedParamClass = expectedParamClass->parent;
            distance++;
        }

        // subtract distance by specificity of interface, since it allows specificity
        distance -= interfaceSharing;

        return distance;
    }

    const MethodInfo *findMethod(FindMethodInfo &&info) {
        static std::unordered_map<FindMethodInfo, const MethodInfo *> methodsCache;
        static std::shared_mutex methodsCacheMutex;

        Gluon::Il2CppFunctions::initialise();
        Il2CppClass *klass = info.klass;
        RET_DEFAULT_UNLESS(klass);

        // check cache
        {
            std::shared_lock lock(methodsCacheMutex);
            if (const auto itr = methodsCache.find(info); itr != methodsCache.end()) {
                return itr->second;
            }
        }

        // let me break it down for you, mark
        //
        // we look though all the methods have the following:
        // - matching name
        // - parameters match as defined in the parametersMatch function
        //
        // resolution priority is as follows:
        // - Identical parameters
        // - Only one method found
        // - Multiple matches: calculate a weight for all methods and use the lowest weighed one
        //
        // Cache results

        std::vector<const MethodInfo *> matches;
        matches.reserve(1);

        const MethodInfo *target;

        auto addMethodsToMatches = [&](Il2CppClass *targetClass) {
            // initialise class if needed
            if (!targetClass->initialized_and_no_error) {
                Gluon::Il2CppFunctions::Class_Init(targetClass);
            }

            // does NOT automatically recurse through the target's parents
            for (const auto methods = std::span(targetClass->methods, targetClass->method_count); const MethodInfo *const &current : methods) {
                if (info.name != current->name) {
                    continue;
                }

                // strict equality
                bool isPerfect;
                if (!parameterMatch(current, std::span(info.genericTypes), std::span(info.argTypes), &isPerfect)) {
                    continue;
                }

                if (isPerfect) {
                    target = current;
                    break;
                }

                // I have no clue why this is here, its unused but im too coward to remove it
                auto methodParameters = std::span(current->parameters, current->parameters_count);
                matches.push_back(current);
            }
        };

        // now look for matches recursively
        Il2CppClass *targetClass = info.klass;
        // if we reached no parent or prefect target is found, finish up
        while (targetClass != nullptr && target == nullptr) {
            addMethodsToMatches(targetClass);
            targetClass = targetClass->parent;
        }

        // Resolve method overloads
        if (!target && !matches.empty()) {
            // Only found one method? it's gonna have to do
            if (matches.size() == 1) {
                target = matches.front();
            }

            // multiple methods found
            // resolve overloads
            if (!target) {
                std::vector<std::pair<const MethodInfo *, std::int64_t>> weights;
                weights.reserve(matches.size());

                // iterate over methods
                for (const MethodInfo *const &method: matches) {
                    // overload resolution
                    std::int64_t weight = 0;

                    // weight the methods based on how far away they are from the expected parameters
                    for (std::size_t i = 0; i < info.argTypes.size(); i++) {
                        const Il2CppType *const &expectedParamType = info.argTypes[i];
                        const Il2CppType *const &methodParamType = method->parameters[i];

                        Il2CppClass *const methodParamClass = getTypeClass(methodParamType, method);
                        Il2CppClass *const expectedParamClass = getTypeClass(expectedParamType, method);

                        weight += calculateParameterWeight(methodParamClass, expectedParamClass);
                    }

                    weights.emplace_back(method, weight);
                }

                if (!target) {
                    Gluon::Logger::warn("findMethod: Found multiple methods that match for {}.{}",
                                        Gluon::Classes::getClassStandardName(klass).c_str(), info.name.data());
                    for (auto const &[method, weight] : weights) {
                        Gluon::Logger::warn("findMethod: weight {} method {}", weight, method->name);
                        // TODO: Log method
                    }

                    target = std::ranges::min_element(weights,
                        [](const std::pair<const MethodInfo *, std::int64_t> &a,
                                const std::pair<const MethodInfo *, std::int64_t> &b) {
                        return a.second < b.second;
                    })->first;

                    CRASH_UNLESS(target);

                    // Gluon::Logger::warn("Using the following:");
                    // TODO: Log method
                }
            }
        }

        if (!target) {
            std::stringstream ss;
            ss << (matches.size() > 1 ? "found multiple matches for" : "could not find");
            ss << " method " << info.name;

            bool first = true;
            // list generic types
            if (!info.genericTypes.empty()) {
                ss << "<";
                for (const Il2CppClass *genericType : info.genericTypes) {
                    if (!first) {
                        ss << ", ";
                    }

                    first = false;
                    ss << Gluon::Classes::getClassStandardName(genericType);
                }
                ss << ">";
            }

            first = true;
            ss << "(";
            for (const Il2CppType *argType : info.argTypes) {
                if (first) {
                    ss << ", ";
                }

                first = false;
                ss << Gluon::Classes::getTypeSimpleName(argType);
            }

            ss << ") in class '" << Gluon::Classes::getClassStandardName(klass) << "'";
            Gluon::Logger::error("{}", ss.str().c_str());
            // TODO: Log methods
        }

        // add to cache
        {
            std::unique_lock lock(methodsCacheMutex);
            methodsCache.emplace(std::move(info), target);
        }

        return target;
    }

    bool isConvertibleFrom(const Il2CppType *to, const Il2CppType *from, bool asArgs) {
        RET_0_UNLESS(to);
        RET_0_UNLESS(from);

        Gluon::Il2CppFunctions::initialise();

        if (asArgs && to->byref && !from->byref) {
            Gluon::Logger::debug("to ({}, {:x}) is ref/out while from  ({}, {:x}) is not. Not convertible.",
                Gluon::Classes::getTypeSimpleName(to), reinterpret_cast<std::uintptr_t>(to),
                Gluon::Classes::getTypeSimpleName(from), reinterpret_cast<std::uintptr_t>(from));
            return false;
        }

        Il2CppClass *classTo = Gluon::Il2CppFunctions::class_from_il2cpp_type(to);
        Il2CppClass *classFrom = Gluon::Il2CppFunctions::class_from_il2cpp_type(from);

        bool ret = to->type == IL2CPP_TYPE_MVAR || Gluon::Il2CppFunctions::class_is_assignable_from(classTo, classFrom);

        if (!ret && Gluon::Il2CppFunctions::class_is_enum(classTo)) {
            ret = isConvertibleFrom(Gluon::Il2CppFunctions::class_enum_basetype(classTo), from, asArgs);
        }

        return ret;
    }

    const MethodInfo *resolveMethodWithSlot(Il2CppClass *klass, std::uint16_t slot) noexcept {
        Gluon::Il2CppFunctions::initialise();

        if (!klass->initialized_and_no_error) {
            Gluon::Il2CppFunctions::Class_Init(klass);
        }

        const auto methodsEnd = klass->methods + klass->method_count;
        for (auto itr = klass->methods; itr != methodsEnd; itr++) {
            if ((*itr)->slot == slot) {
                return *itr;
            }
        }

        return nullptr;
    }


    const MethodInfo *resolveVTableSlot(Il2CppClass *klass, Il2CppClass *declaringClass, std::uint16_t slot) noexcept {
        if (!klass->initialized_and_no_error) {
            Gluon::Il2CppFunctions::Class_Init(klass);
        }

        if (!declaringClass->initialized_and_no_error) {
            Gluon::Il2CppFunctions::Class_Init(klass);
        }

        if (Gluon::Il2CppFunctions::class_is_interface(declaringClass)) {
            // if the declaring class is an interface, vtable_count means nothing and method_count should be used
            // the vtables are still valid, however
            if (slot >= declaringClass->method_count) { // tried to look for a slot outside the interface vtable
                // dump some info so the user can know which method was attempted to be resolved
                Gluon::Logger::error("Declaring class has a vtable that's too small, dumping resolve info:");
                Gluon::Logger::error("Instance class:               {}::{}", klass->namespaze, klass->name);
                Gluon::Logger::error("Instance class vtable slots:  {}", klass->vtable_count);
                Gluon::Logger::error("Declaring class:              {}::{}", declaringClass->namespaze, declaringClass->name);
                Gluon::Logger::error("Declaring class vtable slots: {}", declaringClass->vtable_count);
                Gluon::Logger::error("Attempted slot:               {}", slot);
                return nullptr;
            }

            for (std::uint16_t i = 0; i < klass->interface_offsets_count; i++) {
                if (klass->interfaceOffsets[i].interfaceType == declaringClass) {
                    std::int32_t offset = klass->interfaceOffsets[i].offset;
                    RET_DEFAULT_UNLESS(offset + slot < klass->vtable_count);
                    return klass->vtable[offset + slot].method;
                }
            }

            // if klass is an interface, and we haven't found the method yet; we should look in klass->methods anyway
            if (Gluon::Il2CppFunctions::class_is_interface(klass)) {
                RET_DEFAULT_UNLESS(slot < klass->method_count);
                return klass->methods[slot];
            }

            return nullptr;
        }

        RET_DEFAULT_UNLESS(slot < klass->vtable_count);
        const MethodInfo *method = klass->vtable[slot].method;

        if (method->slot != slot) {
            Gluon::Logger::warn("Resolving vtable slot led to a method info with a different slot; this method could be abstract.");
            Gluon::Logger::warn("Looking for: {}, resolved to: {}", slot, method->slot);

            method = resolveMethodWithSlot(klass, slot);
            Gluon::Logger::info("After resolving method with slot: {} found method {:x}", slot, reinterpret_cast<std::uintptr_t>(method));
        }

        // resolved method slot should be the slot we asked for if it came from a non-interface
        RET_DEFAULT_UNLESS(method && slot == method->slot);
        return method;
    }

    const MethodInfo *resolveVTableSlot(Il2CppClass *klass, std::string_view declaringNamespace, std::string_view declaringClassName, std::uint16_t slot) noexcept {
        return resolveVTableSlot(klass, Gluon::Classes::getClassFromName(declaringNamespace, declaringClassName), slot);
    }

}