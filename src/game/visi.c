/*
 * Unit: game/visi
 *
 * Functions:
 *   0x00201450 ApplyVisiTable
 *   0x00201668 VSAddObjs
 *   0x00201868 BuildVisiTable
 *   0x00201a00 ReleaseVisiTable
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
s32 NuMemFreeFn(s32, s32 *, s32);
extern s32 D_0061F2E8[];
extern s32 D_0063326C;
extern s32 D_00633270;

void ReleaseVisiTable(void) {
    if (D_00633270 != 0) {
        NuMemFreeFn(D_00633270, &D_0061F2E8[0], 0xC0);
    }
    D_0063326C = 0;
    D_00633270 = 0;
}
