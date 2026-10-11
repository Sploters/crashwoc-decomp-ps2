/*
 * Unit: sdk/newlib/libc/errno/errno
 *
 * Functions:
 *   0x002703d0 __errno
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
extern s32 _impure_ptr[];

s32 __errno(void) {
    return _impure_ptr[0];
}
