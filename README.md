# faster-strtof
An algorithm which exceeds the speed of the standard libc function's.

Ideal for interpreters such as Python, for JSON parsers or faster game engine data deserialization.

**Advantages:**
* about  5-6x faster on average
* In some cases 10 or even 50 times faster!
* Execution time is more stable, meaning about 50-60 ns difference on each run, while the standard algorithm easily differs 2-300 ns (in some cases several thousands) on consecutive runs.

**Disadvantages:**
* Not entirely case sensitive, it can only handle "Inf","inf", "NaN" and "nan", while the standard algorithm is entirely is.
* Only allows decimal numbers and exponents
* Does not allow whitespaces
* Does not allow '+' sign before the exponent(E)

**Built with:**
gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0

![outcome](https://raw.githubusercontent.com/theQuetzalcoatl/faster-strtof/main/Screenshot%20from%202021-09-01%2018-56-02.png)
