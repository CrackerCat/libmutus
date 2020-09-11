#pragma once

#include <type_traits>
#include <functional>

#include "../impl/compiler.hpp"
#include "../impl/util.hpp"

namespace mutus {
    namespace impl {
        template<int c, typename tfn, typename tchk>
        struct unroll_impl {
            template<typename... cap>
            MUTUS_API bool operator()(cap... args) {
                if (!tchk::go(args...))
                    return false;

                tfn::go(args...);

                if constexpr(c > 0)
                    return unroll_impl<c - 1, tfn, tchk>{}(args...);
                else
                    return true;
            }
        };

        template<auto end, auto step, typename tfor>
        struct unroll_for_impl {
            template<auto cur, typename... cap>
            static MUTUS_API void go(cap... args) {
                if constexpr (step < 0 ? (cur > end) : (cur < end)) {
                    tfor::template impl<cur>().template go(args...);
                    unroll_for_impl<end, step, tfor>::template go<cur + step>(args...);
                }
            }
        };
    }

    template<typename tfn, typename tchk>
    struct unroll {
        template<auto c>
        struct impl_unroll {
            template<typename... cap>
            MUTUS_API void go(cap... args) {
                while (impl::unroll_impl<c, tfn, tchk>{}(args...));
            }
        };

        template<auto c>
        constexpr auto impl() const {
            return impl_unroll<c>{};
        }

        constexpr unroll(tfn, tchk) { }
    };

    template<typename tfn>
    struct unroll_for {
        template<auto start, auto end, auto step>
        struct impl_for {
            template<typename... cap>
            MUTUS_API void go(cap... args) {
                impl::unroll_for_impl<end, step, tfn>::template go<start>(args...);
            }
        };

        template<auto start, auto end, auto step>
        constexpr auto impl() const {
            return impl_for<start, end, step>{};
        }

        constexpr unroll_for(tfn) { }
    };
}