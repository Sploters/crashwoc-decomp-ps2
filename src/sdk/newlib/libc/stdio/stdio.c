/*
 * Unit: sdk/newlib/libc/stdio/stdio
 *
 * Functions:
 *   0x00278440 __sread
 *   0x002784a8 __swrite
 *   0x00278528 __sseek
 *   0x00278590 __sclose
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
s32 _lseek_r(s32, s16, s32, s32);
s64 _write_r(s32, s16, s32, s32);

s64 __swrite(void *arg0, s32 arg1, s32 arg2)
{
  int new_var2;
  void *new_var;
  new_var = arg0;
  if ((*((u16 *) (((s8 *) new_var) + 0xC))) & 0x100)
  {
    _lseek_r(*((s32 *) (((s8 *) new_var) + 0x54)), *((s16 *) (((s8 *) new_var) + 0xE)), 0, 2);
  }
  *((u16 *) (((s8 *) new_var) + 0xC)) = (u16) ((*((u16 *) (((s8 *) new_var) + 0xC))) & 0xEFFF);
  new_var2 = ((s64) (_write_r(*((s32 *) (((s8 *) new_var) + 0x54)), *((s16 *) (((s8 *) new_var) + 0xE)), arg1, arg2) << 0x20)) >> 0x20;
  return new_var2;
}
