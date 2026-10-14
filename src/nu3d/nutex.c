/*
 * Unit: nu3d/nutex
 *
 * Functions:
 *   0x0011c590 NuTexReadBitmapMMEx
 *   0x0011cb90 NuTexReadBitmapMem
 *   0x0011cfb8 NuTexSetRenderTarget
 *   0x0011d108 NuTexCreateEx
 *   0x0011d300 NuTexCreateStream
 *   0x0011d450 NuTexAccomodateRS
 *   0x0011d580 NuTexAccomodate
 *   0x0011d6e0 NuTexSet
 *   0x0011d848 NuTexDisplayTexturePage
 *   0x0011dbb8 NuTexRecoverNativeDataEx
 *   0x0011de90 NuTexCreateNative
 *   0x0011e2d0 NuTexWriteMaterialSettings
 *   0x0011e438 NuTexGetSysBuffSize
 *   0x0011e448 NuTexInitEx
 *   0x0011e510 NuTexInit
 *   0x0011e5d0 NuTexClose
 *   0x0011e690 NuTexReserve
 *   0x0011e700 NuTexUnReserve
 *   0x0011e730 NuTexReadBitmap
 *   0x0011e758 NuTexReadBitmapMM
 *   0x0011e778 NuTexCreate
 *   0x0011e798 NuTexDestroy
 *   0x0011e850 NuTexRef
 *   0x0011e888 NuTexUnRef
 *   0x0011e8c0 NuTexType
 *   0x0011e8e8 NuTexWidth
 *   0x0011e910 NuTexHeight
 *   0x0011e938 NuTexHasAlpha
 *   0x0011e978 NuTexAlphaBits
 *   0x0011e9c8 NuTexPixelSize
 *   0x0011ea20 NuTexImgSize
 *   0x0011ea88 NuTexImgSizeMM
 *   0x0011eb70 NuTexPalSize
 *   0x0011eba0 NuTexRecoverNativeData
 *   0x0011ebc8 NuTexReadNative
 *   0x0011ec40 NuTexGetPointers
 *   0x0011ec98 NuTexFlushTemp
 *   0x0011eca8 NuTexAssignAddr
 *   0x0011ecd0 NuTexAssignTempAddr
 *   0x0011ed20 NuTexGetReqSize
 *   0x0011ed58 NuTexCloneTempAddr
 *   0x0011ed98 NuTexPalChange
 *   0x0011edd0 NuTexPalChangeStreamTemp
 *   0x0011ee08 NuTexGetTex0
 *   0x0011ee60 NuTexSetPal
 *   0x0011ef80 NuTexSetZBuffer
 *   0x0011eff8 NuTexGetTextAddr
 */

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef long long s64;
typedef unsigned long long u64;

typedef struct NuPs2Tex {
    u8 unk000[0x10];
    u16 p2w;
    u16 p2h;
    u8 unk014[0x1C];
    int reqsize[10];
    u8 unk058[0x10];
    u16 texaddr;
    u8 unk06A[0xC];
    u16 tbw;
    u8 unk078[0x1A];
    u8 mipcount;
    u8 psm;
    u8 unk094[0x18];
    int gsaddr;
    u8 unk0B0[0x10];
} NuPs2Tex;

/* Caller-supplied texture description; copied into the head of NuTex. */
typedef struct NuTexSpec {
    int type;
    int width;
    int height;
    int decal;
    void *bits;
    void *pal;
} NuTexSpec;

typedef struct NuTex {
    union {
        NuTexSpec spec;
        struct {
            int type;
            int width;
            int height;
            int decal;
            void *bits;
            void *pal;
        };
    };
    union {
        u16 refcount;
        s64 flags;
        struct {
            u64 : 16;
            u64 valid : 1;          /* NUTEX_VALID */
            u64 native : 1;         /* NUTEX_NATIVE */
        };
    };
    NuPs2Tex ps2tex;
} NuTex;

typedef struct NuMtl {
    u8 unk000[0x168];
    u64 attrib;
    u8 unk170[0x34];
    int tid;
    int alpha;
    short mm_k;
    u8 mm_l;
    u8 unk1AF[0x15];
    s8 gs_context;
} NuMtl;

#define NUTEX_VALID 0x10000
#define NUTEX_NATIVE 0x20000
#define NUTEX_MAX 0x400

