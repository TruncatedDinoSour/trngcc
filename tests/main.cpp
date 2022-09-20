#include <trngcc.h>
#include <iostream>

#define PASSES 1024

int main(void) {
    TrngccResult trng = trngcc("/dev/urandom");
    char buf[PASSES]  = {0};

    unsigned long idx;
    for (idx = 0; idx < 1000; ++idx)
        std::cout << trngcc_unranged(trng, PASSES, buf).res.i64 << '\n';

    trngcc_end(trng);

    return 0;
}
