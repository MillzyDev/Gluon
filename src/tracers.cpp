#include "tracers.hpp"

#include "abortion.hpp"
#include "gluon_logging.hpp"

namespace Gluon::Tracers {
    static csh capstoneHandle = 0;

    void initCapstone() {
        if (capstoneHandle) {
            return;
        }

        const cs_err csError = cs_open(CS_ARCH_X86, CS_MODE_64, &capstoneHandle);
        cs_option(capstoneHandle, CS_OPT_DETAIL, 1);
        if (csError) {
            Gluon::Logger::error("Capstone initialisation failed! {0}", static_cast<int>(csError));
            SAFE_ABORT();
        }
        Gluon::Logger::info("Capstone initialised!");
    }

    csh getCapstone() {
        return capstoneHandle;
    }
} // Gluon::Tracers