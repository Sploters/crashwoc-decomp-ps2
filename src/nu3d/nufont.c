/*
 * Unit: nu3d/nufont
 *
 * Functions:
 *   0x001309a8 NuFntLoad
 *   0x00130be8 NuFntLoadMem
 *   0x00130e00 NuFntCreate
 *   0x00131480 NuFntPrintChar
 *   0x00131640 NuFntPrintV
 *   0x00131930 NuFntPrint
 *   0x00131990 NuFntPrintLen
 *   0x00131ab8 NuFntPrintEx
 *   0x00131ce8 NuFntInit
 *   0x00131d70 NuFntClose
 *   0x00131dd8 NuFntSet
 *   0x00131e18 NuFntDestroy
 *   0x00131e78 NuFntScale
 *   0x00131eb0 NuFntMoveAbs
 *   0x00131ef8 NuFntMoveRel
 *   0x00131f18 NuFntPos
 *   0x00131f60 NuFntSetPen
 *   0x00131f98 NuFntPrintLenV
 */

extern int D_0062ED3C;
extern int D_0062ED40;
#define font_x D_0062ED3C
#define font_y D_0062ED40

void NuFntMoveRel(int x, int y)
{
    font_x += x;
    font_y += y;
}

/* Font header: only the fields these setters read (offsets from retail). */
struct nufont_s {
    char pad0[0xC];
    int height;     /* 0x0C */
    char pad10[0x4];
    int base;       /* 0x14 */
};

extern struct nufont_s *NuFntCur;
extern struct nufont_s *system_font;
extern int NuFntBase;
extern int NuFntHeight;
extern int D_0062ED34;
extern int D_0062ED38;
extern int D_002E8570[4];
#define font_xscale D_0062ED34
#define font_yscale D_0062ED38
#define font_pen D_002E8570

void NuFntSet(struct nufont_s *font)
{
    if (font == 0) {
        font = system_font;
    }
    NuFntCur = font;
    NuFntHeight = (font->height << 3) * font_yscale >> 4;
    NuFntBase = (font->base << 3) * font_yscale >> 4;
}

void NuFntScale(int xscale, int yscale)
{
    font_xscale = xscale;
    font_yscale = yscale;
    NuFntHeight = (NuFntCur->height << 3) * yscale >> 4;
    NuFntBase = (NuFntCur->base << 3) * yscale >> 4;
}

void NuFntSetPen(int colour)
{
    font_pen[0] = colour & 0xFF;
    font_pen[1] = (colour >> 8) & 0xFF;
    font_pen[2] = (colour >> 16) & 0xFF;
    font_pen[3] = (colour >> 24) & 0xFF;
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
extern s32 PS2_REZ_H;
extern s32 PS2_REZ_W;
extern s32 PS2_VCNTR_X;
extern s32 PS2_VCNTR_Y;

void NuFntPos(s32 *arg0, s32 *arg1) {
    *arg0 = D_0062ED3C - ((PS2_VCNTR_X - ((s32) PS2_REZ_W >> 1)) * 0x10);
    *arg1 = D_0062ED40 - ((PS2_VCNTR_Y - ((s32) PS2_REZ_H >> 1)) * 0x10);
}





void NuFntMoveAbs(s32 arg0, s32 arg1) {
    D_0062ED3C = arg0 + ((PS2_VCNTR_X - ((s32) PS2_REZ_W >> 1)) * 0x10);
    D_0062ED40 = (arg1 - NuFntBase) + ((PS2_VCNTR_Y - ((s32) PS2_REZ_H >> 1)) * 0x10);
}
