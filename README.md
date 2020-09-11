# libmutus
stupid obfuscation library

## flow
- unroll and unroll_for allow for "manual" loop unrolling

## arithmetic
- security through absurdity
- reduce all operations to the bit level
- causes MSVC to usually run out of heap space and causes clang to take obscene amounts of time
- i'm not kidding, it takes a solid 5 minutes for clang to compile division

## documentation
no

## known issues
- reference types are fucky in unrolled expressions; use pointers instead
- division doesn't work
