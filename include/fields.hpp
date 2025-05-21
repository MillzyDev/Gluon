#ifndef GLUON_FIELDS_HPP_
#define GLUON_FIELDS_HPP_

#include <concepts>
#include <methods.hpp>
#include <optional>
#include <string_view>
#include <type_concepts.hpp>
#include <type_traits>

#include "abortion.hpp"
#include "classes.hpp"
#include "gluon_logging.hpp"
#include "il2cpp_functions.hpp"

#include "il2cpp-api-types.h"

namespace Gluon::Fields {
    GLUON_API Il2CppClass *getFieldClass(FieldInfo *field);

    GLUON_API FieldInfo *findField(Il2CppClass *klass, std::string_view fieldName);

    template <class ...TArgs>
    FieldInfo *findField(const std::string_view namespaze, const std::string_view className, TArgs &&...params) {
        return Gluon::Fields::findField(Gluon::Classes::getClassFromName(namespaze, className), params...);
    }

    template <class T, class ...TArgs>
    requires (!std::is_convertible_v<T, Il2CppClass *> && !std::is_convertible_v<T, std::string_view>)
    FieldInfo *findField(T &&instance, TArgs &&...params) {
        Gluon::Il2CppFunctions::initialise();
        Il2CppClass *klass = RET_0_UNLESS(Gluon::Classes::extractClass(instance));
        return Gluon::Fields::findField(klass, params...);
    }

    template <typename TOut = Il2CppObject *>
    std::optional<TOut> getFieldValue(Il2CppObject *instance, FieldInfo *field) {
        Gluon::Il2CppFunctions::initialise();
        RET_NULLOPT_UNLESS(field);

        const Il2CppType *outType = Gluon::Classes::extractIndependentType<TOut>();
        if (!outType && !Gluon::Methods::isConvertibleFrom(outType, field->type, false)) {
            Gluon::Logger::warn("Requested TOut {} does not match field type {}",
                Gluon::Classes::getTypeSimpleName(outType), Gluon::Classes::getTypeSimpleName(field->type));
        }

        TOut out;
        if (instance) {
            Gluon::Il2CppFunctions::field_get_value(instance, field, &out);
        }
        else { // probably a static field
            Gluon::Il2CppFunctions::field_static_get_value(field, &out);
        }

        return out;
    }

    template <typename TOut = Il2CppObject *, typename T>
    std::optional<TOut> getFieldValue(T &&classOrInstance, const std::string_view fieldName) {
        FieldInfo *field = RET_NULLOPT_UNLESS(Gluon::Fields::findField(classOrInstance, fieldName));
        Il2CppObject *object = Gluon::Methods::toIl2CppObject(classOrInstance);
        return Gluon::Fields::getFieldValue<TOut>(object, field);
    }

    // Static fields only
    template <typename TOut = Il2CppObject *>
    std::optional<TOut> getFieldValue(const std::string_view namespaze, const std::string_view className, const std::string_view fieldName) {
        Il2CppClass *klass = RET_NULLOPT_UNLESS(Gluon::Classes::getClassFromName(namespaze, className));
        return Gluon::Fields::getFieldValue(klass, fieldName);
    }

    template <class TArg>
    bool setFieldValue(Il2CppObject *instance, FieldInfo *field, TArg &&value) {
        Gluon::Il2CppFunctions::initialise();
        RET_0_UNLESS(field);

        // Ensure supplied value matches field's type
        const Il2CppType *type = Gluon::Classes::extractType(value);
        RET_0_UNLESS(Gluon::Methods::isConvertibleFrom(Gluon::Il2CppFunctions::field_get_type(field), type));

        void *il2cppValue = Gluon::Methods::extractValue(value);

        if (!instance) {
            Gluon::Il2CppFunctions::field_set_value(instance, field, il2cppValue);
        }
        else { // probably a static field
            Gluon::Il2CppFunctions::field_static_set_value(field, il2cppValue);
        }

        return true;
    }

    template <class T, class TArg>
    bool setFieldValue(T &classOrInstance, std::string_view fieldName, TArg &&value) {
        FieldInfo *field = RET_0_UNLESS(Gluon::Fields::findField(classOrInstance, fieldName));
        Il2CppObject *object = Gluon::Methods::toIl2CppObject(classOrInstance);
        RET_0_UNLESS(Gluon::Fields::setFieldValue(object, field, value));

        if (object) {
            RET_0_UNLESS(Gluon::Classes::fromIl2CppObject(object, classOrInstance));
        }

        return true;
    }

    template <class TArg>
    bool setFieldValue(std::string_view namespaze, std::string_view className, std::string_view fieldName, TArg &&value) {
        Il2CppClass *klass = RET_0_UNLESS(Gluon::Classes::getClassFromName(namespaze, className));
        return setFieldValue(klass, fieldName, value);
    }
}

#endif // GLUON_FIELDS_HPP_