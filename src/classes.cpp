#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "classes.hpp"

#include <exceptions.hpp>
#include <methods.hpp>

#include "arrays.hpp"
#include "assemblies.hpp"
#include "exceptions.hpp"
#include "gluon_logging.hpp"
#include "hashing.hpp"
#include "il2cpp_functions.hpp"
#include "methods.hpp"

#include "il2cpp-runtime-metadata.h"

namespace Gluon::Classes {
    const char *getTypeSimpleName(const Il2CppType *type) {
        static std::unordered_map<Il2CppClass *, const char *> typeIndex;
        static std::mutex typeIndexMutex;

        Gluon::Il2CppFunctions::initialise();

        typeIndexMutex.lock();
        if (typeIndex.empty()) {
            typeIndex[Gluon::Il2CppFunctions::il2cppDefaults->boolean_class] = "bool";
            typeIndex[Gluon::Il2CppFunctions::il2cppDefaults->byte_class] = "byte";
            typeIndex[Gluon::Il2CppFunctions::il2cppDefaults->sbyte_class] = "sbyte";
            typeIndex[Gluon::Il2CppFunctions::il2cppDefaults->char_class] = "char";
            typeIndex[Gluon::Il2CppFunctions::il2cppDefaults->single_class] = "float";
            typeIndex[Gluon::Il2CppFunctions::il2cppDefaults->double_class] = "double";
            typeIndex[Gluon::Il2CppFunctions::il2cppDefaults->int16_class] = "short";
            typeIndex[Gluon::Il2CppFunctions::il2cppDefaults->uint16_class] = "ushort";
            typeIndex[Gluon::Il2CppFunctions::il2cppDefaults->int32_class] = "int";
            typeIndex[Gluon::Il2CppFunctions::il2cppDefaults->uint32_class] = "uint32";
            typeIndex[Gluon::Il2CppFunctions::il2cppDefaults->int64_class] = "long";
            typeIndex[Gluon::Il2CppFunctions::il2cppDefaults->uint64_class] = "ulong";
            typeIndex[Gluon::Il2CppFunctions::il2cppDefaults->object_class] = "object";
            typeIndex[Gluon::Il2CppFunctions::il2cppDefaults->string_class] = "string";
            typeIndex[Gluon::Il2CppFunctions::il2cppDefaults->void_class] = "void";
        }

        if (const auto it = typeIndex.find(Gluon::Il2CppFunctions::class_from_il2cpp_type(type)); it != typeIndex.end()) {
            typeIndexMutex.unlock();
            return it->second;
        }

        typeIndexMutex.unlock();
        return Gluon::Il2CppFunctions::type_get_name(type);
    }

    void genericsToStringHelper(const Il2CppGenericClass *genericClass, std::ostream  &os) {
        const Il2CppGenericContext *context = &genericClass->context;
        const Il2CppGenericInst *genericInst = context->class_inst;

        if (!genericInst) {
            genericInst = context->method_inst;
            if (genericInst) {
                Logging::Logger::warn("Missing class_inst! Trying method_inst?");
            }
        }

        if (genericInst) {
            os << "<";

            for (size_t i = 0; i < genericInst->type_argc; i++) {
                const Il2CppType *type = genericInst->type_argv[i];
                if (i > 0) {
                    os << ", ";
                }

                const char *typeName = getTypeSimpleName(type);
                os << typeName;
            }
            os << ">";
        }
        else {
            Gluon::Logging::Logger::warn("context->class_inst missing for genericClass!");
        }
    }

