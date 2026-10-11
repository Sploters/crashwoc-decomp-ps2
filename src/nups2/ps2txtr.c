/*
 * Unit: nups2/ps2txtr
 *
 * Functions:
 *   0x00165ff0 QuantizeTexture
 *   0x00166330 GenTextureLoad
 *   0x001670a8 GenPaletteLoad
 *   0x00167398 Ps2TxSetStateData
 *   0x001675a8 Ps2TxSetSurfaceStateData
 *   0x00167798 NuPs2CreateRenderTarget
 *   0x00167b10 NuPs2ChangeTexPalStreamTemp
 *   0x00167c98 NuPs2TintTexPalStreamTemp
 *   0x00167df8 NuPs2TxCreate
 *   0x00168120 NuPs2TxGenUpload
 *   0x00168430 Ps2TxValToPowerOfTwo
 *   0x00168470 NuPs2TxDestroy
 *   0x00168500 NuPs2TxUploadPal
 *   0x00168618 NuPs2ChangeTexPal
 *   0x00168718 Ps2TxGetTex0
 *   0x00168788 GetTexturePreLoad
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


s32 Ps2TxValToPowerOfTwo(s32 arg0)
{
  s32 var_a0;
  s32 var_a1;
  s32 temp_a2;
  int new_var;
  var_a0 = arg0;
  new_var = 1;
  temp_a2 = var_a0;
  var_a1 = 0;
  if (temp_a2 >= 2)
  {
    do
    {
      var_a0 = var_a0 >> new_var;
      var_a1 += new_var;
    }
    while (var_a0 >= 2);
  }
  return (!(temp_a2 & ((new_var << var_a1) - new_var))) ? (var_a1) : (var_a1 + new_var);
}



s32 (*NuErrorProlog(s32 *, s32))(s32 *);
extern s32 D_00616150[];
extern s32 D_006161F0[];

s32 GetTexturePreLoad(void) {
    NuErrorProlog(&D_00616150[0], 0x1CC)(&D_006161F0[0]);
    return 0;
}
