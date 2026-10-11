/*
 * Unit: sdk/sce/sdr_main
 *
 * Functions:
 *   0x0026d008 sceSdRemoteInit
 *   0x0026d0b0 sceSdTransToIOP
 *   0x0026d138 sceSdCallBack
 *   0x0026d148 sceSdRemote
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
extern s32 D_00612418[];

s32 sceSdCallBack(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_00612418[0];
    D_00612418[0] = arg0;
    return temp_v0;
}
