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


#ifndef NULL
#define NULL ((void *)0)
#endif
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
s32 NuLstDestroy(s32);
extern s32 D_0062FCF0;

void CloseParticles(void) {
    if (D_0062FCF0 != 0) {
        NuLstDestroy(D_0062FCF0);
    }
    D_0062FCF0 = 0;
}