    std::string getClassStandardName(const Il2CppClass *klass, bool generics) {
        Gluon::Il2CppFunctions::initialise();

        std::stringstream ss;
        const char *namespaze = Gluon::Il2CppFunctions::class_get_namespace(klass);
        const Il2CppClass *declaring = Gluon::Il2CppFunctions::class_get_declaring_type(klass);

        if (const bool hasNamespace = (namespaze && namespaze[0] != '\0'); !hasNamespace && declaring) {
            ss << getClassStandardName(declaring) << "/";
        }
        else {
            ss << namespaze << "::";
        }

        ss << Gluon::Il2CppFunctions::class_get_name(klass);

        if (generics) {
            Gluon::Il2CppFunctions::class_is_generic(klass);

            if (const Il2CppGenericClass *genericClass = klass->generic_class) {
                genericsToStringHelper(genericClass, ss);
            }
        }

        return ss.str();
    }

    Il2CppClass *findNestedClass(Il2CppClass *declaring, std::string_view name) {
        if (!declaring) {
            return nullptr;
        }

        const size_t token = name.find('/');
        const size_t deeperNested = token != std::string::npos;

        const std::string_view subTypeName = deeperNested ? name : name.substr(0, token);

        void *iter = nullptr;
        Il2CppClass *found = nullptr;
        while(Il2CppClass *nested = Gluon::Il2CppFunctions::class_get_nested_types(declaring, &iter)) {
            if (subTypeName == nested->name) {
                found = nested;
                break;
            }
        }

        if (deeperNested) {
            return findNestedClass(found, name.substr(token + 1));
        }

        return found;
    }

    Il2CppReflectionType *getSystemType(const Il2CppClass *klass) {
        Gluon::Il2CppFunctions::initialise();
        RET_0_UNLESS(klass);

        const Il2CppType *type = Il2CppFunctions::class_get_type_const(klass);
        return getSystemType(type);
    }

    Il2CppReflectionType *getSystemType(const Il2CppType *type) {
        Gluon::Il2CppFunctions::initialise();
        return reinterpret_cast<Il2CppReflectionType *>(Gluon::Il2CppFunctions::type_get_object(type));
    }

    Il2CppReflectionType *makeGenericType(Il2CppReflectionType *genericType, Il2CppArray *types) {
        Gluon::Il2CppFunctions::initialise();

        const Il2CppClass *runtimeType = RET_0_UNLESS(Gluon::Il2CppFunctions::il2cppDefaults->runtimetype_class);
        const MethodInfo *makeGenericMethod = RET_0_UNLESS(Gluon::Methods::findMethodUnsafe(runtimeType, "MakeGenericType", 2));

        Il2CppException *exception = nullptr;
        void *params[] = { reinterpret_cast<void *>(genericType), reinterpret_cast<void *>(types) };
        Il2CppObject *genericTypeObj = Gluon::Il2CppFunctions::runtime_invoke(makeGenericMethod, nullptr, params, &exception);
        if (exception) {
            Gluon::Logger::error("makeGenericType: Failed with the exception: {}", Gluon::Exceptions::exceptionToString(exception).c_str());
            return nullptr;
        }

        return reinterpret_cast<Il2CppReflectionType *>(genericTypeObj);
    }

    Il2CppClass *makeGeneric(const Il2CppClass *klass, const std::span<const Il2CppClass *const> args) {
        Gluon::Il2CppFunctions::initialise();

        static Il2CppClass *type = RET_0_UNLESS(Gluon::Il2CppFunctions::il2cppDefaults->systemtype_class);
        Il2CppReflectionType *klassType = RET_0_UNLESS(Gluon::Classes::getSystemType(klass));

        // Call Type.MakeGenericType on it
        Il2CppArray *array = Gluon::Il2CppFunctions::array_new_specific(type, args.size());
        if (!array) {
            Gluon::Logger::error("Failed to make new array with length: {}", args.size());
            return nullptr;
        }

        for (std::size_t i = 0; i < args.size(); i++) {
            Il2CppReflectionType *elementType = Gluon::Classes::getSystemType(args[i]);
            if (!elementType) {
                Gluon::Logger::error("Failed to get type for {}",
                    Gluon::Il2CppFunctions::class_get_name_const(args[i]));
                return nullptr;
            }

            IL2CPP_ARRAY_SET(array, void *, i, reinterpret_cast<void *>(elementType));
        }

        Il2CppReflectionType *reflectionType = RET_0_UNLESS(makeGenericType(klassType, array));
        Il2CppClass *ret = RET_0_UNLESS(Gluon::Il2CppFunctions::class_from_system_type(reflectionType));
        return ret;
    }

