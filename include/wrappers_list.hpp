#ifndef GLUON_WRAPPERS_LIST_HPP_
#define GLUON_WRAPPERS_LIST_HPP_

#include <cstdint>
#include <optional>

#include "exceptions.hpp"
#include "il2cpp_functions.hpp"
#include "methods.hpp"
#include "type_check.hpp"
#include "type_concepts.hpp"
#include "wrappers_array.hpp"
#include "wrappers_object.hpp"

// if system list exists, we can use it in ListW, but with a compile definition
// it can be disabled
#if !defined(NO_CODEGEN_WRAPPERS) && __has_include("System/Collections/Generic/List_1.hpp")
namespace System::Collections::Generic {
    template <typename T>
    class List_1;
}

// include header
#include "System/Collections/Generic/List_1.hpp"
#endif

namespace Gluon {
    // System.Collections.Generic.List
    template <class T>
    struct List : Il2CppObject {
        Gluon::ArrayW<T> _items;
        int _size;
        int _version;
        Il2CppObject* _syncRoot;

        // Add an item to this list
        void Add(T item) {
            if (!static_cast<void*>(this)) throw Gluon::Exceptions::ListException(nullptr, "Running instance method on nullptr instance!");
            Gluon::Il2CppFunctions::initialise();

            auto klass = Gluon::Il2CppFunctions::object_get_class(this);
            auto* ___internal_method = THROW_UNLESS(
                Gluon::Methods::findMethod(
                    klass,
                    "Add",
                    ::std::array<const ::Il2CppClass*, 0>{},
                    ::std::array<const Il2CppType*, 1>{Gluon::TypeCheck::Il2CppNoArgType<T>::get()}
                )
            );
            Gluon::Methods::runMethodRethrow<void, false>(this, ___internal_method, item);
        }

        // Ensure the capacity of this list
        void EnsureCapacity(int min) {
            if (!static_cast<void*>(this)) throw Gluon::Exceptions::ListException(nullptr, "Running instance method on nullptr instance!");
            Gluon::Il2CppFunctions::initialise();

            auto klass = Gluon::Il2CppFunctions::object_get_class(this);
            auto* ___internal_method = THROW_UNLESS(
                Gluon::Methods::findMethod(
                    klass,
                    "EnsureCapacity",
                    std::array<Il2CppClass*, 0>{},
                    ::std::array<const Il2CppType*, 1>{Gluon::TypeCheck::Il2CppNoArgType<int32_t>::get()}
                )
            );
            Gluon::Methods::runMethodRethrow<void, false>(this, ___internal_method, min);
        }
    };
    DEFINE_IL2CPP_ARG_TYPE_GENERIC_CLASS(List, "System.Collections.Generic", "List`1");

    template <class T, class Ptr = List<T>*>
    struct ListWrapper {
        static_assert(sizeof(Ptr) == sizeof(void*), "Size of Ptr type must be the same as a void*!");
        using WrappedType = Ptr;

        constexpr ListWrapper(ListWrapper&&) noexcept = default;
        constexpr ListWrapper(ListWrapper const&) noexcept = default;

        constexpr ListWrapper() noexcept : ptr(nullptr) {}


        // TODO: Apply these il2cpp conversion changes to ArrayW as well, to permit
        // ArrayW to hold wrapper types and not pure pointers
        explicit constexpr ListWrapper(Ptr const& p) noexcept : ptr(p) {}
        explicit constexpr ListWrapper(Ptr&& p) noexcept : ptr(p) {}

        template <class V = void>
            requires(std::is_pointer_v<Ptr> && !Gluon::TypeConcepts::HasIl2CppConversion<Ptr>)
        explicit constexpr ListWrapper(void* alterInit) noexcept : ptr(reinterpret_cast<Ptr>(alterInit)) {}

        explicit constexpr ListWrapper(std::span<T> const p) : ptr(Gluon::Methods::newSpecific<Ptr>(p.size())) {
            std::copy(p.begin(), p.end(), begin());
        }

        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;

        using iterator = pointer;
        using const_iterator = const_pointer;

        [[nodiscard]] constexpr il2cpp_array_size_t size() const {
            return this->ptr->_size;
        }
        [[nodiscard]] constexpr Ptr getPtr() const {
            return this->ptr;
        }
        [[nodiscard]] constexpr Ptr getPtr() {
            return this->ptr;
        }
        T& operator[](size_t i) {
            return get_items()->_values[i];
        }
        const T& operator[](size_t i) const {
            return get_items()->_values[i];
        }