typedef struct {
    u16 qwc;
    u8 mark;
    u8 id;
    void *addr;
    unsigned int vif0;
    unsigned int vif1;
} NuDmaTag;

/* Render stream (see nu3d/nustream); only the fields used here. */
struct nurndrstream_s {
    void *start;                    /* 0x00 */
    u8 *cur;                        /* 0x04 */
    NuDmaTag *txtr_start;           /* 0x08 first texture upload */
    NuDmaTag *txtr_end;             /* 0x0C last texture upload's closing tag */
    int topfree;                    /* 0x10 GS VRAM free pointer */
};

#define DMATAG_IRQ 0x80

/* Quadword written after each tag that brackets a PATH3 texture upload. */
struct nutexmark_s {
    NuDmaTag *tag;
    unsigned int magic;
};

#define TAG_BEFORE(p) (&((NuDmaTag *)(p))[-1])
#define MARK_AT(p) ((struct nutexmark_s *)(p))

extern int nutex_use_path3;
extern u8 *vpDmaTag_Next(u8 *cur, void *addr);
extern u8 *NuTexAccomodate(u8 *dma, int tid);

extern NuTex *D_0062EBEC;
#define TexList D_0062EBEC

extern int gs_botfree;
extern int nurndr_pixel_width;
extern int nurndr_pixel_height;
extern int nurndr_pixel_p2w;
extern int nurndr_pixel_p2h;
extern u64 cur_rt;
extern int gs_top;
extern int D_00633068;
#define gs_tempfree D_00633068
extern int D_00633060;
#define gs_topfree D_00633060
extern int D_00633064;
#define gs_bot D_00633064

extern int D_0062EC04;
#define gs_reserve_sp D_0062EC04
extern int D_0067D640[256];
#define gs_reserve_stack D_0067D640

extern int D_0062EBE8;
#define nutex_initialised D_0062EBE8
extern int D_0062EBF0;
#define nutex_cnt D_0062EBF0
extern int D_0063305C;
#define nutex_max D_0063305C
extern char *D_0062EC10;
#define nutex_sysbuff D_0062EC10
extern int D_0062EBF8;
#define gs_wrap_frame D_0062EBF8
extern int D_0062EBFC;
#define gs_frame D_0062EBFC
extern int D_0062EC00;
#define gs_flush_cnt D_0062EC00
extern int nutex_alloc_cnt;

extern int numtl_force_mipmap;
extern int sys_mm_k;
extern int sys_mm_l;

extern int D_002D3F20[4];
#define nutex_wrap_mode D_002D3F20
extern int D_002D3F30[4];
#define nutex_mag_filter D_002D3F30
extern int D_002D3F40[4];
#define nutex_min_filter D_002D3F40

extern char D_00614590[];
#define nutex_srcfile D_00614590
extern char D_00614690[];
#define msg_tex_invalid D_00614690

extern int NuTexReadBitmapMMEx(char *name, int mipmaps, int *addr, int *addrend);
extern int NuTexCreateEx(NuTex *spec, void *extmem, int extsize);
extern int NuTexCreateNative(void *data);
extern int NuTexRecoverNativeDataEx(int tex, void *dest, int size, int header, int image, int palette);
extern void NuTexClose(void);
extern void NuPs2ChangeTexPal(NuPs2Tex *ps2tex);
extern void NuPs2ChangeTexPalStreamTemp(NuPs2Tex *ps2tex);
extern void NuPs2TxDestroy(NuPs2Tex *ps2tex);
extern int NuPs2TxCreate(NuTex *spec, NuPs2Tex *ps2tex);
extern u64 Ps2TxGetTex0(NuPs2Tex *ps2tex);
extern void NuRndrStreamSetZBState(int addr, int psm, int test);
extern void *NuRndrStreamAddZBState(void *stream, int enable);
extern void ps2videoGetFreeMem(int *bot, int *top);
extern int NuFileLoadBuffer(char *name, void *buf, int max);
extern void *NuMemAllocFn(int size, char *file, int line);
extern void NuMemFreeFn(void *ptr, char *file, int line);
extern void *memset(void *dst, int value, unsigned int size);
typedef void (*NuErrorFunc)(const char *fmt, ...);
extern NuErrorFunc NuErrorProlog(const char *file, int line);
extern u8 *vpDmaTag_CntEx(u8 *cur, unsigned int qwc, unsigned int tag);
extern u8 *vpDmaTag_Close(u8 *cur);
extern u8 *vtBeginDirect(u8 *cur, int flags);
extern u8 *vtCloseDirect(u8 *cur);
extern u8 *gsBeginGifPacket(u8 *cur, int eop, int nloop, int pre, int prim,
                            int flg, int nreg, ...);
