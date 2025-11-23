#ifndef GLUON_LOGGING_HPP_
#define GLUON_LOGGING_HPP_

#include <format>
#include <memory>
#include <string>

#include "gluon_config.hpp"

#define FORMAT_LOGGER(name, adapter, func)                                                                              \
    template<typename ...TArgs>                                                                                         \
    inline static void name(::std::format_string<TArgs...> format, TArgs &&...args) {                              \
        if (!adapter) {                                                                                                 \
            return;                                                                                                     \
        }                                                                                                               \
        adapter->func(::std::vformat(format.get(), ::std::make_format_args(args...)));                                  \
    }

namespace Gluon {
    /**
     *  Interface for feeding Gluon's log into another logging system.
     *  Recommended implementation be done though some form of blocking queue.
     */
    // ReSharper disable once CppClassCanBeFinal
    class GLUON_API LoggerAdapter {
    public:
        virtual ~LoggerAdapter() = default;

        virtual void log(const std::string &message);
        virtual void logInfo(const std::string &message);
        virtual void logWarning(const std::string &message);
        virtual void logError(const std::string &message);
        virtual void logDebug(const std::string &message);
    }; // LoggerAdapter

    class GLUON_API Logger {
    private:
        static std::unique_ptr<LoggerAdapter> _loggerAdapter;

    public:
        static void logBacktraceFull(void *const *stackTraceBuffer, uint16_t stackTraceSize);
        static void init(std::unique_ptr<LoggerAdapter> impl);

        FORMAT_LOGGER(log, Logger::_loggerAdapter, log)
        FORMAT_LOGGER(info, Logger::_loggerAdapter, logInfo)
        FORMAT_LOGGER(warn, Logger::_loggerAdapter, logWarning)
        FORMAT_LOGGER(error, Logger::_loggerAdapter, logError)
        FORMAT_LOGGER(debug, Logger::_loggerAdapter, logDebug)
    }; // Logger

    /**
     * Initialises Gluon's logger interface
     * @param impl The implementation of Gluon's logger adapter
     */
    inline void initLogger(std::unique_ptr<LoggerAdapter> impl) {
        Gluon::Logger::init(std::move(impl));
    }
} // Gluon

#endif // GLUON_LOGGING_HPP_