        /// @brief Get a given index, performs bound checking and throws
        /// std::runtime_error on failure.
        /// @param i The index to get.
        /// @return The reference to the item.
        T& get(size_t i) {
            THROW_UNLESS(i < size() && i >= 0);
            return get_items()->_values[i];
        }
        /// @brief Get a given index, performs bound checking and throws
        /// std::runtime_error on failure.
        /// @param i The index to get.
        /// @return The const reference to the item.
        const T& get(size_t i) const {
            THROW_UNLESS(i < size() && i >= 0);
            return get_items()->_values[i];
        }
        /// @brief Tries to get a given index, performs bound checking and returns a
        /// std::nullopt on failure.
        /// @param i The index to get.
        /// @return The WrapperRef<T> to the item, mostly considered to be a T&.
        std::optional<Gluon::WrapperRef<T>> try_get(size_t i) {
            if (i >= size() || i < 0) {
                return std::nullopt;
            }
            return WrapperRef(get_items()->_values[i]);
        }
        /// @brief Tries to get a given index, performs bound checking and returns a
        /// std::nullopt on failure.
        /// @param i The index to get.
        /// @return The WrapperRef<const T> to the item, mostly considered to be a
        /// const T&.
        std::optional<Gluon::WrapperRef<const T>> try_get(size_t i) const {
            if (i >= size() || i < 0) {
                return std::nullopt;
            }
            return WrapperRef(get_items()->_values[i]);
        }

        iterator begin() {
            return get_items()->_values;
        }
        iterator end() {
            return &get_items()->_values[size()];
        }
        const_iterator begin() const {
            return get_items()->_values;
        }
        const_iterator end() const {
            return &get_items()->_values[size()];
        }

        explicit operator std::span<T const> const() const {
            return std::span<T const>(this->begin(), this->end());
        }

        explicit operator std::span<T>() {
            return std::span<T>(this->begin(), this->end());
        }

        explicit operator Ptr() noexcept {
            return ptr;
        };

        constexpr void* convert() const noexcept {
            if constexpr (std::is_pointer_v<Ptr>) {
                return ptr;
            } else if constexpr (Gluon::TypeConcepts::HasIl2CppConversion<Ptr>) {
                return ptr.convert();
            }
        }

        Ptr operator->() noexcept {
            return ptr;
        }

        Ptr operator->() const noexcept {
            return ptr;
        }

        constexpr ListWrapper& operator=(ListWrapper&&) = default;
        constexpr ListWrapper& operator=(ListWrapper const&) = default;

        constexpr ListWrapper& operator=(Ptr const& ptr) {
            this->ptr = ptr;
            return *this;
        }
        constexpr ListWrapper& operator=(Ptr&& ptr) {
            this->ptr = ptr;
            return *this;
        }

        template <typename U>
        requires(std::is_convertible_v<U, T>)
        constexpr ListWrapper& operator=(ListWrapper<U> const& ptr) {
            this->ptr = ptr.ptr;
            return *this;
        }
        template <typename U>
        requires(std::is_convertible_v<U, T>)
        constexpr ListWrapper& operator=(ListWrapper<U>&& ptr) {
            this->ptr = ptr.ptr;
            return *this;
        }

        // method to create a new list easily
        template <Gluon::Methods::CreationType creationType = Gluon::Methods::CreationType::Temporary>
        static ListWrapper<T, Ptr> New() {
            Gluon::Il2CppFunctions::initialise();
            auto ls = Gluon::Methods::newObject<Ptr, creationType>();
            if (!ls) throw Gluon::Exceptions::ListException(nullptr, "Could not create list!");

            return { *ls };
        }

        // method to create a new list easily
        template <typename U = T, Gluon::Methods::CreationType creationType = Gluon::Methods::CreationType::Temporary>
        requires(std::is_convertible_v<U, T>)
        static ListWrapper<T, Ptr> New(std::initializer_list<U> values) {
            Gluon::Il2CppFunctions::initialise();
            auto ls = Gluon::Methods::newObject<Ptr, creationType>();
            if (!ls) throw Gluon::Exceptions::ListException(nullptr, "Could not create list!");

            ListWrapper<T, Ptr> lsWrap = { *ls };

            lsWrap.insert_range(values);

            return lsWrap;
        }
        template <typename U = T, Gluon::Methods::CreationType creationType = Gluon::Methods::CreationType::Temporary>
        requires(std::is_convertible_v<U, T>)
        static ListWrapper<T, Ptr> New(std::span<U const> const values) {
            Gluon::Il2CppFunctions::initialise();
            auto ls = Gluon::Methods::newObject<Ptr, creationType>();
            if (!ls) throw Gluon::Exceptions::ListException(nullptr, "Could not create list!");

            ListWrapper<T, Ptr> lsWrap = { *ls };

            lsWrap.insert_span(values);

            return lsWrap;
        }

