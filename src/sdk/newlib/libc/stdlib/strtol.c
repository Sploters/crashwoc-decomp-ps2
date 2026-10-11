/*
 * Unit: sdk/newlib/libc/stdlib/strtol
 *
 * Functions:
 *   0x002731e0 _strtol_r
 *   0x00273418 strtol
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
s32 _strtol_r(s32, s32, s32, s32);
extern s32 _impure_ptr[];

void strtol(s32 arg0, s32 arg1, s32 arg2)
{
  int new_var;
  _strtol_r(_impure_ptr[0], arg0, arg1, arg2);
 new_var = 0; do { } while (new_var);
}
