#include <cstdint>
#include <optional>

#include "xref_helpers.hpp"

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
            // TODO: Abort
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
}