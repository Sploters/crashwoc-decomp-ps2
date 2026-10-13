/*
 * Unit: game/particle
 *
 * Functions:
 *   0x001cb678 DrawParticles
 *   0x001cb798 InitParticles
 *   0x001cb7b8 CloseParticles
 *   0x001cb7e0 AddParticle
 *   0x001cb8c8 DestroyParticlesByHandle
 *   0x001cb958 ProcessParticles
 */

#include "creature.h"

extern void *NuLstCreate(int count, int size);
extern void NuLstDestroy(void *list);

/* Retail-owned (no Data-from-C yet). */
extern void *D_0062FCF0;
#define ParticleList D_0062FCF0

void InitParticles(s32 count) {
    ParticleList = NuLstCreate(count, 0x3C);
}

void CloseParticles(void) {
    if (ParticleList != 0) {
        NuLstDestroy(ParticleList);
    }
    ParticleList = 0;
}
