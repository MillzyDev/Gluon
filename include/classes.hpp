#ifndef GLUON_CLASSES_HPP_
#define GLUON_CLASSES_HPP_

#include <string_view>

#include "gluon_config.hpp"

#include "il2cpp-class-internals.h"

namespace Gluon::Classes {
    GLUON_API Il2CppClass *findClass(std::string_view namespaze, std::string_view klass);
} // Gluon::Classes

#endif // GLUON_CLASSES_HPP_
