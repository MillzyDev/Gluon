#include <windows.h>
#include <dbghelp.h>

#include "gluon_logging.hpp"

namespace Gluon {
    std::unique_ptr<LoggerAdapter> Gluon::Logger::_loggerAdapter = nullptr;

    void LoggerAdapter::log(std::string_view fmt, std::format_args args) {}
    void LoggerAdapter::logInfo(std::string_view fmt, std::format_args args) {}
    void LoggerAdapter::logWarning(std::string_view fmt, std::format_args args) {}
    void LoggerAdapter::logError(std::string_view fmt, std::format_args args) {}
    void LoggerAdapter::logDebug(std::string_view fmt, std::format_args args) {}

    void Logger::logBacktraceFull(void *const *stackTraceBuffer, uint16_t stackTraceSize) {
        static HANDLE currentProcess = nullptr;

        if (!currentProcess) {
            DWORD symOptions = SymGetOptions();
            SymSetOptions(symOptions | SYMOPT_UNDNAME); // undecorated symbol names

            currentProcess = GetCurrentProcess();
            SymInitialize(currentProcess, nullptr, TRUE); // initialise for process and all modules
        }

        Gluon::Logger::error("[BACKTRACE BEGIN] Logging backtrace with size: {}...", stackTraceSize);
        Gluon::Logger::error("[BACKTRACE BEGIN]  *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***");
        Gluon::Logger::error("[BACKTRACE BEGIN] Process ID: {} Thread ID: {}", GetCurrentProcessId(), GetCurrentThreadId());

        for (uint16_t i = 0; i < stackTraceSize; i++) {
            constexpr std::size_t kSymbolInfoSize = sizeof(SYMBOL_INFO);
            constexpr std::size_t kFullSymbolInfoSize = kSymbolInfoSize + MAX_SYM_NAME * sizeof(TCHAR);

            // allocate space for symbol info
            auto symbolInfo = static_cast<PSYMBOL_INFO>(std::malloc(kFullSymbolInfoSize));
            std::memset(symbolInfo, 0, kFullSymbolInfoSize);
            symbolInfo->SizeOfStruct = kSymbolInfoSize;
            symbolInfo->MaxNameLen = MAX_SYM_NAME; // SymFromAddr requires this set

            if (SymFromAddr(currentProcess, reinterpret_cast<uint64_t>(stackTraceBuffer[i]), nullptr, symbolInfo)) {
                uint64_t address = static_cast<char *>(stackTraceBuffer[i]) - reinterpret_cast<char *>(symbolInfo->ModBase) - 4;

                // allocate space for module info
                constexpr std::size_t kModuleInfoSize = sizeof(IMAGEHLP_MODULE64);
                auto moduleInfo = static_cast<PIMAGEHLP_MODULE64>(std::malloc(kModuleInfoSize));
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
                    Gluon::Logger::error("        #{:02}  PC {:016x}  {}  ({})", i, address, moduleName, symbolName);
                }
                else {
                    Gluon::Logger::error("        #{:02}  PC {:016x}  {}", i, address, moduleName);
                }

                std::free(moduleInfo);
            }
            std::free(symbolInfo);
        }
    }

    void Logger::init(std::unique_ptr<LoggerAdapter> impl) {
        Gluon::Logger::_loggerAdapter = std::move(impl);
    }
} // Gluon