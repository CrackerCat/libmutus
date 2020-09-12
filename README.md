# libmutus
stupid obfuscation library

## flow
- unroll and unroll_for allow for "manual" loop unrolling

## arithmetic
- reduce all operations to the bit level
    - security through absurdity:tm:
- usually causes MSVC to run out of heap space and clang takes obscene amounts of time
    - i'm not kidding, it takes a solid 5 minutes for clang to compile a single integer division

## documentation
no (though most cases are covered in the test repository)

## known issues
- reference types are fucky in unrolled expressions due to the way the capture logic works; use pointers instead