/*
 * Unit: sdk/newlib/libc/locale/locale
 *
 * Functions:
 *   0x00276f78 _setlocale_r
 *   0x00277000 _localeconv_r
 *   0x00277010 setlocale
 *   0x00277040 localeconv
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
extern s32 D_0062C408[];

s32 *_localeconv_r(void) {
    return &D_0062C408[0];
}



extern s32 _impure_ptr[];

void setlocale(s32 arg0, s32 arg1)
{
  s32 *new_var;
  new_var = &arg0;
  _setlocale_r(_impure_ptr[0], *new_var, arg1);
}
