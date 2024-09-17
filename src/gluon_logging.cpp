#include <memory>
#include <string>

#include "gluon_logging.hpp"

#include <windows.h>
#include <dbghelp.h>

namespace Gluon::Logging {
    void LoggerAdapter::logNeutral(const std::string &message) {}
    void LoggerAdapter::logInfo(const std::string &message) {}
    void LoggerAdapter::logWarn(const std::string &message) {}
    void LoggerAdapter::logError(const std::string &message) {}
    void LoggerAdapter::logDebug(const std::string &message) {}
    void LoggerAdapter::flushStreams() {}

    bool Logger::loggerAdapterReceived = false;
    std::unique_ptr<LoggerAdapter> Logger::loggerAdapter = nullptr;

    void Logger::setLoggerAdapter(std::unique_ptr<LoggerAdapter> implementation) {
        Logger::loggerAdapterReceived = true;
        Logger::loggerAdapter = std::move(implementation);
    }

    void Logger::logBacktraceFull(void *const *stackTraceBuffer, uint16_t stackTraceSize) {
        static HANDLE currentProcess = nullptr;

        if (!currentProcess) {
            DWORD symOptions = SymGetOptions();
            SymSetOptions(symOptions | SYMOPT_UNDNAME); // undecorated symbol names

            currentProcess = GetCurrentProcess();
            SymInitialize(currentProcess, nullptr, TRUE); // initialise for process and all modules
        }

        Gluon::Logging::Logger::error("[BACKTRACE BEGIN] Logging backtrace with size: {}...", stackTraceSize);
        Gluon::Logging::Logger::error("[BACKTRACE BEGIN]  *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***");
        Gluon::Logging::Logger::error("[BACKTRACE BEGIN] Process ID: {} Thread ID: {}", GetCurrentProcessId(), GetCurrentThreadId());

        for (uint16_t i = 0; i < stackTraceSize; i++) {
            constexpr std::size_t kSymbolInfoSize = sizeof(SYMBOL_INFO);
            constexpr std::size_t kFullSymbolInfoSize = kSymbolInfoSize + MAX_SYM_NAME * sizeof(TCHAR);

            // allocate space for symbol info
            auto symbolInfo = reinterpret_cast<PSYMBOL_INFO>(std::malloc(kFullSymbolInfoSize));
            std::memset(symbolInfo, 0, kFullSymbolInfoSize);
            symbolInfo->SizeOfStruct = kSymbolInfoSize;
            symbolInfo->MaxNameLen = MAX_SYM_NAME; // SymFromAddr requires this set

            if (SymFromAddr(currentProcess, reinterpret_cast<uint64_t>(stackTraceBuffer[i]), 0, symbolInfo)) {
                uint64_t address = reinterpret_cast<char *>(stackTraceBuffer[i]) - reinterpret_cast<char *>(symbolInfo->ModBase) - 4;

                // allocate space for module info
                constexpr std::size_t kModuleInfoSize = sizeof(IMAGEHLP_MODULE64);
                auto moduleInfo = reinterpret_cast<PIMAGEHLP_MODULE64>(std::malloc(kModuleInfoSize));
                std::memset(moduleInfo, 0, kModuleInfoSize);
                moduleInfo->SizeOfStruct = kModuleInfoSize; // SymGetModuleInfo functions require this set

                // attempt to get dll name
                std::string_view moduleName;
                if (SymGetModuleInfo64(currentProcess, reinterpret_cast<uint64_t>(stackTraceBuffer[i]), moduleInfo)) {
                    moduleName = std::string_view(moduleInfo->ImageName);
                }

                // if symbol name available, put it in the log.
                if (symbolInfo->NameLen) {
                    std::string_view symbolName(symbolInfo->Name, symbolInfo->NameLen);
                    Gluon::Logging::Logger::error("        #{:02}  PC {:016x}  {}  ({})", i, address, moduleName, symbolName);
                }
                else {
                    Gluon::Logging::Logger::error("        #{:02}  PC {:016x}  {}", i, address, moduleName);
                }

                std::free(moduleInfo);
            }
            std::free(symbolInfo);
        }
    }
}