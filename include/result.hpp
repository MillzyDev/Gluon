#ifndef GLUON_RESULT_HPP_
#define GLUON_RESULT_HPP_

#include <optional>
#include <type_traits>
#include <variant>

#include "exceptions.hpp"

namespace Gluon {

    /**
     * Converts void to monostate for variant
     */
    template <typename T>
    using TypeOrMonostate = std::conditional_t<std::is_same_v<T, void>, std::monostate, T>;

    template <typename T, typename E>
    requires(!std::is_same_v<E, void>)
    struct Result {
        using SuccessValue = TypeOrMonostate<T>;
        using ExceptionValue = E;

        bool success;

        union {
             SuccessValue result;
             ExceptionValue exception;
        };

        Result()
            requires(std::is_default_constructible_v<SuccessValue> || std::is_void_v<T>)
            : Result(SuccessValue()) {}

        Result(Result &&) noexcept = default;
        Result(const Result &) noexcept = default;

        Result(SuccessValue &&result) noexcept : success(true), result(std::forward<SuccessValue>(result)) {}

        Result(const SuccessValue &result) noexcept : success(true), result(std::forward<SuccessValue>(result)) {}

        /**
         * Exception result
         */
         Result(E &&exception) noexcept : success(false), exception(std::forward<E>(exception)) {}
         Result(const E &exception) noexcept : success(false), exception(std::forward<E>(exception)) {}

         ~Result() {
             if (success) {
                 this->result.~SuccessValue();
             }
             else {
                 this->exception.~ExceptionValue();
             }
         }

         [[nodiscard]] inline bool hasResult() const noexcept {
             return success;
         }

         [[nodiscard]] inline bool hasException() const noexcept {
             return !success;
         }

         [[nodiscard]] constexpr SuccessValue const &getResult() const {
             if (!success) {
                 throw Gluon::Exceptions::ResultException("Result does not contain a success result!");
             }

             return result;
         }

         [[nodiscard]] constexpr SuccessValue &getResult() {
             if (!success) {
                 throw Gluon::Exceptions::ResultException("Result does not contain a success result!");
             }

             return result;
         }

         // move result value out of this wrapper
         [[nodiscard]] constexpr SuccessValue moveResult() {
             if (!success) {
                 throw Gluon::Exceptions::ResultException("Result does not contain a success result!");
             }

             return std::move(result);
         }

         [[nodiscard]] constexpr const ExceptionValue &getException() const {
             if (success) {
                 throw Gluon::Exceptions::ResultException("Result does not contain an exception result!");
             }

             return exception;
         }

         [[nodiscard]] constexpr ExceptionValue &getException() {
             if (success) {
                 throw Gluon::Exceptions::ResultException("Result does not contain an exception result!");
             }

             return exception;
         }

         // move result value out of this wrapper
         [[nodiscard]] constexpr ExceptionValue moveException() {
             if (success) {
                 throw Gluon::Exceptions::ResultException("Result does not contain an exception result!");
             }

             return exception;
         }

         [[nodiscard]] constexpr const SuccessValue *getOrRethrow() const {
             if (!success) {
                 throw this->getException();
             }

             return this->getResult();
         }

         [[nodiscard]] constexpr SuccessValue &getOrRethrow() {
             if (!success) {
                 throw this->getException();
             }

             return this->getResult();
         }

         constexpr void rethrow() const {
             if (success) {
                 return;
             }

             throw this->getException();
         }

        /**
         * Moves this result into a variant
         * @return The moved result, is std::monostate if T is void
         */
         [[nodiscard]] constexpr std::variant<SuccessValue, ExceptionValue> intoVariant() noexcept {
             if (success) {
                 return this->moveResult();
             }

             return this->moveException();
         }

         /**
          * Moves this result into an optional
          * @return The moved result, is std::monostate if T is void
          */
         [[nodiscard]] constexpr std::optional<SuccessValue> intoOptionalResult() noexcept {
             if (success) {
                 return this->moveResult();
             }

             return std::nullopt;
         }

         [[nodiscard]] constexpr std::optional<SuccessValue *> asOptionalResult() noexcept {
             if (success) {
                 return &this->getResult();
             }

             return std::nullopt;
         }

         [[nodiscard]] constexpr std::optional<const SuccessValue *> asOptionalResult() const noexcept {
             if (success) {
                 return &this->getResult();
             }

             return std::nullopt;
         };

         [[nodiscard]] constexpr std::optional<E> intoOptionalException() noexcept {
             if (!success) {
                 return this->moveException();
             }

             return std::nullopt;
         }

        [[nodiscard]] constexpr std::optional<E *> asOptionalException()  noexcept {
            if (!success) {
                return &this->getException();
            }

            return std::nullopt;
        }

         [[nodiscard]] constexpr std::optional<const E *> asOptionalException() const noexcept {
             if (!success) {
                 return &this->getException();
             }

             return std::nullopt;
         }
    };
}

#endif // GLUON_RESULT_HPP_