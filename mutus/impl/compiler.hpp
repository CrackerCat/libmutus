#pragma once

#ifdef __GNUC__
    #define MUTUS_FORCEINLINE __attribute__((always_inline, flatten))
    #define MUTUS_FORCEINLINE_CALLS
#else
    #define MUTUS_FORCEINLINE __forceinline
    #define MUTUS_FORCEINLINE_CALLS [[msvc::forceinline_calls]]
#endif