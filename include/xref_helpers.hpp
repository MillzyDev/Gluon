#ifndef GLUON_XREF_HELPERS_HPP_
#define GLUON_XREF_HELPERS_HPP_

#include <cstdint>

namespace Gluon::XrefHelpers {
    template<uint32_t  nToRetOn, auto match, auto skip, int retCount = -1, size_t szBytes = 4096>
    requires(nToRetOn >= 1)
    auto findNth(const uint32_t *address) {

    }
}

#endif