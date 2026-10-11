/*
 * Unit: sdk/newlib/libc/stdlib/malloc
 *
 * Functions:
 *   0x002703e0 malloc
 *   0x00270408 free
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
s32 _malloc_r(s32, s32);
extern s32 _impure_ptr[];

void malloc(s32 arg0)
{
  s32 *new_var;
  new_var = &arg0;
  _malloc_r(_impure_ptr[0], *new_var);
}
