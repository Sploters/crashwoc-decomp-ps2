/*
 * Unit: nucore/numemblk
 *
 * Functions:
 *   0x001053b0 NuMemBlkCreateHigh
 *   0x00105498 NuMemBlkCreate
 *   0x00105560 NuMemBlkDestroy
 *   0x00105588 NuMemBlkAlloc
 *   0x001055d8 NuMemBlkFree
 *   0x00105630 NuMemBlkCheckFreeList
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
s32 (*NuErrorProlog(s32 *, s32))(s32 *);
extern s32 D_00613CA0[];
extern s32 D_0062EA20[];

void NuMemBlkCheckFreeList(void *arg0) {
    s32 *var_v0;
    s32 var_v1;

    var_v0 = (*( s32 **)((s8 *)(arg0) + ( 0)));
    var_v1 = 0;
    if (var_v0 != ((void *)0)) {
        do {
            var_v0 = *var_v0;
            var_v1 += 1;
        } while (var_v0 != ((void *)0));
    }
    if (var_v1 != (*( s32 *)((s8 *)(arg0) + ( 0xC)))) {
        NuErrorProlog(&D_00613CA0[0], 0xA3)(&D_0062EA20[0]);
    }
}





void NuMemBlkFree(void *arg0, s32 *arg1) {
    (*( s32 *)((s8 *)(arg0) + ( 0xC))) = (s32) ((*( s32 *)((s8 *)(arg0) + ( 0xC))) + 1);
    memset(arg1, -2, (*( s32 *)((s8 *)(arg0) + ( 4))));
    *arg1 = (*( s32 **)((s8 *)(arg0) + ( 0)));
    (*( s32 **)((s8 *)(arg0) + ( 0))) = arg1;
}