extern u8 *gsCloseGifPacket(u8 *cur);
extern u8 *Ps2TxSetStateData(u8 *cur, NuPs2Tex *tex, NuPs2Tex *pal, int mode);
extern u8 *Ps2TxSetSurfaceStateData(u8 *cur, int surface);


u64 *NuTexWriteMaterialSettings(u64 *dst, NuMtl *mtl) {
    int k;
    int l;
    int filter;
    int maxlevel;
    NuPs2Tex *ps2tex;

    if (numtl_force_mipmap) {
        k = sys_mm_k;
        l = sys_mm_l;
    } else {
        k = mtl->mm_k;
        l = mtl->mm_l;
    }
    filter = (int)(mtl->attrib >> 36) & 3;
    if (mtl->tid > 0) {
        ps2tex = &TexList[mtl->tid - 1].ps2tex;
        if (filter == 2) {
            maxlevel = ps2tex->mipcount - 1;
        } else {
            maxlevel = 0;
        }
    } else {
        maxlevel = 0;
    }
    *dst++ = 1;
    *dst++ = 0x3F;
    *dst++ = (u64)(maxlevel >= 0 ? maxlevel : 0) << 2 | (u64)nutex_mag_filter[filter] << 5
           | (u64)nutex_min_filter[filter] << 6 | (u64)l << 19 | (u64)k << 32;
    if (mtl->gs_context) {
        *dst++ = 0x15;
    } else {
        *dst++ = 0x14;
    }
    *dst++ = nutex_wrap_mode[(int)(mtl->attrib >> 40) & 3] | (u64)nutex_wrap_mode[(int)(mtl->attrib >> 42) & 3] << 2;
    if (mtl->gs_context) {
        *dst++ = 9;
    } else {
        *dst++ = 8;
    }
    return dst;
}


int NuTexGetSysBuffSize(int count) {
    return count * sizeof(NuTex);
}


inline void NuTexInitEx(char **buff, int count) {
    int size;
    NuTex *list;

    if (nutex_initialised) {
        NuTexClose();
    }
    ps2videoGetFreeMem(&gs_bot, 0);
    size = count * sizeof(NuTex);
    list = (NuTex *)(((int)*buff + 15) & 0xFFFFFFF0);
    gs_botfree = gs_bot;
    gs_top = 0x4000;
    gs_topfree = 0x4000;
    gs_flush_cnt = 0;
    gs_frame = 0;
    gs_wrap_frame = 0;
    *buff = (char *)list;
    TexList = list;
    memset(list, 0, size);
    *buff += size;
    nutex_max = count;
    nutex_initialised = 1;
    nutex_cnt = 0;
    nutex_alloc_cnt = 0;
}


void NuTexInit(void) {
    char *buff;

    if (!nutex_sysbuff) {
        nutex_sysbuff = NuMemAllocFn(NUTEX_MAX * sizeof(NuTex), nutex_srcfile, 0x1A2);
    }
    buff = nutex_sysbuff;
    NuTexInitEx(&buff, NUTEX_MAX);
}


int NuTexReserve(int size) {
    int addr;

    if (gs_reserve_sp >= 256) {
        return 0;
    }
    size += 255;
    gs_reserve_stack[gs_reserve_sp++] = gs_botfree;
    addr = (gs_botfree + 31) & 0xFFFFFFE0;
    gs_botfree = addr + size / 256;
    return addr;
}


void NuTexUnReserve(void) {
    if (gs_reserve_sp) {
        gs_reserve_sp--;
        gs_botfree = gs_reserve_stack[gs_reserve_sp];
    }
}


int NuTexReadBitmap(char *name) {
    return NuTexReadBitmapMMEx(name, 0, 0, 0);
}


int NuTexReadBitmapMM(char *name, int mipmaps) {
    return NuTexReadBitmapMMEx(name, mipmaps, 0, 0);
}


int NuTexCreate(NuTex *spec) {
    return NuTexCreateEx(spec, 0, 0);
}


