/*
 * Unit: nups2/ps2dma
 *
 * Functions:
 *   0x0016b9a0 GSRegDecode
 *   0x0016bd00 DirectDecode
 *   0x0016bfe8 VTDecode
 *   0x0016c8f0 TagWalk
 *   0x0016cc18 gsBeginGifPacket
 *   0x0016ccc0 gsCloseGifPacket
 *   0x0016cda0 NuScratchReset
 *   0x0016cdb0 NuScratchAlloc32
 *   0x0016cde8 NuScratchAlloc64
 *   0x0016ce30 NuScratchAlloc128
 *   0x0016ce78 NuScratchRelease
 *   0x0016ce88 vpDmaTag_Set
 *   0x0016ceb0 vpDmaTag_Cnt
 *   0x0016ced0 vpDmaTag_End
 *   0x0016cef0 vpDmaTag_EndEx
 *   0x0016cf18 vpDmaTag_Ref
 *   0x0016cf40 VU0_DmaWrite
 *   0x0016cfa8 VU0_DmaGetStatus
 *   0x0016cfc0 GS_DmaGetStatus
 *   0x0016d028 GS_DmaWrite
 *   0x0016d0e0 VU1_DmaWrite
 *   0x0016d198 VU1_DmaWriteN
 *   0x0016d218 VU1_DmaRead
 *   0x0016d298 VU1_DmaGetStatus
 *   0x0016d310 SPR_DmaWriteN
 *   0x0016d388 SPR_DmaReadN
 *   0x0016d400 SPR_DmaGetWriteStatus
 *   0x0016d418 SPR_DmaGetReadStatus
 *   0x0016d430 GS_DmaWriteN
 *   0x0016d498 GS_DmaGetWriteStatus
 *   0x0016d4b0 vtBeginDirect
 *   0x0016d4d0 vtCloseDirect
 *   0x0016d4f8 VifTagDummy
 *   0x0016d500 VifTagWalk
 *   0x0016d5e0 CrcChain
 *   0x0016d700 VU1_DmaGetStatusI
 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

typedef struct DmaTag {
    u16 qwc;
    u8 pad;
    u8 id;
    u32 addr;
    u32 vifcode[2];
} __attribute__((aligned(16))) DmaTag;

typedef struct GifTag {
    u64 nloop : 15;
    u64 eop : 1;
    u64 pad16 : 30;
    u64 pre : 1;
    u64 prim : 11;
    u64 flg : 2;
    u64 nreg : 4;
    u64 regs;
} GifTag;

typedef DmaTag *(*TagWalkFunc)(DmaTag *tag, void *user);
typedef void (*NuErrorFunc)(const char *fmt, ...);

typedef char *va_list;
#define va_start(ap, last) \
    ((ap) = ((va_list)__builtin_next_arg(last) \
        - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0)))
#define va_arg(ap, type) (*(type *)(((ap) += 8) - 8))
#define va_end(ap) ((ap) = (va_list)0)

#define VIF0_CHCR       (*(volatile int *)0x10008000)
#define VIF0_QWC        (*(volatile int *)0x10008020)
#define VIF0_TADR       (*(volatile int *)0x10008030)
#define VIF1_CHCR       (*(volatile int *)0x10009000)
#define VIF1_QWC        (*(volatile int *)0x10009020)
#define VIF1_TADR       (*(volatile int *)0x10009030)
#define GIF_CHCR        (*(volatile int *)0x1000A000)
#define GIF_MADR        (*(volatile int *)0x1000A010)
#define GIF_QWC         (*(volatile int *)0x1000A020)
#define SPR_FROM_CHCR   (*(volatile int *)0x1000D000)
#define SPR_FROM_MADR   (*(volatile int *)0x1000D010)
#define SPR_FROM_QWC    (*(volatile int *)0x1000D020)
#define SPR_FROM_SADR   (*(volatile int *)0x1000D080)
#define SPR_TO_CHCR     (*(volatile int *)0x1000D400)
#define SPR_TO_MADR     (*(volatile int *)0x1000D410)
#define SPR_TO_QWC      (*(volatile int *)0x1000D420)
#define SPR_TO_SADR     (*(volatile int *)0x1000D480)
#define DMAC_STAT       (*(volatile int *)0x1000E010)

#define DMA_ADDR_MASK   0x0FFFFFFF
#define DMA_STR         0x100

#define DMAC_CH_VIF0    0x001
#define DMAC_CH_VIF1    0x002
#define DMAC_CH_GIF     0x004
#define DMAC_CH_FROMSPR 0x100
#define DMAC_CH_TOSPR   0x200

#define SCRATCHPAD_BASE 0x70000000

extern DmaTag D_002EA550;
extern DmaTag D_002EA560;
extern DmaTag D_002EA570;
extern DmaTag D_002EA580;
extern u32 *D_0062F0E0;
extern DmaTag *vpdmatag_curr;
extern u32 *D_00633200;
extern GifTag *D_00633208;
extern int D_0062F0D8;
extern int D_0062F0DC;
extern char D_00616E00[];
extern char D_00617850[];
extern int gs_dma_status;
extern int vu1_dma_status;
extern DmaTag *vu1_last_write;

extern NuErrorFunc NuErrorProlog(const char *file, int line);
extern void TagWalk(DmaTag *tag, TagWalkFunc walker);
extern DmaTag *VifTagWalk(DmaTag *tag, void *user);
int VU0_DmaGetStatus(void);
int VU1_DmaGetStatus(void);

#define dmatag_cnt      D_002EA550
#define dmatag_ref      D_002EA560
#define dmatag_end      D_002EA570
#define dmatag_end_ex   D_002EA580
#define nuscratch_ptr   D_0062F0E0
#define giftag_curr     D_00633208
#define gs_dma_walk     D_0062F0D8
#define vu1_dma_walk    D_0062F0DC
#define ps2dma_file     D_00616E00
#define bad_status_msg  D_00617850


u32 *gsBeginGifPacket(u32 *packet, int nloop, int eop, int pre, int prim,
                      int flg, int nreg, ...)
{
    u64 *tag = (u64 *)packet;
    va_list regs;
    int i;

    va_start(regs, nreg);
    tag[1] = 0;
    for (i = 0; i < nreg; i++) {
        tag[1] |= (u64)va_arg(regs, int) << (i * 4);
    }
    va_end(regs);
    tag[0] = (nloop & 0x7FFF) | ((u64)(eop & 1) << 15)
        | ((u64)(pre & 1) << 46) | ((u64)(prim & 0x7FF) << 47)
        | ((u64)(flg & 3) << 58) | ((u64)nreg << 60);
    giftag_curr = (GifTag *)tag;
    return packet + 4;
}


u32 *gsCloseGifPacket(u32 *packet)
{
    GifTag *tag = giftag_curr;

    switch (tag->flg) {
    case 0:
        tag->nloop = (((u32)packet - (u32)tag - sizeof(GifTag)) >> 4)
            / tag->nreg;
        break;
    case 1:
        tag->nloop = (((u32)packet - (u32)tag - sizeof(GifTag)) >> 3)
            / tag->nreg;
        break;
    case 2:
        tag->nloop = ((u32)packet - (u32)tag - sizeof(GifTag)) >> 4;
        break;
    }
    return packet;
}


void NuScratchReset(void)
{
    nuscratch_ptr = (u32 *)SCRATCHPAD_BASE;
}


u32 *NuScratchAlloc32(u32 size)
{
    u32 *previous = nuscratch_ptr;
    u32 *block = (u32 *)(((u32)previous + 3) & ~3U);

    size = (size + 3) & ~3U;
    nuscratch_ptr = (u32 *)((u32)block + size);
    *nuscratch_ptr = (u32)previous;
    nuscratch_ptr++;
    return block;
}


u32 *NuScratchAlloc64(u32 size)
{
    u32 *previous = nuscratch_ptr;
    u32 *block = (u32 *)(((u32)previous + 7) & ~7U);

    size = (size + 3) & ~3U;
    nuscratch_ptr = (u32 *)((u32)block + size);
    *nuscratch_ptr = (u32)previous;
    nuscratch_ptr++;
    return block;
}


u32 *NuScratchAlloc128(u32 size)
{
    u32 *previous = nuscratch_ptr;
    u32 *block = (u32 *)(((u32)previous + 15) & ~15U);

    size = (size + 3) & ~3U;
    nuscratch_ptr = (u32 *)((u32)block + size);
    *nuscratch_ptr = (u32)previous;
    nuscratch_ptr++;
    return block;
}


void NuScratchRelease(void)
{
    nuscratch_ptr = (u32 *)nuscratch_ptr[-1];
}


DmaTag *vpDmaTag_Set(DmaTag *tag, u32 qwc, u32 id, u32 addr)
{
    tag->qwc = qwc;
    tag->id = id << 4;
    tag->addr = addr;
    tag->pad = 0;
    tag->vifcode[0] = 0;
    tag->vifcode[1] = 0;
    return tag + 1;
}


DmaTag *vpDmaTag_Cnt(DmaTag *tag)
{
    vpdmatag_curr = tag;
    *tag = dmatag_cnt;
    return tag + 1;
}


DmaTag *vpDmaTag_End(DmaTag *tag)
{
    vpdmatag_curr = tag;
    *tag = dmatag_end;
    return tag + 1;
}


DmaTag *vpDmaTag_EndEx(DmaTag *tag, u32 vifcode0, u32 vifcode1)
{
    vpdmatag_curr = tag;
    dmatag_end_ex.vifcode[0] = vifcode0;
    dmatag_end_ex.vifcode[1] = vifcode1;
    *tag = dmatag_end_ex;
    return tag + 1;
}


DmaTag *vpDmaTag_Ref(DmaTag *tag, u32 addr)
{
    vpdmatag_curr = tag;
    dmatag_ref.addr = addr;
    *tag = dmatag_ref;
    return tag + 1;
}


void VU0_DmaWrite(DmaTag *tag)
{
    while (VU0_DmaGetStatus()) {
    }
    VIF0_QWC = 0;
    VIF0_TADR = (int)tag;
    DMAC_STAT = DMAC_CH_VIF0;
    VIF0_CHCR = 0x145;
}


int VU0_DmaGetStatus(void)
{
    return VIF0_CHCR & 0x100;
}


int GS_DmaGetStatus(void)
{
    int status;

    if (gs_dma_walk) {
        gs_dma_walk = 0;
    }
    status = gs_dma_status;
    if (status == 0 && (GIF_CHCR & DMA_STR)) {
        NuErrorProlog(ps2dma_file, 0x570)(bad_status_msg);
    }
    return status;
}


void VU1_DmaWriteN(DmaTag *tag, int qwc)
{
    while (VU1_DmaGetStatus()) {
    }
    VIF1_QWC = qwc;
    VIF1_TADR = (u32)tag & DMA_ADDR_MASK;
    DMAC_STAT = DMAC_CH_VIF1;
    VIF1_CHCR = 0x141;
    vu1_last_write = tag;
}


void VU1_DmaRead(DmaTag *tag, int wait)
{
    if (wait) {
        while (VU1_DmaGetStatus()) {
        }
    }
    VIF1_QWC = 0;
    VIF1_TADR = (u32)tag & DMA_ADDR_MASK;
    DMAC_STAT = DMAC_CH_VIF1;
    VIF1_CHCR = 0x141;
}


int VU1_DmaGetStatus(void)
{
    int status;

    if (vu1_dma_walk) {
        vu1_dma_walk = 0;
        TagWalk(vu1_last_write, VifTagWalk);
    }
    status = vu1_dma_status;
    if (status == 0 && (VIF1_CHCR & DMA_STR)) {
        NuErrorProlog(ps2dma_file, 0x580)(bad_status_msg);
    }
    return status;
}


void SPR_DmaWriteN(u32 spr, void *data, int qwc)
{
    while (SPR_TO_CHCR & DMA_STR) {
    }
    SPR_TO_QWC = qwc;
    SPR_TO_MADR = (u32)data & DMA_ADDR_MASK;
    SPR_TO_SADR = spr & DMA_ADDR_MASK;
    DMAC_STAT = DMAC_CH_TOSPR;
    SPR_TO_CHCR = 0x101;
}


void SPR_DmaReadN(void *data, u32 spr, int qwc)
{
    while (SPR_FROM_CHCR & DMA_STR) {
    }
    SPR_FROM_QWC = qwc;
    SPR_FROM_MADR = (u32)data & DMA_ADDR_MASK;
    SPR_FROM_SADR = spr & DMA_ADDR_MASK;
    DMAC_STAT = DMAC_CH_FROMSPR;
    SPR_FROM_CHCR = 0x100;
}


int SPR_DmaGetWriteStatus(void)
{
    return (SPR_TO_CHCR >> 8) & 1;
}


int SPR_DmaGetReadStatus(void)
{
    return (SPR_FROM_CHCR >> 8) & 1;
}


void GS_DmaWriteN(void *data, int qwc)
{
    while (GIF_CHCR & DMA_STR) {
    }
    GIF_QWC = qwc;
    GIF_MADR = (u32)data & DMA_ADDR_MASK;
    DMAC_STAT = DMAC_CH_GIF;
    GIF_CHCR = 0x101;
}


int GS_DmaGetWriteStatus(void)
{
    return (GIF_CHCR >> 8) & 1;
}


u32 *vtBeginDirect(u32 *packet, u32 irq)
{
    D_00633200 = packet;
    *packet = (irq << 31) | 0x50000000;
    return packet + 1;
}


u32 *vtCloseDirect(u32 *packet)
{
    u32 *direct = D_00633200;

    *direct |= (((u32)(packet - 1) - (u32)direct) >> 4) & 0xFFFF;
    return packet;
}


DmaTag *VifTagDummy(DmaTag *tag, void *user)
{
    return tag;
}


int VU1_DmaGetStatusI(void)
{
    return VIF1_CHCR & 0x100;
}
