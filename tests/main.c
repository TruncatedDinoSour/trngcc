#include <trngcc.h>
#include <stdio.h>

#define PASSES 1024

int main(void) {
    TrngccResult trng = trngcc("/dev/urandom");
    char buf[PASSES]  = {0};

    unsigned long idx;
    for (idx = 0; idx < 1000; ++idx)
        printf("%ld\n", trngcc_unranged(trng, PASSES, buf).res.i64);

    trngcc_end(trng);

    return 0;
}
