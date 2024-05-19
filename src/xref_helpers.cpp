#include "xref_helpers.hpp"

#include "gluon_logging.hpp"
#include "capstone/capstone.h"

namespace Gluon::XrefHelpers {
    static csh capstoneHandle;
    static bool valid;

    void inititialiseCapstone() {
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
}