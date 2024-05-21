#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "classes.hpp"

#include "assemblies.hpp"
#include "gluon_logging.hpp"
#include "hashing.hpp"
#include "il2cpp_functions.hpp"

#include "il2cpp-runtime-metadata.h"

namespace Gluon::Classes {
    void genericsToStringHelper(Il2CppGenericClass *genericClass, std::ostream  &os) {
        Il2CppGenericContext *context = &genericClass->context;
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
                // TODO
                //const char *typeName = Try
            }
        }
    }

    std::string getClassStandardName(const Il2CppClass *klass, bool generics) {
        Gluon::Il2CppFunctions::initialise();

        std::stringstream ss;
        const char *namespaze = Gluon::Il2CppFunctions::class_get_namespace(klass);
        auto *declaring = Gluon::Il2CppFunctions::class_get_declaring_type(klass);

        bool hasNamespace = (namespaze && namespaze[0] != '\0');
        if (!hasNamespace && declaring) {
            ss << getClassStandardName(declaring) << "/";
        }
        else {
            ss << namespaze << "::";
        }

        ss << Gluon::Il2CppFunctions::class_get_name(klass);

        if (generics) {
            Gluon::Il2CppFunctions::class_is_generic(klass);
            Il2CppGenericClass *genericClass = klass->generic_class;

            if (genericClass) {

            }
        }

        // TODO
    }

    Il2CppClass *findNestedClass(Il2CppClass *declaring, std::string_view name) {
        if (!declaring) {
            return nullptr;
        }

        size_t token = name.find('/');
        size_t deeperNested = token != std::string::npos;

        std::string_view subTypeName = deeperNested ? name : name.substr(0, token);

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

    Il2CppClass *findClassFromName(std::string_view namespaze, std::string_view name) {
        Gluon::Il2CppFunctions::initialise();

        static std::unordered_map<std::pair<std::string, std::string>, Il2CppClass *, Gluon::Hashing::HashPair> classIndex;
        static std::mutex classIndexMutex;

        auto key = std::pair<std::string, std::string>(namespaze, name);

        classIndexMutex.lock();
        auto itr = classIndex.find(key);
        if (itr != classIndex.end()) {
            classIndexMutex.unlock();
            return itr->second;
        }
        classIndexMutex.unlock();

        const std::vector<const Il2CppAssembly *> assemblies = Gluon::Assemblies::getAssemblies();
        for (auto assembly : assemblies) {
            auto image = Gluon::Il2CppFunctions::assembly_get_image(assembly);
            if (!image) {
                Gluon::Logging::Logger::error("Assembly with name: {} has a null image!", assembly->aname.name);
                continue;
            }

            auto klass = Gluon::Il2CppFunctions::class_from_name(image, namespaze.data(), name.data());
            if (klass) {
                classIndexMutex.lock();
                classIndex.emplace(key, klass);
                classIndexMutex.unlock();
                return klass;
            }
        }

        // check for nested class
        auto token = name.find("/");
        bool nested = token != std::string::npos;
        if (nested) {
            auto declaringTypeName = std::string(name.substr(0, token));
            Il2CppClass *declaring = findClassFromName(namespaze, declaringTypeName);
            Il2CppClass *klass = findNestedClass(declaring, name.substr(token + 1));

            if (klass) {
                classIndexMutex.lock();
                classIndex.emplace(key, klass);
                classIndexMutex.unlock();

                return klass;
            }
        }

        Gluon::Logging::Logger::error("Could not find class with namespace: {} and name: {}", namespaze.data(), name.data());
        return nullptr;
    }
}