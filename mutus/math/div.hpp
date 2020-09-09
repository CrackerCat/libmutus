#pragma once
#include <cstdint>

#include "../impl/compiler.hpp"
#include "../impl/util.hpp"

#include "add.hpp"

namespace mutus {
    template<typename tn>
    MUTUS_FORCEINLINE tn bit_div(tn a, tn b) {
        tn q{}, r{};
        unroll_for _for{[](int x, tn *a, tn *b, tn *q, tn *r) {
            *q <<= 1;
            *r <<= 1;
            *r |= (*a & (1ULL << x)) >> x;

            if (*r >= *b) {
                *r = ripple_add_faster(*r, ~*b + 1);
                *q |= 1;
            }
        }};
        _for.template go<static_cast<int>(sizeof(tn) * 8) - 1, -1, -1>(&a, &b, &q, &r);
        return q;
    }
}