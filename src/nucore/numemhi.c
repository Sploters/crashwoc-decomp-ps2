/*
 * Unit: nucore/numemhi
 *
 * Functions:
 *   0x00105688 NuAllocHighInit
 *   0x00105708 NuAllocHigh
 *   0x001057b8 NuFreeHigh
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
extern s32 D_00613CC8[];
extern s32 D_00613CF0[];
extern void *D_0062EA28;

void NuAllocHighInit(s32 arg0, s32 arg1) {
    s32 var_s0;
    s32 var_s1;

    var_s1 = arg1;
    var_s0 = arg0;
    if (D_0062EA28 != ((void *)0)) {
        NuErrorProlog(&D_00613CC8[0], 0x2F)(&D_00613CF0[0]);
    }
    if (var_s0 & 0xF) {
        var_s1 -= 0xF;
        var_s0 = (var_s0 + 0xF) & 0xF;
    }
    if (var_s1 & 0xF) {
        var_s1 = (var_s1 - 0xF) & 0xF;
    }
    D_0062EA28 = (void *) var_s0;
    (*( s32 *)((s8 *)(var_s0) + ( 4))) = var_s1;
    (*( s32 *)((s8 *)(var_s0) + ( 0))) = var_s0;
}
