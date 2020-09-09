#pragma once

#include <cstdint>

#include "../impl/compiler.hpp"
#include "../flow/unroll.hpp"

namespace mutus {
    template<typename T>
    MUTUS_FORCEINLINE T ripple_add(T a, T b) {
        T c{};
        bool carry = false;
        unroll_for _for{[](int x, T *a, T *b, T *c, bool *carry) {
            bool a1 = (*a >> x) & 1;
            bool b1 = (*b >> x) & 1;
            bool sum = a1 ^ b1 ^ *carry;

            *carry = (a1 & b1) | (*carry & (a1 ^ b1));
            *c |= (sum ? (T) 1 : (T) 0) << x;
        }};
        _for.template go<0, sizeof(T) * 8, 1>(&a, &b, &c, &carry);
        return c;
    }

    template<typename T>
    MUTUS_FORCEINLINE T ripple_add_faster(T a, T b) {
        T c{};
        bool carry = false;
        unroll_for _for{[](int x, T *a, T *b, bool *carry) {
            const auto old_a = *a;

            *a ^= *b;
            *b &= old_a;

            *b <<= 1;
        }};
        _for.template go<0, sizeof(T) * 8, 1>(&a, &b, &carry);
        return a;
    }
}