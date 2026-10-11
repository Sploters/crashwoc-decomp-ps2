/*
 * Unit: edtools/edcam
 *
 * Functions:
 *   0x001c1cf8 edcamMove
 *   0x001c2070 edcamSetSpeed
 *   0x001c2088 edcamSetAdjustFreedom
 *   0x001c2110 edcamSet
 *   0x001c2178 edcamSetPos
 *   0x001c2198 edcamSetAng
 *   0x001c21a8 edcamSetDist
 *   0x001c21b8 edcamGetDist
 *   0x001c21c8 edcamSetPosAng
 *   0x001c21f8 edcamGetPosAng
 *   0x001c2240 edcamGetPosAngSnap
 *   0x001c2288 edcamGetPosPointer
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
extern void *D_0062F99C;

f32 edcamGetDist(void) {
    return (*( f32 *)((s8 *)(D_0062F99C) + ( 0x14)));
}





s32 edcamGetPosPointer(void) {
    return D_0062F99C;
}





void edcamSetDist(f32 arg0) {
    (*( f32 *)((s8 *)(D_0062F99C) + ( 0x14))) = arg0;
}





void edcamSetAng(s32 arg0, s32 arg1)
{
  s8 *new_var;
 new_var = ((s8 *) D_0062F99C) + 0xC; do { *((s32 *) (((s8 *) D_0062F99C) + 0x10)) = arg1; } while (0);
  *((s32 *) new_var) = arg0;
}
