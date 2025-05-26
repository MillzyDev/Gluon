#ifndef GLUON_PROPERTIES_HPP_
#define GLUON_PROPERTIES_HPP_

#include <optional>
#include <string_view>

#include "abortion.hpp"
#include "classes.hpp"
#include "gluon_config.hpp"
#include "methods.hpp"

#include "il2cpp-api-types.h"
#include "il2cpp-object-internals.h"

namespace Gluon::Properties {
    GLUON_API const PropertyInfo *findProperty(Il2CppClass *klass, std::string_view propertyName);
    GLUON_API const PropertyInfo *findProperty(std::string_view namespaze, std::string_view className, std::string_view propertyName);

    template <class T>
    const PropertyInfo *findProperty(T &&instance, std::string_view propertyName) {
        Il2CppClass *klass = Gluon::Classes::extractClass(instance);
        return Gluon::Properties::findProperty(klass, propertyName);
    }

    template <class TOut = Il2CppObject *, bool checkTypes = true, class T>
    std::optional<TOut> getPropertyValue(T &&classOrInstance, const PropertyInfo *property) {
        Gluon::Il2CppFunctions::initialise();
        const MethodInfo *getter = RET_NULLOPT_UNLESS(Gluon::Il2CppFunctions::property_get_get_method(property));
        return Gluon::Methods::runMethod<TOut, checkTypes>(classOrInstance, getter);
    }

    template <class TOut = Il2CppObject *, bool checkTypes = true, class T>
    std::optional<TOut> getPropertyValue(T &&classOrInstance, const std::string_view propertyName) {
        Gluon::Il2CppFunctions::initialise();
        const PropertyInfo *property = RET_NULLOPT_UNLESS(Gluon::Properties::findProperty(classOrInstance, propertyName));
        return Gluon::Properties::getPropertyValue<TOut, checkTypes>(classOrInstance, property);
    }

    template <class TOut = Il2CppObject *, bool checkTypes = true>
    std::optional<TOut> getPropertyValue(const std::string_view namespaze, const std::string_view className, const std::string_view propertyName) {
        Gluon::Il2CppFunctions::initialise();
        Il2CppClass *klass = RET_NULLOPT_UNLESS(Gluon::Classes::getClassFromName(namespaze, className));
        return Gluon::Properties::getPropertyValue<TOut, checkTypes>(klass, propertyName);
    }

    template <bool checkTypes = true, class T, class TArg>
    bool setPropertyValue(T &classOrInstance, const PropertyInfo *property, TArg &&value) {
        Gluon::Il2CppFunctions::initialise();
        RET_0_UNLESS(property);

        const MethodInfo *setter = RET_0_UNLESS(Gluon::Il2CppFunctions::property_get_set_method(property));
        return static_cast<bool>(Gluon::Methods::runMethod<Il2CppObject *, checkTypes>(classOrInstance, setter, value));
    }

    template <bool checkTypes = true, class T, class TArg>
    bool setPropertyValue(T &classOrInstance, const std::string_view propertyName, TArg &&value) {
        Gluon::Il2CppFunctions::initialise();
        const PropertyInfo *property = RET_0_UNLESS(Gluon::Properties::findProperty(classOrInstance, propertyName));
        return Gluon::Properties::setPropertyValue<checkTypes>(classOrInstance, property, value);
    }

    template <bool checkTypes = true, class TArg>
    bool setPropertyValue(const std::string_view namespaze, const std::string_view className, const std::string_view propertyName, TArg &&value) {
        Gluon::Il2CppFunctions::initialise();
        Il2CppClass *klass = RET_0_UNLESS(Gluon::Classes::getClassFromName(namespaze, className));
        return Gluon::Properties::setPropertyValue<checkTypes>(klass, propertyName, value);
    }
}

#endif // GLUON_PROPERTIES_HPP_