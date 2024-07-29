#include <cstdint>
#include <optional>

#include "xref_helpers.hpp"

#include "abortion.hpp"
#include "gluon_logging.hpp"
#include "capstone/capstone.h"

namespace Gluon::XrefHelpers {
    static csh capstoneHandle;
    static bool valid;

    void initialiseCapstone() {
        cs_err csError = cs_open(CS_ARCH_X86, CS_MODE_64, &capstoneHandle);
        cs_option(capstoneHandle, CS_OPT_DETAIL, 1);
        if (csError) {
            Gluon::Logging::Logger::error("Capstone initialisation failed! {0}", static_cast<int>(csError));
            SAFE_ABORT();
        }
        Gluon::Logging::Logger::info("Capstone initialised!");
        valid = true;
    }

    csh getCapstoneHandle() {
        return capstoneHandle;
    }

    std::optional<uint32_t *> callConv(cs_insn *insn) {
        if (insn->id == X86_INS_CALL) {
            cs_x86_op op = insn->detail->x86.operands[0];
            switch (op.type) {
                case X86_OP_IMM:
                    return reinterpret_cast<uint32_t *>(op.imm);
                default:
                    Gluon::Logging::Logger::warn("Unable to get CALL operand address for non-immediate.");
                    break;
            }
        }
        return std::nullopt;
    }

    std::optional<uint32_t *> jmpConv(cs_insn *insn) {
        if (insn->id == X86_INS_JMP) {
            cs_x86_op op = insn->detail->x86.operands[0];
            switch (op.type) {
                case X86_OP_IMM:
                    return reinterpret_cast<uint32_t *>(op.imm);
                default:
                    Gluon::Logging::Logger::warn("Unable to get JMP operand address for non-immediate.");
                    break;
            }
        }
        return std::nullopt;
    }
    
    std::optional<uint32_t *> leaConv(cs_insn *insn) {
        if (insn->id == X86_INS_LEA) {
            cs_x86_op op = insn->detail->x86.operands[1];

            switch (op.type) {
                case X86_OP_MEM:
                    if (op.mem.base != X86_REG_RIP) {
                        Gluon::Logger::warn("Unable to get effective address; not rip-relative.");
                        break;
                    }
                    return reinterpret_cast<uint32_t *>(insn->address + insn->size + op.mem.disp);
                default:
                    Gluon::Logger::warn("Unable to get effective address for non-memory operand.");
                    break;
            }
        }
        return std::nullopt;
    }

    std::optional<uint32_t *> movConv(cs_insn *insn) {
        if (insn->id == X86_INS_MOV) {
            cs_x86_op op = insn->detail->x86.operands[1];

            switch (op.type) {
                case X86_OP_MEM:
                    if (op.mem.base != X86_REG_RIP) {
                        Gluon::Logger::warn("Unable to get address of source operand; not rip-relative.");
                        break;
                    }
                    return reinterpret_cast<uint32_t *>(insn->address + insn->size + op.mem.disp); // calculate address
                case X86_OP_IMM:
                    return reinterpret_cast<uint32_t *>(op.imm); // get address from immediate
                default:
                    Gluon::Logger::warn("Unable to get address of source operand; is not rip-relative address nor immediate.");
            }
        }
        return std::nullopt;
    }
}