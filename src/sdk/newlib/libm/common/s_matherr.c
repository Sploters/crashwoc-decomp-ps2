/*
 * Unit: sdk/newlib/libm/common/s_matherr
 *
 * Functions:
 *   0x0026cbe0 matherr
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
s32 dpcmp(s64, s64);

s32 matherr(void *arg0) {
    s64 temp_a0;

    temp_a0 = (*( s64 *)((s8 *)(arg0) + ( 8)));
    dpcmp(temp_a0, temp_a0);
    return 0;
}
