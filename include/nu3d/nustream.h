#ifndef NU3D_NUSTREAM_H
#define NU3D_NUSTREAM_H

#include "nu_types.h"
#include "numath/numathtypes.h"

struct numtl_s;

/* A render packet stream: DMA packets are written at cur. */
struct nurndrstream_s {
    u8 *start;                /* 0x00 */
    union {
        u8 *b;
        s32 *i;
        u32 *w;
        f32 *f;
    } cur;                    /* 0x04 */
    u32 *txtr_start;          /* 0x08 texture DMA chain */
    u32 *txtr_end;            /* 0x0C */
    int zbuf;                 /* 0x10 GS address */
    struct numtl_s *mtl;      /* 0x14 */
    void *mpgtag;             /* 0x18 */
    unsigned int sortkey;     /* 0x1C */
    int local_mtx_id;         /* 0x20 */
    short view_mtx_id;        /* 0x24 */
    short scissor_id;         /* 0x26 */
    short xyoffset_id;        /* 0x28 */
    void *light_addr;         /* 0x2C */
    unsigned int flags;       /* 0x30 */
    int mtl_tid;              /* 0x34 */
}; /* 0x38 */

struct nucolour4_s;

void NuRndrInitStreams(void);
void NuRndrStreamInit(int size);
struct nurndrstream_s *NuRndrStreamAllocStream(unsigned int sortkey, void *mpgtag, unsigned int flags);
void NuRndrStreamFlush(void);
void NuRndrStreamSetMisc(struct nurndrstream_s *stream);
int NuRndrStreamSetLocalMtx(struct nurndrstream_s *stream);
int NuRndrSetViewMtx(struct numtx_s *vm, struct numtx_s *pm, struct numtx_s *clip_pm);
int NuRndrSetSpecularLight(struct nuvec_s *dir, struct nucolour4_s *col);
void *NuStreamGetOutputBuffer(int index);
void NuRndrSwapStreamBuffers(void);
void FaceYDirStream(int angle);
int NuRndrSetLocalMtx(struct numtx_s *mtx, int clip);
void NuRndrSetXYOffset(int x, int y);
void NuRndrSetScissor(int x0, int y0, int x1, int y1);
int NuRndrSetZFix(int zfix);
int NuRndrSetDirectionalLights(struct nuvec_s *d0, struct nucolour3_s *c0, struct nuvec_s *d1, struct nucolour3_s *c1, struct nuvec_s *d2, struct nucolour3_s *c2);
int NuRndrSetDirectionalLights4(struct nuvec_s *d0, struct nucolour4_s *c0, struct nuvec_s *d1, struct nucolour4_s *c1, struct nuvec_s *d2, struct nucolour4_s *c2);
int NuRndrSetAmbientLight(struct nucolour3_s *colour);
int NuRndrSetGlobalAlpha(float alpha);
int NuRndrSetFxMtx(struct numtx_s *mtx);
u64 *NuRndrStreamGetScissor(void);
u64 *NuRndrStreamGetOffset(void);
void NuRndrStreamResetStream(struct nurndrstream_s *stream, int flag);
void NuRndrStreamResetStreams(int flag);
void NuRndrVu1Draw(void);
void NuRndrStreamFxMPG(int mode);
void *NuRndrStreamAlloc(int qwords);
int NuRndrStreamAddMtl(struct nurndrstream_s *stream, struct numtl_s *mtl);
u64 NuRndrStreamAddTex(struct nurndrstream_s *stream, int tid, signed char mode);
int NuRndrStreamSetDirectionalLights(struct nurndrstream_s *stream, struct nuvec_s *d0, struct nucolour3_s *c0, struct nuvec_s *d1, struct nucolour3_s *c1, struct nuvec_s *d2, struct nucolour3_s *c2);
int NuRndrStreamSetAmbientLight(struct nurndrstream_s *stream, struct nucolour3_s *colour);
int NuRndrStreamSetViewMtx(struct nurndrstream_s *stream);
int NuRndrStreamSetViewMtxScissorClip(struct nurndrstream_s *stream);
int NuRndrStreamSetLocalMtxEx(struct nurndrstream_s *stream, int clip);
void *NuRndrStreamPrependMPG(struct nurndrstream_s *stream, void *tag);
void NuRndrStreamClearZBState(void);
void NuRndrStreamSetZBState(unsigned int zbuf, unsigned int psm, int zmask);
void *NuRndrStreamAddZBState(void *stream, int index);
void CheckRndrStream(void);

#endif /* NU3D_NUSTREAM_H */
