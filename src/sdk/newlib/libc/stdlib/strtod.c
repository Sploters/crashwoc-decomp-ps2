/*
 * Unit: sdk/newlib/libc/stdlib/strtod
 *
 * Functions:
 *   0x00272288 _strtod_r
 *   0x00273188 strtod
 *   0x002731b8 strtodf
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
s32 dptofp(s32);

void strtodf(void)
{
 do { dptofp(strtod()); } while (0);
}



extern s32 _impure_ptr[];

void strtod(s32 arg0, s32 arg1)
{
  int new_var;
  int new_var2;
  new_var2 = 0;
  _strtod_r(_impure_ptr[new_var2], arg0, arg1);
  new_var = new_var2;
}