    Il2CppClass *makeGeneric(const Il2CppClass *klass, const Il2CppType **types, std::uint32_t count) {
        static Il2CppClass *type = RET_0_UNLESS(Gluon::Il2CppFunctions::il2cppDefaults->systemtype_class);
        Il2CppReflectionType *klassType = RET_0_UNLESS(Gluon::Classes::getSystemType(klass));

        // Call Type.MakeGeneric on it
        Il2CppArray *array = Gluon::Il2CppFunctions::array_new_specific(type, count);
        if (!array) {
            Gluon::Logger::error("Failed to make new array with length: {}", count);
            return nullptr;
        }

        for (std::size_t i = 0; i < count; i++) {
            Il2CppReflectionType *elementType = Gluon::Classes::getSystemType(types[i]);
            if (!elementType) {
                Gluon::Logger::error("Failed to get type for {}", Gluon::Il2CppFunctions::type_get_name(types[i]));
                return nullptr;
            }

            IL2CPP_ARRAY_SET(array, void *, i, reinterpret_cast<void *>(elementType));
        }

        Il2CppReflectionType *reflectionType = RET_0_UNLESS(makeGenericType(klassType, array));
        Il2CppClass *ret = RET_0_UNLESS(Gluon::Il2CppFunctions::class_from_system_type(reflectionType));
        return ret;
    }

    Il2CppClass *getClassFromName(std::string_view namespaze, std::string_view name) {
        static std::unordered_map<std::pair<std::string, std::string>, Il2CppClass *, Gluon::Hashing::HashPair> classCache;
        static std::mutex classCacheMutex;

        Gluon::Il2CppFunctions::initialise();

        // Check cache
        const auto key = std::pair<std::string, std::string>(namespaze, name);

        classCacheMutex.lock();
        if (const auto itr = classCache.find(key); itr != classCache.end()) {
            classCacheMutex.unlock();
            return itr->second;
        }
        classCacheMutex.unlock();

        Il2CppDomain *domain = RET_0_UNLESS(Gluon::Il2CppFunctions::domain_get());
        std::size_t assemblyCount;
        const Il2CppAssembly **assemblies = Gluon::Il2CppFunctions::domain_get_assemblies(domain, &assemblyCount);

        for (std::size_t i = 0; i < assemblyCount; i++) {
            const Il2CppAssembly *assembly = assemblies[i];
            const Il2CppImage *image = Gluon::Il2CppFunctions::assembly_get_image(assembly);

            if (!image) {
                Gluon::Logger::error("Assembly with name: {} has a null image!", assembly->aname.name);
                continue;
            }

            if (Il2CppClass *klass = Gluon::Il2CppFunctions::class_from_name(image, namespaze.data(), name.data())) {
                classCacheMutex.lock();
                classCache.emplace(key, klass);
                classCacheMutex.unlock();
                return klass;
            }
        }

        // Unable to find class directly; check for nested and look for it
        const std::size_t token = name.find('/');
        if (bool nested = token != std::string::npos) { // nested name
            // get the first part of the nested name
            const auto declaringTypeName = std::string(name.substr(0, token));
            // get the first class; the declaring class
            Il2CppClass *declaring = getClassFromName(namespaze, declaringTypeName);
            // recursively look through nested classes of the declaring until no more tokens ('/')

            if (Il2CppClass *klass = findNestedClass(declaring, name.substr(token + 1))) {
                classCacheMutex.lock();
                classCache.emplace(key, klass);
                classCacheMutex.unlock();
                return klass;
            }
        }

        Gluon::Logger::error("Could not find class {}::{}", namespaze.data(), name.data());
        return nullptr;
    }


}