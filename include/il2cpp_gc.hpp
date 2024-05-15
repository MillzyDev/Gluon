#ifndef GLUON_IL2CPP_GC_HPP_
#define GLUON_IL2CPP_GC_HPP_

#include <cstddef>
#include <new>

namespace Gluon::Il2CppGc {

    /**
     * @brief @brief Returns an allocated instance of the provided size that will not be written over by future GC allocations and holds references.
     * You MUST use the gcAllocSpecific function defined here to destroy it.
     *
     * @param size The size to allocate an instance of.
     * @return The allocated instance.
     */
    [[nodiscard]] void *gcAllocSpecific(size_t size);

    /**
     * @brief Deletes the provided allocated instance from the gcAllocSpecific function defined here.
     * Other pointers will result in undefined behaviour.
     *
     * @param address The pointer to free explicitly.
     */
    void gcFreeSpecific(void *address) noexcept;

    /**
     * @brief Reallocation implementation is equivalent to: alloc + free
     * @param address The pointer to resize.
     * @param new_size The new size of the memory.
     * @return The resized instance.
     */
    [[nodiscard]] void *gcReallocSpecific(void *address, size_t new_size);

    template<class TValue>
    struct GcAllocator {
        constexpr GcAllocator() noexcept = default;

        template<class TCopyValue>
        constexpr explicit GcAllocator(const GcAllocator<TCopyValue> &) noexcept {}

        template<class TOtherValue>
        constexpr bool operator ==(const GcAllocator<TOtherValue> &) const noexcept {
            return true;
        }

        TValue *allocate(const size_t size) {
            if (size == 0) {
                return nullptr;
            }
            if (size > static_cast<size_t>(-1) / sizeof(TValue)) {
                throw std::bad_array_new_length();
            }
            void *const allocated = gcAllocSpecific(size * sizeof(TValue));
            if (!allocated) {
                throw std::bad_alloc();
            }
            return static_cast<TValue *>(allocated);
        }

        void deallocate(TValue *const address, size_t) const noexcept {
            gcFreeSpecific(address);
        }
    };
}

#endif // GLUON_IL2CPP_GC_HPP_