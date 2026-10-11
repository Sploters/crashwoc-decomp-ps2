/*
 * Unit: nu3d/nucvtskn
 *
 * Functions:
 *   0x00155408 ShareEdge
 *   0x001555f0 WriteWeights
 *   0x001558d8 AddPointDataToPacket
 *   0x00155e68 GenModData
 *   0x00156168 EndSkinPacket
 *   0x00156350 FindStripNext
 *   0x00156520 AddStripTri
 *   0x00156768 GenStrips
 *   0x00156938 NuPs2CreateSkinDma
 *   0x00156fa8 SortPrimdefs
 *   0x00157700 SetVtxSkinData
 *   0x00157920 NuPs2CreateSkinNorm
 *   0x00158108 NuPs2CreateSkinSmoothOld
 *   0x00158900 NuPs2CreateSkinSmooth
 *   0x00158cb8 NuPs2CreateSkin
 *   0x00158d38 NuPs2DestroySkin
 *   0x00158de8 CloseSkinning
 *   0x00158e18 InitSkinning
 *   0x00158e68 ResetSkinning
 *   0x00158e78 NuPs2SetTempSkinBuffer
 *   0x00158ec8 NuCvtSknInstFix
 *   0x00158f08 NuCvtSknInstUnFix
 *   0x00158f48 PrevPktIdx
 */


extern int D_00633128;
extern int skinbuffer_free;

void ResetSkinning(void) {
    skinbuffer_free = D_00633128;
}


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
extern s32 D_0063312C;
extern s32 D_00633178;
extern s32 D_00633180;
extern s32 D_00633184;

void NuPs2SetTempSkinBuffer(s32 arg0, s32 arg1)
{
  if (arg0 != 0)
  {
    D_00633178 = D_00633128;
    D_00633180 = skinbuffer_free;
    D_00633184 = D_0063312C;
    skinbuffer_free = (D_00633128 = arg0);
    D_0063312C = arg1;
    return;
  }
  D_00633128 = D_00633178;
  skinbuffer_free = D_00633180;
  D_0063312C = D_00633184;
}



s32 NuMemFreeFn(s32, s32 *, s32);
extern s32 D_006156B0[];

void CloseSkinning(void) {
    if (D_00633128 != 0) {
        NuMemFreeFn(D_00633128, &D_006156B0[0], 0x55);
    }
    D_00633128 = 0;
}
