//
// Created by DefCon42 on 9/10/2020.
//

#include <cstdint>

#include "../impl/compiler.hpp"
#include "../flow/unroll.hpp"

namespace mutus {
    template<int n>
    struct integer {
        bool data[n];

        MUTUS_API integer(uint64_t on) {
            unroll_for _for{for_invoker{[](int cur, integer *_this, uint64_t *on) {
                _this->data[cur] = ((*on >> cur) & 1) != 0;
            }}};
            _for.template impl<0, n, 1>().go(this, &on);
        }

        MUTUS_API integer() : integer(0ULL) {}

        MUTUS_API integer(const integer& other) : data{} {
            std::copy(other.data, other.data + n, data);
        }

        MUTUS_API uint64_t as_uint64() const {
            uint64_t c{};
            unroll_for _for{for_invoker{[](int cur, const integer *_this, uint64_t *nc) {
                *nc |= static_cast<uint64_t>(_this->data[cur]) << cur;
            }}};
            _for.template impl<0, n, 1>().go(this, &c);
            return c;
        }

        struct bnot {
            template<auto cur>
            struct bnot_impl {
                template<typename... cap>
                MUTUS_API void go(const bool *a, bool *b) {
                    b[cur] = !a[cur];
                }
            };

            template<auto cur>
            constexpr static auto impl() {
                return bnot_impl<cur>{};
            }
        };

        struct band {
            template<auto cur>
            struct band_impl {
                template<typename... cap>
                MUTUS_API void go(const bool *a, const bool *b, bool *c) {
                    c[cur] = a[cur] & b[cur];
                }
            };

            template<auto cur>
            constexpr static auto impl() {
                return band_impl<cur>{};
            }
        };

        struct bor {
            template<auto cur>
            struct bor_impl {
                template<typename... cap>
                MUTUS_API void go(const bool *a, const bool *b, bool *c) {
                    c[cur] = a[cur] | b[cur];
                }
            };

            template<auto cur>
            constexpr static auto impl() {
                return bor_impl<cur>{};
            }
        };

        struct shl {
            template<auto cur>
            struct shl_impl {
                template<typename... cap>
                MUTUS_API void go(const bool *a, bool * b, int off) {
                    if (cur < off)
                        b[cur] = false;
                    else
                        b[cur] = a[cur - off];
                }
            };

            template<auto cur>
            constexpr static auto impl() {
                return shl_impl<cur>{};
            }
        };

        struct shr {
            template<auto cur>
            struct shr_impl {
                template<typename... cap>
                MUTUS_API void go(const bool *a, bool *b, int off) {
                    if (cur + off > n)
                        b[cur] = false;
                    else
                        b[cur] = a[cur + off];
                }
            };

            template<auto cur>
            constexpr static auto impl() {
                return shr_impl<cur>{};
            }
        };

        struct add {
            template<auto cur>
            struct add_impl {
                template<typename... cap>
                MUTUS_API void go(const bool *a, const bool *b, integer *c, bool *carry) {
                    bool a1 = a[cur];
                    bool b1 = b[cur];
                    bool sum = a1 ^ b1 ^ *carry;

                    *carry = (a1 & b1) | (*carry & (a1 ^ b1));
                    *c |= (sum ? integer{1} : integer{0}) << cur;
                }
            };

            template<auto cur>
            constexpr static auto impl() {
                return add_impl<cur>{};
            }
        };

        struct mul {
            template<auto cur>
            struct mul_impl {
                template<typename... cap>
                MUTUS_API void go(integer *a, integer *b, integer *c) {
                    if (b->data[0])
                        *c += *a;
                    *a <<= 1;
                    *b >>= 1;
                }
            };

            template<auto cur>
            constexpr static auto impl() {
                return mul_impl<cur>{};
            }
        };

        struct div {
            template<auto cur>
            struct div_impl {
                template<typename... cap>
                MUTUS_API void go(const integer *a, const integer *b, integer *q, integer *r) {
                    *q <<= 1;
                    *r <<= 1;
                    r->data[0] |= (*a >> cur).data[0];

                    if (*r >= *b) {
                        *r -= *b;
                        q->data[0] = true;
                    }
                }
            };

            template<auto cur>
            constexpr static auto impl() {
                return div_impl<cur>{};
            }
        };

