# Trngcc

> Random number generation made easy in C

## Source code

All of the sources you will need can be found in
the [include](/include) folder

## Why Trngcc?

- Easy
- Small
- Simple
- Works on any compiler
- Works on any standard
- Works on both C and C++
- No 3rd party dependencies
- GPLv3 (open source) license
- ...

## Example

You can find multiple examples in [tests](/tests) folder,
but here's one:

```c
#include "include/trngcc.h"
#include <stdio.h>

#define PASSES 1024

int main(void) {
    TrngccResult trng = trngcc("/dev/urandom");
    char buf[PASSES]  = {0};

    unsigned long idx;
    for (idx = 0; idx < 10000; ++idx)
        printf("%ld\n", trngcc_unranged(trng, PASSES, buf).res.i64);

    trngcc_end(trng);

    return 0;
}
```

This will print 10 thousand numbers, 1024 passes per
number

## Testing

Just run the [test.sh](/test.sh) script, make sure you have gcc, g++, clang, clang++
and tcc installed, those compilers are the ones being tested, please install the
latest versions :)