        // method to create a new list easily
        template <Gluon::Methods::CreationType creationType = Gluon::Methods::CreationType::Temporary>
        static ListWrapper<T, Ptr> New(const il2cpp_array_size_t size) {
            Gluon::Il2CppFunctions::initialise();
            // type checking requires int, cannot convert ulong -> int
            auto ls = Gluon::Methods::newObject<Ptr, creationType>(static_cast<int>(size));
            if (!ls) throw Gluon::Exceptions::ListException(nullptr, "Could not create list!");

            ListWrapper<T, Ptr> lsWrap = { *ls };

            return lsWrap;
        }

        /**
         * @brief System.Collections.Generic::List<T>.IndexOf(T item)
         *
         * https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.indexof?view=net-8.0
         * @param index
         * @param item
         */
        std::optional<std::uint32_t> index_of(T const& item) const {
            auto start = this->begin();
            auto end = this->end();
            auto it = std::find(start, end, item);

            if (it == end) return std::nullopt;

            return std::distance(start, it);
        }

        constexpr bool empty() const {
            return this->size() == 0;
        }

        std::optional<std::reference_wrapper<T>> front() {
            if (this->empty()) return std::nullopt;

            return *begin();
        }
        std::optional<std::reference_wrapper<T>> back() {
            if (this->empty()) return std::nullopt;

            return *end();
        }

        std::optional<std::reference_wrapper<T const>> front() const {
            if (this->empty()) return std::nullopt;

            return *begin();
        }
        std::optional<std::reference_wrapper<T const>> back() const {
            if (this->empty()) return std::nullopt;

            return *end();
        }

        template <typename F>
        std::optional<std::reference_wrapper<T>> find(F&& func) {
            auto start = this->begin();
            auto end = this->end();
            auto it = std::find_if(start, end, std::forward<F>(func));

            if (it == end) return std::nullopt;

            return find(func);
        }

        template <typename F>
        std::optional<std::reference_wrapper<T const>> find(F&& func) const {
            auto start = this->begin();
            auto end = this->end();
            auto it = std::find_if(start, end, std::forward<F>(func));

            if (it == end) return std::nullopt;

            return *it;
        }

        template <typename F>
        std::optional<std::reference_wrapper<T>> reverse_find(F&& func) {
            auto start = this->begin();
            auto end = this->end();

            auto rev_start = std::make_reverse_iterator(start);
            auto rev_end = std::make_reverse_iterator(end);

            auto it = std::find_if(rev_start, rev_end, std::forward<F>(func));

            if (it == rev_end) return std::nullopt;

            return *it;
        }
        template <typename F>
        std::optional<std::reference_wrapper<T const>> reverse_find(F&& func) const {
            auto start = this->begin();
            auto end = this->end();

            auto rev_start = std::make_reverse_iterator(start);
            auto rev_end = std::make_reverse_iterator(end);

            auto it = std::find_if(rev_start, rev_end, std::forward<F>(func));

            if (it == rev_end) return std::nullopt;

            return *it;
        }

        /**
         * @brief System.Collections.Generic::List<T>.Clear()
         *
         * https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.clear?view=net-8.0
         * @param index
         * @param item
         */
        void clear() {
            ++this->ptr->_version;
            if constexpr (Gluon::TypeConcepts::Il2CppReferenceType<T>) {
                const int size = this->size();
                this->ptr->_size = 0;
                if (size > 0) {
                    std::fill(this->begin(), this->end(), T());
                    return;
                }
            } else {
                this->ptr->_size = 0;
            }
        }