        MUTUS_API bool operator==(const integer& other) const {
            auto not_equal = false;

            unroll_for _for{for_invoker{[](int cur, const integer *_this, const integer *_other, bool *eq) {
                *eq |= _this->data[cur] ^ _other->data[cur];
            }}};
            _for.template impl<0, n, 1>().go(this, &other, &not_equal);

            return !not_equal;
        }

        MUTUS_API bool operator>(const integer& other) const {
            auto i1 = ~*this & other;
            auto i2 = *this & ~other;

            i1 |= i1 >> 1;
            i1 |= i1 >> 2;
            i1 |= i1 >> 4;
            i1 |= i1 >> 8;
            i1 |= i1 >> 16;
            i1 |= i1 >> 32;

            auto gt = i2 & ~i1;
            gt |= gt >> 1;
            gt |= gt >> 2;
            gt |= gt >> 4;
            gt |= gt >> 8;
            gt |= gt >> 16;
            gt |= gt >> 32;

            return gt.data[0];
        }

        MUTUS_API bool operator>=(const integer& other) const {
            return *this == other || *this > other;
        }

        // hooray for not needing to worry about floating point crap!!!!
        //
        MUTUS_API bool operator!=(const integer& other) const {
            return !(*this == other);
        }

        MUTUS_API bool operator<(const integer& other) const {
            return !(*this >= other);
        }

        MUTUS_API bool operator<=(const integer& other) const {
            return !(*this > other);
        }

        // not everything is implemented here
        // do it yourself you nerd

        MUTUS_API integer operator|(const integer& b) const {
            integer c{};
            unroll_for<bor> _for{{}};
            _for.template impl<0, n, 1>().go(this->data, b.data, c.data);
            return c;
        }

        MUTUS_API integer operator&(const integer& b) const {
            integer c{};
            unroll_for<band> _for{{}};
            _for.template impl<0, n, 1>().go(this->data, b.data, c.data);
            return c;
        }

        MUTUS_API integer operator<<(int ref) const {
            integer c{};
            unroll_for<shl> _for{{}};
            _for.template impl<0, n, 1>().go(this->data, c.data, ref);
            return c;
        }

        MUTUS_API integer operator>>(int ref) const {
            integer c{};
            unroll_for<shr> _for{{}};
            _for.template impl<0, n, 1>().go(this->data, c.data, ref);
            return c;
        }

        MUTUS_API integer operator~() const {
            integer b{};
            unroll_for<bnot> _for{{}};
            _for.template impl<0, n, 1>().go(this->data, b.data);
            return b;
        }

        MUTUS_API integer operator+(const integer& b) const {
            integer c{};
            bool carry = false;
            unroll_for<add> _for{{}};
            _for.template impl<0, n, 1>().go(this->data, b.data, &c, &carry);
            return c;
        }

        MUTUS_API integer operator-(const integer& b) const {
            return *this + (~b + 1);
        }

        MUTUS_API integer operator*(const integer& b) const {
            integer a1 = *this;
            integer b1 = b;

            integer c{};
            unroll_for<mul> _for{{}};
            _for.template impl<0, n, 1>().go(&a1, &b1, &c);
            return c;
        }

        MUTUS_API integer operator/(const integer& b) const {
            integer q{}, r{};
            unroll_for<div> _for{{}};
            _for.template impl<n - 1, -1, -1>().go(this, &b, &q, &r);
            return r;
        }

        MUTUS_API integer& operator<<=(int ref) {
            integer a1 = *this;
            unroll_for<shl> _for{{}};
            _for.template impl<0, n, 1>().go(a1.data, this->data, ref);
            return *this;
        }

        MUTUS_API integer& operator>>=(int ref) {
            integer a1 = *this;
            unroll_for<shr> _for{{}};
            _for.template impl<0, n, 1>().go(a1.data, this->data, ref);
            return *this;
        }

        MUTUS_API integer& operator-=(const integer& b) {
            return (*this += (~b + 1));
        }

        MUTUS_API integer& operator+=(const integer& b) {
            integer a1 = *this;
            bool carry = false;
            unroll_for<add> _for{{}};
            _for.template impl<0, n, 1>().go(a1.data, b.data, this, &carry);
            return *this;
        }

        MUTUS_API integer& operator|=(const integer& b) {
            unroll_for<bor> _for{{}};
            _for.template impl<0, n, 1>().go(this->data, b.data, this->data);
            return *this;
        }
    };
}