void NuTexDestroy(int tex) {
    tex--;
    if (tex >= 0) {
        if (!(TexList[tex].flags & NUTEX_NATIVE)) {
            if (TexList[tex].bits) {
                NuMemFreeFn(TexList[tex].bits, nutex_srcfile, 0x3F9);
                TexList[tex].bits = 0;
            }
            NuPs2TxDestroy(&TexList[tex].ps2tex);
        }
        TexList[tex].flags &= ~NUTEX_VALID;
        nutex_alloc_cnt--;
    }
}



void NuTexClose(void) {
    int i;

    for (i = 0; i < nutex_max; i++) {
        if (TexList[i].bits != 0) {
            NuMemFreeFn(TexList[i].bits, nutex_srcfile, 0x1B0);
            TexList[i].bits = 0;
        }
        if (TexList[i].flags & NUTEX_VALID) {
            NuTexDestroy(i + 1);
        }
    }
    nutex_initialised = nutex_alloc_cnt = nutex_cnt = 0;
}

short NuTexRef(int tex) {
    tex--;
    if (tex >= 0) {
        return ++TexList[tex].refcount;
    }
    return 0;
}


short NuTexUnRef(int tex) {
    tex--;
    if (tex >= 0) {
        return --TexList[tex].refcount;
    }
    return 0;
}


int NuTexType(int tex) {
    tex--;
    if (tex >= 0) {
        return TexList[tex].type;
    }
    return 0;
}


int NuTexWidth(int tex) {
    tex--;
    if (tex >= 0) {
        return TexList[tex].width;
    }
    return 0;
}


int NuTexHeight(int tex) {
    tex--;
    if (tex >= 0) {
        return TexList[tex].width;
    }
    return 0;
}


int NuTexHasAlpha(int tex) {
    int type;

    tex--;
    if (tex >= 0) {
        type = TexList[tex].type;
        if (type == 1 || type == 3) {
            return 1;
        }
    }
    return 0;
}


int NuTexAlphaBits(int type) {
    if (type == 1) goto b4;
    if (type == 0) goto b0;
    if (type == 2) goto b0;
    if (type == 3) goto b8;
    goto def;
b0:
    return 0;
b4:
    return 4;
b8:
    return 8;
def:
    return 0;
}


inline int NuTexPixelSize(int type) {
    switch (type) {
    case 4:
        return 4;
    case 5:
        return 8;
    case 0:
    case 1:
        return 16;
    case 2:
        return 24;
    case 3:
        return 32;
    }
    return 0;
}


inline int NuTexImgSize(int type, int width, int height) {
    return (width * height * NuTexPixelSize(type)) >> 3;
}


int NuTexImgSizeMM(int type, int width, int height, int levels) {
    int size;
    int level;

    size = NuTexImgSize(type, width, height);
    level = 0;
    while (level < levels) {
        level++;
        size += ((width >> level) * (height >> level) * NuTexPixelSize(type)) >> 3;
    }
    return size;
}


int NuTexPalSize(int type) {
    switch (type) {
    case 5:
        return 0x400;
    case 4:
        return 0x40;
    }
    return 0;
}


int NuTexRecoverNativeData(int tex, void *dest, int size) {
    return NuTexRecoverNativeDataEx(tex, dest, size, 1, 1, 1);
}


int NuTexReadNative(char *name, char *buff, int size, int *loaded) {
    int pad;
    int len;

    pad = -((int)buff & 15) & 15;
    buff += pad;
    len = pad + NuFileLoadBuffer(name, buff, size - pad);
    if (len) {
        if (loaded) {
            *loaded = len;
        }
        return NuTexCreateNative(buff);
    }
    return 0;
}


int NuTexGetPointers(int tex, NuTex **entry, NuPs2Tex **ps2tex) {
    tex--;
    if (tex >= 0 && (TexList[tex].flags & NUTEX_VALID)) {
        if (entry) {
            *entry = &TexList[tex];
        }
        if (ps2tex) {
            *ps2tex = &TexList[tex].ps2tex;
        }
        return 1;
    }
    return 0;
}


void NuTexFlushTemp(void) {
    gs_tempfree = gs_botfree;
}


void NuTexAssignAddr(int tex, int addr) {
    if (tex > 0) {
        tex--;
        TexList[tex].ps2tex.gsaddr = addr;
    }
}


void NuTexAssignTempAddr(int tex) {
    if (tex > 0) {
        tex--;
        gs_tempfree = (gs_tempfree + 31) & 0xFFFFFFE0;
        TexList[tex].ps2tex.gsaddr = gs_tempfree;
        gs_tempfree += TexList[tex].ps2tex.reqsize[0];
    }
}


