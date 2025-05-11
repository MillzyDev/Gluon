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
            Gluon::Logger::info("After resolving method with slot: {} found method {}", slot, method);
        }

        // resolved method slot should be the slot we asked for if it came from a non-interface
        RET_DEFAULT_UNLESS(method && slot == method->slot);
        return method;
    }

    const MethodInfo *resolveVTableSlot(Il2CppClass *klass, std::string_view declaringNamespace, std::string_view declaringClassName, std::uint16_t slot) noexcept {
        return resolveVTableSlot(klass, Gluon::Classes::getClassFromName(declaringNamespace, declaringClassName), slot);
    }

}