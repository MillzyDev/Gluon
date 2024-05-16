#include <cstddef>
#include <cstring>

#include "gc.hpp"
#include "il2cpp_functions.hpp"

namespace Gluon::Gc {
    [[nodiscard]] void *gcAllocSpecific(size_t size) {
        void *address = Gluon::Il2CppFunctions::gc_alloc_fixed(size);
        return address;
    }

    [[nodiscard]] void *gcReallocSpecific(void *originalAddress, size_t newSize) {
        void *newAddress = gcAllocSpecific(newSize);
        std::memcpy(newAddress, originalAddress, newSize);
        gcFreeSpecific(originalAddress);
        return newAddress;
    }

    void gcFreeSpecific(void *address) noexcept {
        Gluon::Il2CppFunctions::gc_free_fixed(address);
    }
}