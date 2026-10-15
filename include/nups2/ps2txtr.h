#ifndef NUPS2_PS2TXTR_H
#define NUPS2_PS2TXTR_H

#include "nu_types.h"

/* GS-side state of one texture: VRAM placement and upload data per mip. */
struct nups2tex_s {
    u8 pad000[0x8];           /* 0x00 */
    union {
        struct {
            u16 palend;       /* 0x08 */
            u16 gsbase;       /* 0x0A */
            u16 gsused;       /* 0x0C */
        };
        struct {
            u64 sizes : 48;
            u64 uploaded : 1;
        };
    };
    u16 tw;                   /* 0x10 */
    u16 th;                   /* 0x12 */
    void *mipdata[7];         /* 0x14 */
    int reqsize[7];           /* 0x30 */
    int upsize[7];            /* 0x4C */
    u16 tbp[7];               /* 0x68 */
    u16 tbw[7];               /* 0x76 */
    u16 dbw[7];               /* 0x84 */
    u8 mipcount;              /* 0x92 */
    u8 psm;                   /* 0x93 */
    u8 uppsm;                 /* 0x94 */
    u8 tcc;                   /* 0x95 */
    u8 alphamode;             /* 0x96 */
    u8 pad097;                /* 0x97 */
    u8 *paldata;              /* 0x98 */
    u8 *palstream;            /* 0x9C */
    u16 cbp;                  /* 0xA0 */
    u16 cbw;                  /* 0xA2 */
    u16 cpsm;                 /* 0xA4 */
    u16 palsize;              /* 0xA6 */
    u16 palstreamsize;        /* 0xA8 */
    u8 pad0AA[0x2];           /* 0xAA */
    int gsaddr;               /* 0xAC */
}; /* 0xB0 */

struct nutex_s;

int QuantAddColour(u32 colour);
void QuantizeTexture(struct nutex_s *spec);
int GenPaletteLoad(struct nutex_s *spec, struct nups2tex_s *tex);
u64 *Ps2TxSetStateData(u64 *dst, struct nups2tex_s *tex, struct nups2tex_s *paltex, char context);
int Ps2TxValToPowerOfTwo(int value);
u64 *Ps2TxSetSurfaceStateData(u64 *dst, int surface);
void NuPs2ChangeTexPalStreamTemp(struct nups2tex_s *tex, u32 *pal);
int NuPs2TxDestroy(struct nups2tex_s *tex);
u8 *NuPs2TxUploadPal(u8 *stream, struct nups2tex_s *tex, int addr);
void NuPs2ChangeTexPal(struct nups2tex_s *tex, u32 *pal);
u64 Ps2TxGetTex0(struct nups2tex_s *tex);
int GetTexturePreLoad(void);

#endif /* NUPS2_PS2TXTR_H */
