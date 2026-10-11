/*
 * Unit: nu3d/nucvtgeo
 *
 * Functions:
 *   0x001505b0 NuCvtDmaFaceOn
 *   0x00150a58 NuCvtDmaFaceY
 *   0x00150f28 NuCvtSizeDmaBezTriCFromIdx
 *   0x00151020 NuCvtDmaBezTriCFromIdx
 *   0x00151c40 NuCvtTriStreamCFromIdx
 *   0x00152090 NuCvtTriStripStreamCFromIdx
 *   0x00152528 NuCvtSizeDmaTriCFromIdx
 *   0x001526f0 NuCvtDmaTriCFromIdx
 *   0x00153320 NuCvtSizeDmaTriStripCFromIdx
 *   0x00153738 NuCvtDmaTriStripCFromIdx
 *   0x001547d8 NuPs2CreateFaceOn
 *   0x00154b40 NuCvtCalcFastPoint
 *   0x00154cd0 NuPs2CreateRenderStream
 *   0x00155140 NuPs2DestroyRenderStream
 *   0x00155178 NuCvtGeoSetColourRefOffset
 *   0x00155180 NuCvtGeoFixup
 *   0x001551c0 NuCvtGeoBeginColourRef
 *   0x001551d8 NuCvtGeoEndColourRef
 *   0x00155210 NuCvtSizeDmaFaceOn
 *   0x001552b8 NuCvtSizeDmaFaceY
 *   0x00155360 NuCvtSetMaxSpan
 *   0x00155390 NuCvtGeoAddColourRef
 *   0x001553d8 NuCvtDestroyPacketInfo
 */

struct nucolourref_s { // 0xc
	/* 0x0 */ unsigned int *addr;
	/* 0x4 */ int count;
	/* 0x8 */ unsigned int cols;
};


void NuCvtGeoFixup(struct nucolourref_s* ref, int offset) {
        while(ref->count != 0)
        {
            ref->addr = (int)((char*)offset + (int)ref->addr);
            ref = (char*)ref + ((ref->count * 8) + 8);
        }
}


extern int D_0062EEDC;
#define colourref_active D_0062EEDC
extern int D_0062EEE0;
#define colourref_offset D_0062EEE0
extern int D_00633118;
#define colourref_base D_00633118
extern int D_0063311C;
#define colourref_count D_0063311C

void NuCvtGeoSetColourRefOffset(int offset) {
    colourref_offset = offset;
}


void NuCvtGeoBeginColourRef(int base) {
    colourref_base = base;
    colourref_active = 1;
    colourref_count = 0;
    colourref_offset = 0;
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
extern f32 NuCvtMaxSpan;
extern s32 NuCvtPntEna;

void NuCvtSetMaxSpan(f32 arg0) {
    NuCvtMaxSpan = arg0;
    if (arg0 <= 0.0f) {
        NuCvtPntEna = 0;
        return;
    }
    NuCvtPntEna = 1;
}



s32 NuMemFreeFn(s32, s32 *, s32);
extern s32 D_00615538[];
extern s32 info_pktdata;

void NuCvtDestroyPacketInfo(void) {
    if (info_pktdata != 0) {
        NuMemFreeFn(info_pktdata, &D_00615538[0], 0xA9E);
    }
}





void NuPs2DestroyRenderStream(void *arg0) {
    s32 temp_a0_2;
    void *temp_a0;

    temp_a0 = (*( void **)((s8 *)(arg0) + ( 0xC)));
    if (temp_a0 != ((void *)0)) {
        temp_a0_2 = (*( s32 *)((s8 *)(temp_a0) + ( 8)));
        if (temp_a0_2 != 0) {
            NuMemFreeFn(temp_a0_2, &D_00615538, 0xA98);
        }
    }
}





void *NuCvtGeoEndColourRef(void)
{
  int new_var3;
  int new_var;
  s8 *new_var2;
  NuCvtGeoAddColourRef(0);
  new_var = 8;
  new_var2 = ((s8 *) D_0063311C) - -4;
  new_var3 = D_0063311C + (((*((s32 *) new_var2)) * new_var) + new_var);
  D_0062EEDC = 0;
  return new_var3;
}
