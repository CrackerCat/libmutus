#pragma once

#include <tuple>

#include "compiler.hpp"

namespace mutus {
    template<typename fn_t>
    struct always_inline_invoker {
        template<typename... args>
        constexpr MUTUS_FORCEINLINE auto operator()(args... a) const {
            MUTUS_FORCEINLINE_CALLS
            {
                return fn_t{}(a...);
            }
        }
    };
}