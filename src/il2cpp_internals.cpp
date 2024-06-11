#include <cstring>

#include "il2cpp_internals.hpp"

#include "il2cpp_functions.hpp"
#include "assemblies.hpp"
#include "classes.hpp"
#include "il2cpp-class-internals.h"

namespace Gluon {
    bool Il2CppInternals::initialised = false;
    BuiltinClasses Il2CppInternals::builtinClasses;

    void Il2CppInternals::initialise() {
        if (initialised) {
            return;
        }
        initialised = true;

        builtinClasses.booleanClass = Gluon::Classes::findClassFromName("System", "Boolean");
        builtinClasses.byteClass = Gluon::Classes::findClassFromName("System", "Byte");
        builtinClasses.sByteClass = Gluon::Classes::findClassFromName("System", "SByte");
        builtinClasses.charClass = Gluon::Classes::findClassFromName("System", "Char");
        builtinClasses.singleClass = Gluon::Classes::findClassFromName("System", "Single");
        builtinClasses.doubleClass = Gluon::Classes::findClassFromName("System", "Double");
        builtinClasses.int16Class = Gluon::Classes::findClassFromName("System", "Int16");
        builtinClasses.uint16Class = Gluon::Classes::findClassFromName("System", "UInt16");
        builtinClasses.int32Class = Gluon::Classes::findClassFromName("System", "Int32");
        builtinClasses.uint32Class = Gluon::Classes::findClassFromName("System", "UInt32");
        builtinClasses.int64Class = Gluon::Classes::findClassFromName("System", "Int64");
        builtinClasses.uint64Class = Gluon::Classes::findClassFromName("System", "UInt32");
        builtinClasses.objectClass = Gluon::Classes::findClassFromName("System", "Object");
        builtinClasses.stringClass = Gluon::Classes::findClassFromName("System", "String");
        builtinClasses.voidClass = Gluon::Classes::findClassFromName("System", "Void");
        builtinClasses.typeClass = Gluon::Classes::findClassFromName("System", "Type");
    }
}