        /**
         * @brief System.Collections.Generic::List<T>.Insert(int index, T item)
         *
         * https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.insert?view=net-8.0
         * @param index
         * @param item
         */
        void insert_at(il2cpp_array_size_t index, T item) {
            if (index > this->size()) {
                throw ListException(ptr, "Capacity size too small");
            }
            if (this->size() == this->get_items().size()) {
                this->EnsureCapacity(this->size() + 1);
            }
            if (index < this->size()) {
                std::copy(this->begin() + index, this->begin() + index + 1, this->end() - index);
                // Array.Copy(this._items, index, this._items, index + 1,
                //            this._size - index);
            }
            this->ptr->_items[index] = item;
            ++this->ptr->_size;
            ++this->ptr->_version;
        }
        /**
         * @brief System.Collections.Generic::List<T>.Add(T item)
         * https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.add?view=net-8.0
         *
         * @param item
         */
        void push_back(T item) {
            ++this->ptr->_version;
            auto items = this->get_items();
            auto size = this->size();
            if (size < items.size()) {
                this->ptr->_size = size + 1;
                items[size] = item;
                return;
            }
            this->AddWithResize(item);
        }

        template <typename... TArgs>
        requires(Gluon::TypeConcepts::Il2CppReferenceType<T>)
        void emplace_back(TArgs&&... args) {
            this->push_back(T::New_ctor(std::forward<TArgs>(args)...));
        }

        template <typename... TArgs>
        void emplace_back(TArgs&&... args) {
            this->push_back(T(std::forward<TArgs>(args)...));
        }

        /**
         * @brief Copies to a new array
         *
         * @return ArrayW<T>
         */
        Gluon::ArrayW<T> to_array() const {
            Gluon::ArrayW<T> newArr = Gluon::ArrayW<T>(this->size());
            std::copy(this->begin(), this->end(), newArr.begin());

            return newArr;
        }

        /**
         * @brief System.Collections.Generic::List<T>.Remove(T item)
         *
         * https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.remove?view=net-8.0
         * @param index
         * @param item
         */
        bool erase(T item) {
            auto index = index_of(item);
            if (!index.has_value()) {
                return false;
            }

            erase_at(index.value());
            return true;
        }

        /**
         * @brief System.Collections.Generic::List<T>.RemoveAt(int index)
         *
         * https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.removeat?view=net-8.0
         * @param index
         * @param item
         */
        void erase_at(il2cpp_array_size_t index) {
            if (index >= this->size()) {
                throw ListException(ptr, "index is over size bounds");
            }
            --this->ptr->_size;
            if (index < this->size()) {
                std::copy(this->begin() + 1, this->end(), this->end() - index);
                // Array.Copy(this._items, index + 1, this._items, index, this._size - index);
            }
            if constexpr (Gluon::TypeConcepts::Il2CppReferenceType<T>) {
                ptr->_items[this->size()] = T();
            }
            ++this->ptr->_version;
        }

        /**
         * @brief System.Collections.Generic::List<T>.RemoveRange(int index, int count)
         *
         * https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.removerange?view=net-8.0
         * @param index
         * @param item
         */
        void erase_range(int index, int count) {
            if (index < 0) {
                throw ListException(ptr, "index is less than 0");
            }
            if (count < 0) {
                throw ListException(ptr, "count is less than 0");
            }
            if (this->size() - index < count) {
                throw ListException(ptr, "count is over bounds");
            }
            if (count <= 0) {
                return;
            }
            // int size = this->size();
            ptr->_size -= count;
            if (index < this->size()) {
                std::copy(this->begin() + index + count, this->end() - index, this->begin() + index);
                // Array.Copy(this._items, index + count, this._items, index, this._size - index);
            }
            ++ptr->_version;
            if constexpr (Gluon::TypeConcepts::Il2CppReferenceType<T>) {
                std::fill(this->end(), this->end() + count, T());
                // Array.Clear(this._items, this._size, count);
            }
        }

        /**
         * @brief Adds the collection to the end of the List. Ensures capacity is appropiate
         *
         * System.Collections.Generic::List<T>.AddRange(Enumerable<T> enumerable)
         * https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.addrange?view=net-8.0
         *
         * @tparam It Iterator type
         * @param begin
         * @param end
         */
        template <typename It>
        void insert_range(It begin, It end) {
            insert_span(std::span<T const>(begin, end));
        }

        /**
         * @brief Adds the collection to the end of the List. Ensures capacity is appropiate
         *
         * System.Collections.Generic::List<T>.AddRange(Enumerable<T> enumerable)
         * https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.addrange?view=net-8.0
         *
         * @tparam It
         * @param begin
         * @param count amount of items
         */
        template <typename It>
        void insert_range(It begin, int count) {
            insert_span(std::span<T const>(begin, count));
        }

