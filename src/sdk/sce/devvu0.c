/*
 * Unit: sdk/sce/devvu0
 *
 * Functions:
 *   0x00263528 sceDevVu0CheckBusy
 *   0x00263540 sceDevVu0Reset
 *   0x00263558 sceDevVu0Pause
 *   0x002635b8 sceDevVu0Continue
 *   0x00263608 sceDevVu0PutDBit
 *   0x00263638 sceDevVu0PutTBit
 *   0x00263668 sceDevVu0GetDBit
 *   0x00263678 sceDevVu0GetTBit
 *   0x00263688 sceDevVu0Sync
 *   0x002636c8 sceDevVu0Exec
 *   0x002636d8 sceDevVu0GetTpc
 *   0x002636e8 sceDevVu0GetCnd
 *   0x002637c8 sceDevVu0PutCnd
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


s32 sceDevVu0Continue(void)
{
  s32 temp_v0;
  s32 var_v0;
  temp_v0 = sceDevVu0Sync();
  var_v0 = 0;
  if (temp_v0 < 4)
  {
    if (temp_v0 > ((short) 0))
    {
      var_v0 = 1;
      sceDevVu0Exec(sceDevVu0GetTpc());
    }
  }
  return var_v0;
}
