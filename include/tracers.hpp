#ifndef GLUON_TRACERS_HPP_
#define GLUON_TRACERS_HPP_

#include <cstdint>
#include <optional>

#include "gluon_config.hpp"
#include "gluon_logging.hpp"
#include "capstone/capstone.h"

#define ADDR_PTR(...) reinterpret_cast<AddrPtr>(__VA_ARGS__)

typedef std::uint32_t *AddrPtr;

namespace Gluon::Tracers {
    GLUON_API void initCapstone();
    GLUON_API csh getCapstone();

    template <std::uint32_t N, x86_insn Instruction, std::uint8_t Operand>
    GLUON_HIDDEN std::uint32_t *findNth(const std::uint32_t *address) {
        initCapstone();

        cs_insn *insn = cs_malloc(getCapstone());
        auto ptr = reinterpret_cast<std::uint64_t>(address);
        auto instructions = reinterpret_cast<const std::uint8_t *>(address);

        constexpr std::size_t kSize = 4096;
        std::size_t size = kSize;
        std::uint32_t n = N;

        while (cs_disasm_iter(getCapstone(), &instructions, &size, &ptr, insn)) {
            if (insn->id == Instruction) {
                n -= 1;
            }

            if (n == 0) {
                const cs_detail *detail = insn->detail;
                const cs_x86 arch = detail->x86;

                switch (const cs_x86_op operand = arch.operands[Operand]; operand.type) {
                    case X86_OP_IMM:
                        return reinterpret_cast<std::uint32_t *>(operand.imm);
                    case X86_OP_MEM:
                        if (operand.mem.base != X86_REG_RIP) {
                            Gluon::Logger::warn("Instruction targeted at 0x{:x} (n={}, target={}) with MEM operand at index {} is not relative to Instruction Pointer.", insn->address, N, cs_insn_name(getCapstone(), Instruction), Operand);
                            break;
                        }

                        return reinterpret_cast<std::uint32_t *>(insn->address + insn->size + operand.mem.disp);
                    default:
                        Gluon::Logger::warn("Instruction targeted at 0x{:x} (n={}, target={}) cannot be decoded; operand at index {} is not immediate nor a memory address.", insn->address, N, cs_insn_name(getCapstone(), Instruction), Operand);
                        break;
                }

                break;
            }
        }

        Gluon::Logger::warn("Could not find {} instruction (n={}) in the available buffer.", cs_insn_name(getCapstone(), Instruction), N);
        cs_free(insn, 1);
        return nullptr;
    }

    template <std::uint32_t N>
    GLUON_HIDDEN std::uint32_t *findNthCall(const std::uint32_t *address) {
        return findNth<N, X86_INS_CALL, 0>(address);
    }

    template <std::uint32_t N>
    GLUON_HIDDEN std::uint32_t *findNthJmp(const std::uint32_t *address) {
        return findNth<N, X86_INS_JMP, 0>(address);
    }

    template <std::uint32_t N>
    GLUON_HIDDEN std::uint32_t *findNthMov(const std::uint32_t *address) {
        return findNth<N, X86_INS_MOV, 1>(address);
    }

    template <std::uint32_t N>
    GLUON_HIDDEN std::uint32_t *findNthLea(const std::uint32_t *address) {
        return findNth<N, X86_INS_LEA, 1>(address);
    }
} // Gluon::Tracers

#endif // GLUON_TRACERS_HPP_
