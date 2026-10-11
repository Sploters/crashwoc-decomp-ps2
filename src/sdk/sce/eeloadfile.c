/*
 * Unit: sdk/sce/eeloadfile
 *
 * Functions:
 *   0x0027fb40 _lf_bind
 *   0x0027fc40 _lf_version
 *   0x0027fcd0 sceSifLoadFileReset
 *   0x0027fd08 _sceSifLoadModuleBuffer
 *   0x0027ff10 sceSifStopModule
 *   0x00280118 sceSifUnloadModule
 *   0x002801a8 sceSifSearchModuleByName
 *   0x00280248 sceSifSearchModuleByAddress
 *   0x002802d8 sceSifLoadModuleBuffer
 *   0x002802f8 sceSifLoadStartModuleBuffer
 *   0x00280318 _sceSifLoadModule
 *   0x00280540 sceSifLoadModule
 *   0x00280560 sceSifLoadStartModule
 *   0x00280580 _sceSifLoadElfPart
 *   0x00280688 sceSifLoadElfPart
 *   0x002806a8 sceSifLoadElf
 *   0x002806d0 sceSifGetIopAddr
 *   0x002807c0 sceSifSetIopAddr
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


void sceSifLoadStartModuleBuffer(void)
{
 do { _sceSifLoadModuleBuffer(); } while (0);
}



extern s32 D_00613418[];
extern s32 D_007068E8[];

s32 sceSifLoadFileReset(void)
{
  unsigned int new_var;
  D_00613418[0] = -1;
  new_var = 4;
  memset(&D_007068E8[0], 0, new_var);
  return 0;
}
