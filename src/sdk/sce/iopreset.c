/*
 * Unit: sdk/sce/iopreset
 *
 * Functions:
 *   0x002808a0 sceSifResetIop
 *   0x002809e0 sceSifIsAliveIop
 *   0x00280a08 sceSifSyncIop
 *   0x00280a40 sceSifRebootIop
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
s32 sceSifGetReg(M2C_UNK);

s32 sceSifIsAliveIop(void)
{
  int new_var;
  new_var = sceSifGetReg(4) & 0x10000;
  return new_var != 0;
}
