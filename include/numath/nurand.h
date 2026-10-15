#ifndef NUMATH_NURAND_H
#define NUMATH_NURAND_H

#include "nu_types.h"

struct nunrand_s {
    s64 idum;
};

void NuRandSetSeed(struct nunrand_s *nrand, int seed);
s64 NuRand(struct nunrand_s *nrand);
float NuFloatRand(struct nunrand_s *nrand);
float NuRandFloat(void);
int NuRandInt(void);
void NuRandSeed(unsigned int seed);

#endif /* NUMATH_NURAND_H */