int NuTexGetReqSize(int tex, int level) {
    if (tex > 0) {
        tex--;
        return TexList[tex].ps2tex.reqsize[level] << 8;
    }
    return 0;
}


void NuTexCloneTempAddr(int dst, int src) {
    if (dst > 0 && src > 0) {
        dst--;
        src--;
        TexList[dst].ps2tex.gsaddr = TexList[src].ps2tex.gsaddr;
    }
}


void NuTexPalChange(int tex) {
    if (tex > 0) {
        NuPs2ChangeTexPal(&TexList[tex - 1].ps2tex);
    }
}


void NuTexPalChangeStreamTemp(int tex) {
    if (tex > 0) {
        NuPs2ChangeTexPalStreamTemp(&TexList[tex - 1].ps2tex);
    }
}


u64 NuTexGetTex0(int tex) {
    tex--;
    if (tex >= 0 && (TexList[tex].flags & NUTEX_VALID)) {
        return Ps2TxGetTex0(&TexList[tex].ps2tex);
    }
    return 0;
}


u8 *NuTexSetPal(u8 *cur, int tid, int paltid, s8 mode) {
    NuPs2Tex *tex;
    NuPs2Tex *pal;

    if (tid > 0 && paltid > 0) {
        if (TexList[tid - 1].flags & NUTEX_VALID) {
            tex = &TexList[tid - 1].ps2tex;
            pal = &TexList[paltid - 1].ps2tex;
            cur = vpDmaTag_CntEx(cur, 0x13000000, 0x50000000);
            cur[-0xE] = 2;
            cur = vtBeginDirect(cur - 4, 0);
            cur = gsBeginGifPacket(cur, 0, 1, 0, 0, 0, 1, 0xE);
            cur = Ps2TxSetStateData(cur, tex, pal, mode);
            cur = gsCloseGifPacket(cur);
            cur = vtCloseDirect(cur);
            cur = vpDmaTag_Close(cur);
        } else {
            NuErrorProlog(nutex_srcfile, 0x5A4)(msg_tex_invalid);
        }
    }
    return cur;
}

static inline int NuTexFindFree(void) {
    int start;

    start = nutex_cnt;
    while (TexList[nutex_cnt].flags & NUTEX_VALID) {
        if (start == ++nutex_cnt) {
            return -1;
        }
        if (nutex_cnt >= nutex_max) {
            nutex_cnt = 0;
        }
    }
    nutex_alloc_cnt++;
    return nutex_cnt;
}

int NuTexCreateStream(NuTex *spec) {
    int tex;

    tex = NuTexFindFree();
    if (tex >= 0) {
        TexList[tex].spec = spec->spec;
        if (NuPs2TxCreate(spec, &TexList[tex].ps2tex)) {
            TexList[tex].bits = 0;
            TexList[tex].valid = 1;
            TexList[tex].native = (spec == 0);
            return tex + 1;
        }
    }
    return 0;
}

u8 *NuTexSetRenderTarget(u8 *cur, int tid) {
    NuTex *tex;
    int addr;

    if (TexList[tid - 1].flags & NUTEX_VALID) {
        tex = &TexList[tid - 1];
        cur = vpDmaTag_CntEx(cur, 0x13000000, 0x50000000);
        cur[-0xE] = 0xA;
        cur = vtBeginDirect(cur - 4, 0);
        cur = gsBeginGifPacket(cur, 0, 1, 0, 0, 0, 1, 0xE);
        addr = tex->ps2tex.texaddr;
        if (!addr) {
            addr = tex->ps2tex.gsaddr;
            if (!addr) {
                addr = gs_botfree;
            }
        }
        *(u64 *)cur = 0;
        cur += 8;
        *(u64 *)cur = 0x3F; /* TEXFLUSH */
        cur += 8;
        *(u64 *)cur = (addr >> 5) | ((u64)tex->ps2tex.tbw << 16) | ((u64)tex->ps2tex.psm << 24);
        cur += 8;
        *(u64 *)cur = 0x4C; /* FRAME_1 */
        cur += 8;
        cur = gsCloseGifPacket(cur);
        cur = vtCloseDirect(cur);
        cur = vpDmaTag_Close(cur);
        nurndr_pixel_width = tex->width;
        nurndr_pixel_height = tex->height;
        nurndr_pixel_p2w = tex->ps2tex.p2w;
        nurndr_pixel_p2h = tex->ps2tex.p2h;
    }
    cur_rt = (addr >> 5) | ((u64)tex->ps2tex.tbw << 16) | ((u64)tex->ps2tex.psm << 24);
    return cur;
}

