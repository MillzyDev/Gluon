#ifndef GLUON_ARRAYS_HPP_
#define GLUON_ARRAYS_HPP_

#include <cstdint>

#include "il2cpp-object-internals.h"

#define IL2CPP_ARRAY_ADDR(array, type, index) reinterpret_cast<type *>(Gluon::Arrays::il2cppArrayAddrWithSize(array, sizeof(type), index)

#define IL2CPP_ARRAY_GET(array, type, index) (*reinterpret_cast<type *>(IL2CPP_ARRAY_ADDR((array), type, (index)))

#define IL2CPP_ARRAY_SET(array, type, index, value) {                                    \
    type *__ptr = reinterpret_cast<type *>(IL2CPP_ARRAY_ADDR((array), type, (index)));   \
    *__ptr = (value);                                                                    \
}

#define IL2CPP_ARRAY_SETREF(array, index, value) {                                            \
    void **__ptr = reinterpret_cast<void **>(IL2CPP_ARRAY_ADDR((array), void *, (index)));    \
    *__ptr = (value);                                                                         \
}

namespace Gluon::Arrays {
    static char *il2cppArrayAddrWithSize(Il2CppArray *array, std::int32_t size, std::uintptr_t index) {
        return reinterpret_cast<char *>(array) + kIl2CppSizeOfArray + size * index;
    }
}

#endif // GLUON_ARRAYS_HPP_