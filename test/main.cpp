#include <iostream>

#include <mutus/impl/mutus.hpp>

__attribute__((noinline)) void test_math() {
    volatile uint64_t a1 = 123, b1 = 456, c1 = 100, d1 = 10, e1 = 3;
    uint64_t a = a1, b = b1, c = c1, d = d1, e = e1;

    printf("123 + 456 = %d\n", mutus::ripple_add(a, b));
    printf("123 * 456 = %d\n", mutus::bit_mul<1>(a, b));
    printf("123 * 456 = %d\n", mutus::bit_mul<4>(a, b));
    printf("123 * 0xCCCCCCC = %d\n", mutus::bit_mul<2>(a, 0xCCCCCCCULL));
    printf("123 * 0xCCCCCCC = %d\n", mutus::bit_mul<8>(a, 0xCCCCCCCULL));
    printf("100 / 10 = %d\n", mutus::bit_div(c, d));
    printf("100 / 3 = %d\n", mutus::bit_div(c, e));
}

__attribute__((noinline)) void test_unroll() {
    int c = 0;
    mutus::unroll{[](int *c) {
        printf("%d\n", *c);
        (*c)++;
    },
    [](int *c) {
        return *c < 20;
    }}.go<20>(&c);

    uint64_t f = 0;
    mutus::unroll_for{[](int cur, uint64_t *fuck) {
        printf("%lld\n", cur);
        ++(*fuck);
    }}.go<0, 200, 2>(&f);
    printf("l: %lld\n", f);
}

int main(int argc, char** argv) {
    std::cout << "Hello, World!" << std::endl;

    test_unroll();
    test_math();

    return 0;
}