        /**
         * @brief Adds the collection to the end of the List. Ensures capacity is appropiate
         *
         * System.Collections.Generic::List<T>.AddRange(Enumerable<T> enumerable)
         * https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.addrange?view=net-8.0
         *
         * @tparam C
         * @param container
         */
        template <typename C>
        void insert_range(C container) {
            insert_span(std::span<T const>(container.begin(), container.end()));
        }

        /**
         * @brief Adds the collection to the end of the List. Ensures capacity is appropiate
         *
         * System.Collections.Generic::List<T>.AddRange(Enumerable<T> enumerable)
         * https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.addrange?view=net-8.0
         *
         * @param span
         */
        void insert_span(std::span<T const> span) {
            if (span.empty()) return;

            this->EnsureCapacity(span.size() + this->size());
            auto start = this->end();
            std::copy(span.begin(), span.end(), start);

            ptr->_size += span.size();
            ++ptr->_version;
        }

        /// @brief Provides a reference span of the held data within this array. The span should NOT outlive this instance.
        /// @return The created span.
        std::span<T> ref_to() {
            return std::span(begin(), end());
        }

        /// @brief Provides a reference span of the held data within this array. The span should NOT outlive this instance.
        /// @return The created span.
        std::span<T> ref_to() const {
            return std::span(begin(), end());
        }

    private:
        auto const& get_items() const {
            return ptr->_items;
        }
        auto& get_items() {
            return ptr->_items;
        }

        auto as_il2cpp_list() {
            return reinterpret_cast<List<T>*>(ptr);
        }

    protected:
        void SetCapacity(il2cpp_array_size_t value) {
            if (value < this->size()) {
                throw ListException(ptr, "Capacity size too small");
            }
            if (value != this->get_items().size()) {
                if (value > 0) {
                    auto array = Gluon::ArrayW<T>(value);
                    if (this->size() > 0) {
                        std::copy(this->begin(), this->end(), array.begin());
                    }
                    ptr->_items = array;
                    return;
                }
                ptr->_items = Gluon::ArrayW<T>(static_cast<il2cpp_array_size_t>(0));
            }
        }

        void AddWithResize(T item) {
            auto size = this->size();
            this->EnsureCapacity(size + 1);
            ptr->_size = size + 1;
            get_items()[size] = item;
        }

        void EnsureCapacity(il2cpp_array_size_t min) {
            if (get_items().size() < min) {
                auto num = (get_items().size() == 0) ? 4 : (get_items().size() * 2);
                if (num > 2146435071) {
                    num = 2146435071;
                }
                if (num < min) {
                    num = min;
                }
                SetCapacity(num);
            }
        }

        Ptr ptr;
    };

    template <typename T, template <typename U = T> typename Wrapper>
    ListWrapper(Wrapper<T>) -> ListWrapper<T, Wrapper<T>>;

    // if system list exists, we can use it in ListW, but with a compile definition
    // it can be disabled
#if !defined(NO_CODEGEN_WRAPPERS) && __has_include("System/Collections/Generic/List_1.hpp")
    // forward declare usage
    template <typename T, typename Ptr = System::Collections::Generic::List_1<T>*>
    using ListW = ListWrapper<T, Ptr>;
#else
    template <typename T, typename Ptr = Gluon::List<T>*>
    using ListW = Gluon::ListWrapper<T, Ptr>;
#endif
} // Gluon

MARK_GEN_REF_T(Gluon::ListWrapper);
MARK_GEN_REF_PTR_T(Gluon::List);

static_assert(Gluon::TypeConcepts::HasIl2CppConversion<Gluon::ListWrapper<int, Gluon::List<int>*>>);
template <class T, class Ptr>
struct GLUON_HIDDEN Gluon::TypeCheck::NeedBox<Gluon::ListWrapper<T, Ptr>> {
    constexpr static bool value = false;
};

static_assert(Gluon::TypeConcepts::HasIl2CppConversion<Gluon::ListW<int>>);

template <typename T, typename Ptr>
auto format_as(Gluon::ListWrapper<T, Ptr> list) {
    if (!list) return std::string("null");
    std::stringstream s;
    std::copy(list.begin(), list.end(), std::ostream_iterator<int>(s, ", "));
    return s.str();
}

#endif // GLUON_WRAPPERS_LIST_HPP_