#include <limits.h>
#include <stdint.h>

#include "utils.h"

static const uint64_t MAX_RAND_RETRIES = 10;
static const uint64_t MIN_RAND = 100000000000;
static const uint64_t MAX_RAND = ULONG_LONG_MAX;


uint64_t MyGetRandom64()
{
    uint64_t random_number = 0;

    for (uint64_t i = 0; i < MAX_RAND_RETRIES; i++)
        if (__builtin_ia32_rdrand64_step(&random_number))
        {
            random_number = random_number/ULONG_MAX*(MAX_RAND - MIN_RAND) + MIN_RAND;
            return random_number;
        }

    return 0;
}
