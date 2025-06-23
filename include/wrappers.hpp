#ifndef GLUON_WRAPPERS_HPP_
#define GLUON_WRAPPERS_HPP_

#include <cassert>
#include <cstdint>
#include <type_traits>
#include <initializer_list>

#include "il2cpp_functions.hpp"
#include "methods.hpp"
#include "type_check.hpp"
#include "wrapper_base.hpp"
#include "wrappers_array.hpp"
#include "wrappers_delegate.hpp"
#include "wrappers_string.hpp"
#include "wrappers_list.hpp"
#include "wrappers_pointer.hpp"

#include "il2cpp-config.h"
#include "il2cpp-api-types.h"
#include "il2cpp-class-internals.h"
#include "il2cpp-object-internals.h"
#include "il2cpp-tabledefs.h"

namespace Gluon {
    template <class T>
    struct Array;

    template <typename T, typename Ptr>
    struct ListWrapper;

    template <typename Ptr>
    struct StringWrapper;
} // Gluon

DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppMulticastDelegate*, multicastdelegate);
NEED_NO_BOX(Il2CppMulticastDelegate);

DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppAsyncCall*, async_call);
NEED_NO_BOX(Il2CppAsyncCall);

DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppInternalThread*, internal_thread);
NEED_NO_BOX(Il2CppInternalThread);

DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppReflectionEvent*, event_info);
NEED_NO_BOX(Il2CppReflectionEvent);

DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppStringBuilder*, stringbuilder);
NEED_NO_BOX(Il2CppStringBuilder);

DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppStackFrame*, stack_frame);
NEED_NO_BOX(Il2CppStackFrame);

DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppReflectionAssemblyName*, assembly_name);
NEED_NO_BOX(Il2CppReflectionAssemblyName);

DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppReflectionModule*, module);
NEED_NO_BOX(Il2CppReflectionModule);

DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppSystemException*, system_exception);
NEED_NO_BOX(Il2CppSystemException);

DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppArgumentException*, argument_exception);
NEED_NO_BOX(Il2CppArgumentException);

DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppMarshalByRefObject*, marshalbyrefobject);
NEED_NO_BOX(Il2CppMarshalByRefObject);

DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppSafeHandle*, safe_handle);
NEED_NO_BOX(Il2CppSafeHandle);

DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppSortKey*, sort_key);
NEED_NO_BOX(Il2CppSortKey);

DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppErrorWrapper*, error_wrapper);
NEED_NO_BOX(Il2CppErrorWrapper);

DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppComObject*, il2cpp_com_object);
NEED_NO_BOX(Il2CppComObject);

DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppTypedRef, typed_reference);

DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppDelegate*, delegate);
NEED_NO_BOX(Il2CppDelegate);

DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppReflectionMonoType*, monotype);
NEED_NO_BOX(Il2CppReflectionMonoType);

DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppThread*, thread);
NEED_NO_BOX(Il2CppThread);

DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppReflectionRuntimeType*, runtimetype);
NEED_NO_BOX(Il2CppReflectionRuntimeType);

DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppException*, exception);
NEED_NO_BOX(Il2CppException);

DEFINE_IL2CPP_ARG_TYPE(long double, "System", "Decimal");

template <class T>
struct Gluon::TypeCheck::Il2CppNoArgClass<Gluon::ListW<T>> {
    static inline Il2CppClass *get() {
        Il2CppClass *klass = Gluon::TypeCheck::Il2CppNoArgClass<typename Gluon::ListW<T>::WrappedType>::get();
        return klass;
    }
};

struct Il2CppImageGlobalMetadata {
    TypeDefinitionIndex typeStart;
    TypeDefinitionIndex exportedTypeStart;
    CustomAttributeIndex customAttributeStart;
    MethodIndex entryPointIndex;
    const Il2CppImage* image;
};

#endif // GLUON_WRAPPERS_HPP_