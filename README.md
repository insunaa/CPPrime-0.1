# CPPrime-0.1
CPPrime 2 is an open source python based benchmark that scales well with RAM clock, timings and overall latency, less so with CPU speed. It is a fork of [PYPrime-2.x](https://github.com/monabuntur/PYPrime-2.x)

To build CPPrime use a compiler of your choice to compile `CPPrime.cpp` or use `meson setup && ninja` to build.

If you choose meson, be sure to change the configuration with `meson configure` as the default buildtype is debug and there are no compiler arguments for architecture optimization set.

A huge thanks goes to the guys at BenchMate and monabuntur without whom this wouldn't have been possible!
