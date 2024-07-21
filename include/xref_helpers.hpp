#ifndef GLUON_XREF_HELPERS_HPP_
#define GLUON_XREF_HELPERS_HPP_

#include <cstdint>
#include <optional>

#include "gluon_config.hpp"
#include "gluon_logging.hpp"
#include "capstone/capstone.h"

// words cannot describe, how much i hated writing/porting this xref stuff

namespace Gluon::XrefHelpers {
    GLUON_API void initialiseCapstone();
    GLUON_API csh getCapstoneHandle();

    template<x86_insn... args>
    constexpr bool insnMatch(cs_insn *insn) {
        if constexpr (sizeof...(args) > 0) {
            return (((insn->id == args) || ...));
        }
        return false;
    }

    template<uint32_t nToRetOn, int retCount = -1, size_t szBytes = 4096, class F1, class F2>
    requires(nToRetOn >= 1)
    auto findNth(const uint32_t *address, F1 &&match, F2 &&skip) {
        cs_insn *insn = cs_malloc(getCapstoneHandle());
        auto ptr = reinterpret_cast<uint64_t>(address);
        auto instructions = reinterpret_cast<const uint8_t *>(address);

        int rCount = retCount;
        uint32_t nCalls = nToRetOn;
        size_t size = szBytes;

        while(size > 0) {
            bool res = cs_disasm_iter(getCapstoneHandle(), &instructions, &size, &ptr, insn);
            if (res) {
                if (insn->id == X86_INS_RET) {
                    if (rCount == 0) {
                        cs_free(insn, 1);
                        Gluon::Logging::Logger::warn("Could not find: {} call at: {} within {} rets! Found all of the rets first!", nToRetOn, (void*)ptr, retCount);
                        return (decltype(match(insn)))std::nullopt;
                    }
                    rCount--;
                }
                else {
                    auto testRes = match(insn);
                    if (testRes) {
                        if (nCalls == 1) {
                            cs_free(insn, 1);
                            return testRes;
                        }
                        else {
                            nCalls--;
                        }
                    }
                    else if (skip(insn)) {
                        if (nCalls == 1) {
                            std::string name(insn->mnemonic);
                            cs_free(insn, 1);
                            Gluon::Logging::Logger::warn("Found: {} match, at: {} within: {} rets, but the result was a {}! Cannot compute destination address!", nToRetOn, (void*)ptr, retCount, name);
                            return (decltype(match(insn)))std::nullopt;
                        }
                        else {
                            nCalls--;
                        }
                    }
                }
                // Other instructions are silently ignored
            }
            else {
                // See below.
            }
        }
        cs_free(insn, 1);
        Gluon::Logging::Logger::warn("Could not find: {} call at: {} within: {} rets, within size: {}!", nToRetOn, (void *)address, retCount, szBytes);
        return (decltype(match(insn)))std::nullopt;
    }

    template<uint32_t nToRetOn, auto match, auto skip, int retCount = -1, size_t szBytes = 4096>
    requires(nToRetOn >= 1)
    auto findNth(const uint32_t *address) {
        cs_insn *insn = cs_malloc(getCapstoneHandle());
        auto ptr = reinterpret_cast<uint64_t>(address);
        auto instructions = reinterpret_cast<const uint8_t *>(address);

        int rCount = retCount;
        uint32_t nCalls = nToRetOn;
        size_t size = szBytes;

        while (size > 0) {
            bool res = cs_disasm_iter(getCapstoneHandle(), &instructions, &size, &ptr, insn);
            if (res) {
                if (insn->id == X86_INS_RET) {
                    if (rCount == 0) {
                        cs_free(insn, 1);
                        Gluon::Logging::Logger::warn("Could not find: {} call at: {} within {} rets! Found all of the rets first!", nToRetOn, (void*)ptr, retCount);
                        return (decltype(match(insn)))std::nullopt;
                    }
                    rCount--;
                }
                else {
                    auto testRes = match(insn);
                    if (testRes) {
                        if (nCalls == 1) {
                            cs_free(insn, 1);
                            return testRes;
                        }
                        else {
                            nCalls--;
                        }
                    }
                    else if (skip(insn)) {
                        if (nCalls == 1) {
                            std::string name(insn->mnemonic);
                            cs_free(insn, 1);
                            Gluon::Logging::Logger::warn("Found: {} match, at: {} within: {} rets, but the result was a {}! Cannot compute destination address!", nToRetOn, (void*)ptr, retCount, name);
                            return (decltype(match(insn)))std::nullopt;
                        }
                        else {
                            nCalls--;
                        }
                    }
                }
                // Other instructions are silently ignored.
            }
            else {
                // This is where invalid instructions would get skipped.
                // Unfortunately, CISC architectures make this annoying.
                // Therefore, I have elected to ignore it and hope nothing bad happens. :3
            }
        }
        cs_free(insn, 1);
        Gluon::Logging::Logger::warn("Could not find: {} call at: {} within: {} rets, within size: {}!", nToRetOn, (void *)address, retCount, szBytes);
        return (decltype(match(insn)))std::nullopt;
    }

    GLUON_API std::optional<uint32_t *> callConv(cs_insn *insn);

    GLUON_API std::optional<uint32_t *> jmpConv(cs_insn *insn);

    GLUON_API std::optional<uint32_t *> leaConv(cs_insn *insn);

    template<uint32_t nToRetOn, bool includeR = false, int retCount = -1, size_t szBytes = 4096>
    requires (nToRetOn >= 1)
    auto findNthCall(const uint32_t *address) {
        if constexpr (includeR) {
            return findNth<nToRetOn, retCount, szBytes>(address, &callConv, &insnMatch<X86_INS_CALL>);
        }
        else {
            return findNth<nToRetOn, retCount, szBytes>(address, &callConv, &insnMatch<>);
        }
    }

    template<uint32_t nToRetOn, bool includeR = false, int retCount = -1, size_t szBytes = 4096>
    requires (nToRetOn >= 1)
    auto findNthJmp(const uint32_t *address) {
        if constexpr (includeR) {
            return findNth<nToRetOn, retCount, szBytes>(address, &jmpConv, &insnMatch<X86_INS_JMP>);
        }
        else {
            return findNth<nToRetOn, retCount, szBytes>(address, &jmpConv, &insnMatch<>);
        }
    }

    template<uint32_t nToRetOn, bool includeR = false, int retCount = -1, size_t szBytes = 4096>
    requires (nToRetOn >= 1)
    auto findNthLea(const uint32_t *address) {
        if constexpr (includeR) {
            return findNth<nToRetOn, retCount, szBytes>(address, &leaConv, &insnMatch<X86_INS_LEA>);
        }
        else {
            return findNth<nToRetOn, retCount, szBytes>(address, &leaConv, &insnMatch<>);
        }
    }
}

#endif