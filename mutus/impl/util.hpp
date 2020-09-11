#pragma once

#include <tuple>

#include "compiler.hpp"

namespace mutus {
    template<typename fn_t>
    struct basic_invoker {
        template<typename... args>
        constexpr MUTUS_API static auto go(args... a) {
            MUTUS_FORCEINLINE_CALLS
            {
                return fn_t{}(a...);
            }
        }

        constexpr basic_invoker(fn_t fn) {}
    };

    template<typename fn_t>
    struct for_invoker {
        template<int cur>
        struct for_impl {
            template<typename... args>
            constexpr MUTUS_API static auto go(args... a) {
                MUTUS_FORCEINLINE_CALLS
                {
                    return fn_t{}(cur, a...);
                }
            }
        };

        template<int cur>
        constexpr static auto impl() {
            return for_impl<cur>{};
        }

        constexpr for_invoker(fn_t fn) {}
    };
}