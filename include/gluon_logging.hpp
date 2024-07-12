#ifndef GLUON_GLUON_LOGGING_HPP_
#define GLUON_GLUON_LOGGING_HPP_

#include <format>
#include <memory>
#include <string>

#include "gluon_config.hpp"

namespace Gluon::Logging {
    class GLUON_API LoggerAdapter {
    public:
        virtual void logNeutral(const std::string &message);
        virtual void logInfo(const std::string &message);
        virtual void logWarn(const std::string &message);
        virtual void logError(const std::string &message);
    };

    class GLUON_API Logger {
    private:
        static bool loggerAdapterReceived;
        static std::unique_ptr<LoggerAdapter> loggerAdapter;

    public:
        static void setLoggerAdapter(std::unique_ptr<LoggerAdapter> implementation);

        static void logBacktraceFull(void *const *stackTraceBuffer, uint16_t stackTraceSize);

        template<typename... TArgs>
        inline static void neutral(std::format_string<TArgs...> format, TArgs &&...args) {
            if (!Logger::loggerAdapterReceived) {
                return;
            }
            Logger::loggerAdapter->logNeutral(std::vformat(format.get(), std::make_format_args(args...)));
        }

        template<typename... TArgs>
        inline static void info(std::format_string<TArgs...> format, TArgs &&...args) {
            if (!Logger::loggerAdapterReceived) {
                return;
            }
            Logger::loggerAdapter->logInfo(std::vformat(format.get(), std::make_format_args(args...)));
        }

        template<typename... TArgs>
        inline static void warn(std::format_string<TArgs...> format, TArgs &&...args) {
            if (!Logger::loggerAdapterReceived) {
                return;
            }
            Logger::loggerAdapter->logWarn(std::vformat(format.get(), std::make_format_args(args...)));
        }

        template<typename... TArgs>
        inline static void error(std::format_string<TArgs...> format, TArgs &&...args) {
            if (!Logger::loggerAdapterReceived) {
                return;
            }
            Logger::loggerAdapter->logError(std::vformat(format.get(), std::make_format_args(args...)));
        }
    };
}

namespace Gluon {
    typedef ::Gluon::Logging::Logger Logger;
}

#endif