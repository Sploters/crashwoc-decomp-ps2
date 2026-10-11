/*
 * Unit: nups2/vu1man
 *
 * Functions:
 *   0x0016b7a0 NuPs2InitVU
 *   0x0016b7f8 VU1_DisplayFRegs
 *   0x0016b8c8 VU1_DisplaySRegs
 *   0x0016b948 VU1_LoadXD
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
s32 FlushCache(s32);
s32 (*NuDebugMsgProlog(s32 *, s32))(s32 *);
s32 VU1_DmaWrite(s32 *, s32);
extern s32 D_00281530[];
extern s32 D_002834E0[];
extern s32 D_00616A08[];
extern s32 D_00616A60[];

void NuPs2InitVU(void) {
    FlushCache(0);
    VU1_DmaWrite(&D_00281530[0], 1);
    VU1_DmaWrite(&D_002834E0[0], 1);
    NuDebugMsgProlog(&D_00616A08[0], 0x38)(&D_00616A60[0]);
}



s32 (*NuDebugMsgProlog(s32 *, s32))(s32 *);

void VU1_LoadXD(void) {
    FlushCache(0);
    VU1_DmaWrite(&D_00281530, 1);
    VU1_DmaWrite(&D_002834E0, 1);
    NuDebugMsgProlog(&D_00616A08, 0x38)(&D_00616A60);
}
