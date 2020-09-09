#pragma once
#include <cstdint>

#include "../impl/compiler.hpp"
#include "../impl/util.hpp"

#include "add.hpp"

namespace mutus {
    template<int nbits, typename tn>
    MUTUS_FORCEINLINE tn bit_mul(tn a, tn b) {
        static_assert((sizeof(tn) * 8) % nbits == 0);
        tn c{};
        unroll_for _for{[](int x, tn *a, tn *b, tn *c) {
            auto n = (*a >> x) & ((1ULL << nbits) - 1);
            *c = ripple_add_faster((*b << x) * n, *c);
        }};
        _for.template go<0, ((sizeof(tn) * 8) / nbits), nbits>(&a, &b, &c);
        return c;
    }
}