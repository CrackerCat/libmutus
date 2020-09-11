#include <iostream>

#include <mutus/impl/mutus.hpp>

__attribute__((noinline, dllexport)) void test_math_impl(uint64_t a) {
    mutus::integer<64> i1{a};
   //printf("%lld << 3 = %llx\n", a, (i1 << 3).as_uint64());
   //printf("0xCCCCCCC >> 3 = %llx\n", (mutus::integer<64>{0xCCCCCCC} >> 3).as_uint64());
    printf("%lld * 0xCCCCCCC = %llx\n", a, (i1 * 0xCCCCCCC).as_uint64());
    printf("%lld / 1 = %llx\n", a, (i1 / 1).as_uint64());
    printf("%lld / 3 = %llx\n", a, (i1 / 3).as_uint64());
    //printf("%lld + 0xCCCCCCC = %llx\n", a, (i1 + 0xCCCCCCC).as_uint64());
}

__attribute__((noinline)) void test_math() {
    volatile uint64_t a1 = 123, b1 = 456, c1 = 100, d1 = 10, e1 = 1;

    test_math_impl(a1);
    test_math_impl(b1);
    test_math_impl(d1);
    test_math_impl(e1);

    //printf("123 * 456 = %d\n", mutus::bit_mul<1>(a, b));
    //printf("123 * 456 = %d\n", mutus::bit_mul<4>(a, b));
    //printf("123 * 0xCCCCCCC = %d\n", mutus::bit_mul<2>(a, 0xCCCCCCCULL));
    //printf("123 * 0xCCCCCCC = %d\n", mutus::bit_mul<8>(a, 0xCCCCCCCULL));
    //printf("100 / 10 = %d\n", mutus::bit_div(c, d));
    //printf("100 / 3 = %d\n", mutus::bit_div(c, e));
}

__attribute__((noinline)) void test_unroll() {
    int c = 0;
    mutus::unroll{mutus::basic_invoker{[](int *c) {
        printf("%d\n", *c);
        (*c)++;
    }},
    mutus::basic_invoker{[](int *c) {
        return *c < 20;
    }}}.impl<20>().go(&c);

    uint64_t f = 0;
    mutus::unroll_for{mutus::for_invoker{[](int cur, uint64_t *fuck) {
        printf("%lld\n", cur);
        ++(*fuck);
    }}}.impl<0, 200, 2>().go(&f);
    printf("l: %lld\n", f);
}

int main(int argc, char** argv) {
    std::cout << "Hello, World!" << std::endl;

    test_unroll();
    test_math();

    return 0;
}