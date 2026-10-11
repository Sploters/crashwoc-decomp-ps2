/*
 * Unit: sdk/newlib/libc/stdio/findfp
 *
 * Functions:
 *   0x00276420 std
 *   0x00276478 __sfmoreglue
 *   0x002764e8 __sfp
 *   0x002765c8 _cleanup_r
 *   0x002765d8 _cleanup
 *   0x002765e8 __sinit
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
extern s32 __sclose[];
extern s32 __sread[];
extern s32 __sseek[];
extern s32 __swrite[];

volatile unsigned char std(void *arg0, s16 arg1, s16 arg2, s32 arg3)
{
  int new_var;
  *((s32 *) (((s8 *) arg0) + 0x54)) = arg3;
  *((s16 *) (((s8 *) arg0) + 0xC)) = arg1;
  new_var = 0x20;
  *((s16 *) (((s8 *) arg0) + 0xE)) = arg2;
  *((s32 **) (((s8 *) arg0) + new_var)) = &__sread[0];
  *((s32 **) (((s8 *) arg0) + 0x24)) = &__swrite[0];
  *((s32 **) (((s8 *) arg0) + 0x28)) = &__sseek[0];
  *((s32 **) (((s8 *) arg0) + 0x2C)) = &__sclose[0];
  *((s32 *) (((s8 *) arg0) + 0)) = 0;
  new_var = 4;
  *((s32 *) (((s8 *) arg0) + new_var)) = 0;
  *((s32 *) (((s8 *) arg0) + 8)) = 0;
  *((s32 *) (((s8 *) arg0) + 0x10)) = 0;
  *((s32 *) (((s8 *) arg0) + 0x18)) = 0;
  do
  {
  }
  while (0);
  *((void **) (((s8 *) arg0) + 0x1C)) = arg0;
}
