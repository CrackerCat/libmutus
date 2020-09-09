#pragma once

#include <type_traits>
#include <functional>

#include "../impl/compiler.hpp"
#include "../impl/util.hpp"

namespace mutus {
    // make the compiler a bit happier with some manual unrolling
    namespace impl {
        template<int c, typename tfn, typename tchk, typename... cap>
        struct unroll_impl {
            MUTUS_FORCEINLINE bool operator()(cap... args) {
                constexpr auto fn = tfn{};
                constexpr auto chk = tchk{};

                if (!chk(args...))
                    return false;

                fn(args...);

                if constexpr (c > 0) {
                    if (!chk(args...))
                        return false;

                    fn(args...);

                    if constexpr (c > 1) {
                        if (!chk(args...))
                            return false;

                        fn(args...);

                        if constexpr (c > 2) {
                            if (!chk(args...))
                                return false;

                            fn(args...);

                            if constexpr (c > 3)
                                return unroll_impl<c - 4, tfn, tchk, cap...>{}(args...);
                            else
                                return true;
                        } else {
                            return true;
                        }
                    } else {
                        return true;
                    }
                } else {
                    return true;
                }
            }
        };

        template<auto end, auto step, typename tfn, typename... cap>
        struct unroll_for_impl {
            template<auto cur>
            static MUTUS_FORCEINLINE void go(cap... args) {
                constexpr auto fn = tfn{};
                if constexpr (step < 0 ? (cur > end) : (cur < end)) {
                    fn(cur, args...);
                    if constexpr (step < 0 ? ((cur + step) > end) : ((cur + step) < end)) {
                        fn(cur + step, args...);
                        if constexpr (step < 0 ? ((cur + step * 2) > end) : ((cur + step * 2) < end)) {
                            fn(cur + step * 2, args...);
                            if constexpr (step < 0 ? ((cur + step * 3) > end) : ((cur + step * 3) < end)) {
                                fn(cur + step * 3, args...);
                                unroll_for_impl<end, step, tfn, cap...>::template go<cur + step * 4>(args...);
                            }
                        }
                    }
                }
            }
        };
    }

    template<typename tfn, typename tchk>
    struct unroll {
        template<int c, typename... cap>
        MUTUS_FORCEINLINE void go(cap... args) {
            while (impl::unroll_impl<c, always_inline_invoker<tfn>,
                    always_inline_invoker<tchk>, cap...>{}(args...));
        }

        constexpr unroll(tfn fn, tchk chk) { }
    };

    template<typename tfn>
    struct unroll_for {
        template<auto start, auto end, auto step, typename... cap>
        MUTUS_FORCEINLINE void go(cap... args) {
            impl::unroll_for_impl<end, step, always_inline_invoker<tfn>, cap...>::template go<start>(args...);
        }
        constexpr unroll_for(tfn fn) { }
    };
}