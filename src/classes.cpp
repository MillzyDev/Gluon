#include <sstream>
#include <string>
#include <string_view>

#include "classes.hpp"

#include "gluon_logging.hpp"
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
                const char *typeName = Try
            }
        }
    }

    std::string getClassStandardName(const Il2CppClass *klass, bool generics) {
        Gluon::Il2CppFunctions::Init();

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
    }
}