/*
 * Trngcc -- Random number generation made easy in C
 *
 * Author:  Ari Archer [https://ari-web.xyz/] <ari.web.xyz@gmail.com>
 * License: GPLv3
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>

#define TRGNCC_CHK_ERR(var)   \
    if (var.err != TRNGCC_OK) \
        return TRNGCC_RERR(var.err);

#define TRNGCC_RERR(error) trngcc_result(error, trngcc_result_type_i64(0))

#define TRNGCC_RRES(rtype, rvalue) \
    trngcc_result(TRNGCC_OK, trngcc_result_type_##rtype(rvalue))

#define TCC_RESULT_TYPE(type) \
    TrngccResultType type;    \
    type.type = val;          \
    return type;

typedef enum {
    TRNGCC_OK = 0,
    TRNGCC_FAILED_OPEN,
    TRNGCC_FAILED_READ,
    TRNGCC_FAILED_CLOSE,
    TRNGCC_INVALID_PASSES
} TrngccError;

typedef union {
    int64_t i64;
    FILE *fptr;
} TrngccResultType;

typedef struct {
    TrngccError err;
    TrngccResultType res;
} TrngccResult;

TrngccResult trngcc_result(const TrngccError err, const TrngccResultType res) {
    TrngccResult result;
    result.err = err;
    result.res = res;

    return result;
}

TrngccResultType trngcc_result_type_i64(int64_t val) { TCC_RESULT_TYPE(i64); }

TrngccResultType trngcc_result_type_fptr(FILE *val) { TCC_RESULT_TYPE(fptr); }

TrngccResult trngcc(const char *random) {
    FILE *f = fopen(random, "r");

    if (f == NULL)
        return TRNGCC_RERR(TRNGCC_FAILED_OPEN);

    return TRNGCC_RRES(fptr, f);
}

TrngccResult trngcc_end(const TrngccResult f) {
    TRGNCC_CHK_ERR(f);

    if (fclose(f.res.fptr) == EOF)
        return TRNGCC_RERR(TRNGCC_FAILED_CLOSE);

    return TRNGCC_RRES(i64, 0);
}

TrngccResult
trngcc_unranged(const TrngccResult f, const uint64_t passes, char buf[]) {
    uint64_t p;
    int64_t res   = 0;
    char previous = (char)passes;

    if (passes <= 0)
        return TRNGCC_RERR(TRNGCC_INVALID_PASSES);

    TRGNCC_CHK_ERR(f);

    fread(buf, sizeof(char), passes, f.res.fptr);

    for (p = 0; p < passes; ++p) {
        res -= (buf[p] + buf[(previous + (p - 1)) % passes]);
        previous = buf[p];
    }

    return TRNGCC_RRES(i64, res);
}
