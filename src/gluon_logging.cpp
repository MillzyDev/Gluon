#include "gluon_logging.hpp"

#include <memory>
#include <string>

namespace Gluon::Logging {
    void LoggerAdapter::logNeutral(const std::string &message) {}
    void LoggerAdapter::logInfo(const std::string &message) {}
    void LoggerAdapter::logWarn(const std::string &message) {}
    void LoggerAdapter::logError(const std::string &message) {}

    bool Logger::loggerAdapterReceived = false;
    std::unique_ptr<LoggerAdapter> Logger::loggerAdapter = nullptr;

    void Logger::setLoggerAdapter(std::unique_ptr<LoggerAdapter> implementation) {
        Logger::loggerAdapterReceived = true;
        Logger::loggerAdapter = std::move(implementation);
    }
}