u8 *NuTexSet(u8 *cur, int tid, s8 mode) {
    NuPs2Tex *tex;

    if (tid > 0) {
        if (TexList[tid - 1].flags & NUTEX_VALID) {
            tex = &TexList[tid - 1].ps2tex;
            cur = vpDmaTag_CntEx(cur, 0x13000000, 0x50000000);
            cur[-0xE] = 2;
            cur = vtBeginDirect(cur - 4, 0);
            cur = gsBeginGifPacket(cur, 0, 1, 0, 0, 0, 1, 0xE);
            cur = Ps2TxSetStateData(cur, tex, 0, mode);
            cur = gsCloseGifPacket(cur);
            cur = vtCloseDirect(cur);
            cur = vpDmaTag_Close(cur);
        } else {
            NuErrorProlog(nutex_srcfile, 0x57A)(msg_tex_invalid);
        }
    } else if (tid < 0) {
        cur = vpDmaTag_CntEx(cur, 0x11000000, 0x50000000);
        cur = vtBeginDirect(cur - 4, 0);
        cur = gsBeginGifPacket(cur, 0, 1, 0, 0, 0, 1, 0xE);
        cur = Ps2TxSetSurfaceStateData(cur, tid);
        cur = gsCloseGifPacket(cur);
        cur = vtCloseDirect(cur);
        cur = vpDmaTag_Close(cur);
    }
    return cur;
}

void *NuTexSetZBuffer(void *stream, int tex) {
    NuTex *entry;
    int addr;

    if (TexList[tex - 1].flags & NUTEX_VALID) {
        entry = &TexList[tex - 1];
        addr = entry->ps2tex.texaddr;
        if (!addr) {
            addr = gs_botfree;
        }
        NuRndrStreamSetZBState(addr, entry->ps2tex.psm, 0);
        stream = NuRndrStreamAddZBState(stream, 1);
    }
    return stream;
}


int NuTexGetTextAddr(int tex) {
    NuTex *entry = &TexList[tex - 1];

    if (entry->flags & NUTEX_VALID) {
        return entry->ps2tex.texaddr ? entry->ps2tex.texaddr : gs_botfree;
    }
    return 0;
}

/* Upload texture `tid` from inside a render stream. With PATH3 the upload is
 * bracketed by two interrupting tags and chained onto the stream's list of
 * texture uploads. */
void NuTexAccomodateRS(struct nurndrstream_s *stream, int tid)
{
    NuDmaTag *open;

    gs_topfree = stream->topfree;
    if (nutex_use_path3) {
        /* Opening tag: interrupts, and is later pointed past the upload. */
        open = (NuDmaTag *)stream->cur;
        stream->cur = vpDmaTag_Next(stream->cur, 0);
        TAG_BEFORE(stream->cur)->id |= DMATAG_IRQ;
        TAG_BEFORE(stream->cur)->mark = 0x18;
        MARK_AT(stream->cur)->tag = 0;
        MARK_AT(stream->cur)->magic = 0xACEB00B5;
        stream->cur += sizeof(NuDmaTag);
        if (stream->txtr_end != 0) {
            stream->txtr_end->addr = stream->cur;
        } else {
            stream->txtr_start = (NuDmaTag *)stream->cur;
        }

        stream->cur = NuTexAccomodate(stream->cur, tid);

        /* Closing tag: interrupts, and points back at the opening one. */
        stream->txtr_end = (NuDmaTag *)stream->cur;
        stream->cur = vpDmaTag_Next(stream->cur, 0);
        TAG_BEFORE(stream->cur)->id |= DMATAG_IRQ;
        TAG_BEFORE(stream->cur)->mark = 0x19;
        MARK_AT(stream->cur)->tag = open;
        MARK_AT(stream->cur)->magic = 0xBABEF00D;
        stream->cur += sizeof(NuDmaTag);
        open->addr = stream->cur;
    } else {
        stream->cur = NuTexAccomodate(stream->cur, tid);
    }
    stream->topfree = gs_topfree;
}
