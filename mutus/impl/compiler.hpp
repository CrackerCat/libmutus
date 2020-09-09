#pragma once

#ifdef __GNUC__
    #define MUTUS_FORCEINLINE __attribute__((always_inline, flatten))
    #define MUTUS_FORCEINLINE_CALLS
#else
    // note that even with this crap MSVC will still on occasion refuse to inline calls
    // fun fact: if you call std::forward enough it won't even inline that
    // lol
    #define MUTUS_FORCEINLINE __forceinline
    #define MUTUS_FORCEINLINE_CALLS [[msvc::forceinline_calls]]
#endif
