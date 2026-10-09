/*
 * Unit: nu3d/nurndr
 *
 * Functions:
 *   0x0011f030 NuRndrSetZBuffer
 *   0x0011f1b8 NuRndrSetRTarget
 *   0x0011f3d0 NuRndrClear
 *   0x0011f870 NuRndrGradClear
 *   0x0011fe28 NuRndrGScnObj
 *   0x00120240 NuRndrGobj
 *   0x00120590 NuRndrFxBlueAndGreenData
 *   0x001207d0 NuRndrFxSpriteSet
 *   0x00120a78 NuRndrFxSprite2Set
 *   0x00120e60 NuRndrFxCode
 *   0x00121220 NuRndrGobjSkin
 *   0x00121498 NuRndrShadowInit
 *   0x00121628 NuRndrStreamQuad3dClip
 *   0x00121988 NuRndrOTInit
 *   0x00121a98 NuRndrOTInsertTriBatch
 *   0x00121ee0 NuRndrOTFlush
 *   0x00122068 NuRndrStreamTriStrip3dClip
 *   0x00122458 NuRndrStreamTri3dClip
 *   0x00122850 NuRndrCylCapMakeData
 *   0x00122b88 NuRndrCylMakeData
 *   0x00122da8 NuRndrShadowCylinderCap
 *   0x00123210 NuRndrShadowEllipse
 *   0x00123660 NuRndrShadowPols
 *   0x001241b8 NuRndrShadowCylinderCap2
 *   0x00124f40 NuRndrShadowStack
 *   0x00125180 NuHGobjRndrMtxDwa
 *   0x00125608 NuHGobjEval
 *   0x00125918 NuHGobjEvalAnim
 *   0x00125c10 NuHGobjEvalAnim2
 *   0x00125f30 NuHGobjRndrAnim
 *   0x00126558 NuHGobjEvalDwa
 *   0x00126758 NuHGobjEvalDwa2
 *   0x00126900 NuHGobjEvalDwaBlend
 *   0x00126c28 NuHGobjEvalDwaBlend2
 *   0x00126ee0 NuHGobjEvalAnimBlend
 *   0x001271e8 NuHGobjEvalAnimBlend2
 *   0x00127530 NuHGobjRndrAnimBlend
 *   0x001279b0 NuRndrLine2d
 *   0x00127c08 NuRndrLineStrip2d
 *   0x00127e70 ClipLinePoint
 *   0x00128138 NuRndrLine3d
 *   0x00128578 NuRndrTri2d
 *   0x001287d0 NuRndrTriStrip2d
 *   0x00128a38 NuRndrTriStrip3d
 *   0x00128c48 NuRndrRect2di
 *   0x00128f40 NuRndrGradRect2di
 *   0x001293c0 NuRndrRectUV2di
 *   0x001296e8 NuRndrLine2di
 *   0x00129940 NuRndrLineRect2di
 *   0x00129ca0 NuRndrTri2di
 *   0x00129f68 NuRndrTriStrip2di
 *   0x0012a218 NuRndrLineStrip2di
 *   0x0012a4c8 NuRndrQuad2d
 *   0x0012a730 NuRndrTriStripGif
 *   0x0012a9c0 NuRndrOldFaceOn
 *   0x0012ae38 NuRndrFaceOn
 *   0x0012b270 NuRndrFaceY
 *   0x0012b610 NuRndrRectUV3di
 *   0x0012b870 NuRndrRectUVStream
 *   0x0012bac0 NuRndrWater
 *   0x0012cbf8 NuRndrShadow
 *   0x0012d238 NuRndrShadowLightFan
 *   0x0012d5b0 NuRndrShadowStencil
 *   0x0012db68 NuRndrTrail
 *   0x0012e200 NuRndrAddFootPrint
 *   0x0012e560 NuRndrFootPrints
 *   0x0012e810 NuRndrStreamQuadShad
 *   0x0012ee58 NuRndrWaterRippleUpdate
 *   0x0012ef88 NuRndrStreamQuadWRip
 *   0x0012f640 NuHGobjRndr
 *   0x0012f6c0 NuHGobjRndrMtx
 *   0x0012f6e0 NuRndrInitEx
 *   0x0012f770 NuRndrInit
 *   0x0012f800 NuRndrClose
 *   0x0012f808 NuRndrBeginScene
 *   0x0012f8c8 NuRndrBeginSceneEx
 *   0x0012f990 NuRndrEndScene
 *   0x0012f9c0 NuRndrEndSceneEx
 *   0x0012fa10 NuRndrSwapScreen
 *   0x0012fa98 NuRndrGlobalMtl
 *   0x0012faa0 NuRndrFogColour
 *   0x0012faa8 NuRndrFogDistance
 *   0x0012fab0 NuRndrFogMode
 *   0x0012fab8 NuRndrParticleGroup
 *   0x0012fc68 NuRndrStarfield
 *   0x0012fc98 NuRndrRect
 *   0x0012fdf0 NuRndrLine3dDbg
 *   0x0012fdf8 NuRndrLine3dDbgFlush
 *   0x0012fe00 NuRndrTri3dClip
 *   0x0012fe28 NuRndrTriStrip3dClip
 *   0x0012fe50 NuRndrQuad3d
 *   0x0012fe58 NuRndrFlush
 *   0x0012fea8 NuRndrShadowOnOff
 *   0x0012feb0 NuRndrRectUV3diNS
 *   0x00130000 NuRndrFx
 *   0x00130078 NuRndrShadowDirCol
 *   0x001300b8 NuRndrWaterLevelTint
 *   0x001300c8 NuRndrAddShadow
 *   0x001301a0 NuRndrShadPolys
 *   0x00130230 NuRndrAddWaterRipple
 *   0x001302a8 NuRndrWaterRip
 *   0x001303a0 NuRndrInitWorld
 *   0x001303d0 NuRndrSplineSearch
 *   0x00130418 NuRndrCreateBlendShapeDeformerWeightsArray
 *   0x001304a0 NuRndrDeformerWeightsArrayInit
 *   0x001304e8 NuRndrFxBlueOrGreenData
 *   0x00130638 NuRndrOTInsertItem
 *   0x00130690 OTTriInit
 *   0x001306c0 OTTriInsert
 *   0x00130740 GlobalCam3DNClip
 *   0x001307c0 RotateCyl
 *   0x00130888 DeRotateVectorAngles
 *   0x00130920 NuRndrTest
 *   0x00130928 NuRndrAnglesZX
 */

typedef unsigned int u32;

extern struct numtl_s *D_00633078;
#define GlobalMtl D_00633078

extern int nurndr_shadowson;

extern struct numtl_s *numtl_defaultmtl3d;

extern float NuRndrWaterLevel;
extern u32 NuRndrWaterTint;
extern int NuRndrShadMaskCount;

typedef struct nurndrfoot_s {
    char unk00[0x32];
    char active;
    char unk33[5];
} nurndrfoot_s;

extern nurndrfoot_s NuRndrFootData[];

typedef void (*NuErrorFunc)(const char *fmt, ...);
extern NuErrorFunc NuErrorProlog(const char *file, int line);
extern char D_00614770[];
extern char D_00614790[];

extern int NuHGobjRndrMtxDwa(void *hobj, void *mtx, int a2, int a3, void *dwa, int blend);
extern int NuRndrStreamTri3dClip(void *verts, int num, void *mtx, void *strm);
extern int NuRndrStreamTriStrip3dClip(void *verts, int num, void *mtx, void *strm);
extern void NuMtlBuildRenderList(void);
extern void NuRndrOTFlush(void);
extern void NuRndrStreamFlush(void);

typedef struct { float x, y, z; } NuVec3;

extern void *memset(void *s, int c, unsigned int n);
extern char ot_tri[];
extern int ot_highest;

extern NuVec3 curlight;
extern u32 curlightcol;
extern int NRFanData;
extern int NuRndrShadowLightFan(void);

extern int NuRndrShadMasks[];
extern unsigned char D_0062ECA8[];
extern int NuSplineFindAllSub(void *spline, void *buf, int *masks, int maxcount);

extern unsigned int D_00689A80[];
extern u32 *vpDmaTag_RetEx(void *buf, u32 a1, u32 a2);
extern u32 *vpDmaTag_Close(void *p);

/* Only the packet cursor at offset 4 is used by the 2D packet writers. */
struct RndrPacketStream {
    void *unknown_00;
    u32 *cur;
};

extern struct RndrPacketStream *rndrstream_2d;
extern u32 *rndrstream_free;
extern struct numtl_s *numtl_defaultmtl2d;
extern int PS2_REZ_W;
extern int PS2_REZ_H;
extern int PS2_VCNTR_X;
extern int PS2_VCNTR_Y;

extern void NuRndrStreamLink(struct RndrPacketStream *stream);
extern void NuRndrStreamSetMisc(struct RndrPacketStream *stream);
extern void NuRndrStreamAddMtl(struct RndrPacketStream *stream, struct numtl_s *mtl);
extern u32 *vpDmaTag_CntEx(u32 *packet, u32 vif0, u32 vif1);
extern u32 *vpDmaTag_Next(u32 *packet, u32 address);
extern u32 *vtBeginDirect(u32 *packet, u32 irq);
extern u32 *vtCloseDirect(u32 *packet);
extern u32 *gsBeginGifPacket(u32 *packet, int nloop, int eop, int pre,
                             int prim, int flg, int nreg, ...);
extern u32 *gsCloseGifPacket(u32 *packet);

/* Eight entries per channel; names of the sprite parameters remain unknown. */
struct FxSprite2 {
    float parameter_00;
    int parameter_04;
    int count;
    float parameter_0c;
    float parameter_10;
    float parameter_14;
    float parameter_18;
    int parameter_1c[8];
    int colour[8];
    float channel[24][8];
};

extern u32 *NuRndrStreamAlloc(int count);

u32 *NuRndrFxSprite2Set(u32 code, struct FxSprite2 *fx) {
    /* The target reserves two quadwords for its GIF tag scratch. */
    u32 tag[8];
    u32 *base;
    u32 *packet;
    int i;

    base = NuRndrStreamAlloc(fx->count * 8 + 8);
    packet = vpDmaTag_RetEx(base, 0x13000000,
        ((fx->count * 8 + 4) << 16) | 0x6c008000);
    *(unsigned long long *)tag = 0x1003400000008001ULL;
    *packet++ = tag[0];
    *packet++ = tag[1];
    *packet++ = 1;
    *packet++ = 0;
    *(unsigned long long *)tag = 0x4403400000008001ULL;
    *packet++ = tag[0];
    *packet++ = tag[1];
    *packet++ = 0x5252;
    *packet++ = 0;
    *packet++ = (u32)fx->parameter_00;
    *packet++ = 0;
    *packet++ = fx->parameter_04;
    *packet++ = fx->count;
    *(float *)packet++ = fx->parameter_14;
    *(float *)packet++ = fx->parameter_18;
    *(float *)packet++ = fx->parameter_0c;
    *(float *)packet++ = fx->parameter_10;

    for (i = 0; i < fx->count; i++) {
        *packet++ = 0;
        *packet++ = 0;
        *packet++ = 0;
        *packet++ = fx->parameter_1c[i];
        *packet++ = fx->colour[i];
        *packet++ = fx->colour[i] >> 8;
        *packet++ = (short)(fx->colour[i] >> 16);
        *packet++ = (signed char)(fx->colour[i] >> 24);
        *(float *)packet++ = fx->channel[2][i];
        *(float *)packet++ = fx->channel[3][i];
        *(float *)packet++ = fx->channel[0][i];
        *(float *)packet++ = fx->channel[1][i];
        *(float *)packet++ = fx->channel[6][i];
        *(float *)packet++ = fx->channel[7][i];
        *(float *)packet++ = fx->channel[4][i];
        *(float *)packet++ = fx->channel[5][i];
        *(float *)packet++ = fx->channel[10][i];
        *(float *)packet++ = fx->channel[11][i];
        *(float *)packet++ = fx->channel[8][i];
        *(float *)packet++ = fx->channel[9][i];
        *(float *)packet++ = fx->channel[14][i];
        *(float *)packet++ = fx->channel[15][i];
        *(float *)packet++ = fx->channel[12][i];
        *(float *)packet++ = fx->channel[13][i];
        *(float *)packet++ = fx->channel[18][i];
        *(float *)packet++ = fx->channel[19][i];
        *(float *)packet++ = fx->channel[16][i];
        *(float *)packet++ = fx->channel[17][i];
        *(float *)packet++ = fx->channel[22][i];
        *(float *)packet++ = fx->channel[23][i];
        *(float *)packet++ = fx->channel[20][i];
        *(float *)packet++ = fx->channel[21][i];
    }

    *packet++ = code | 0x14000000;
    *packet++ = 0x13000000;
    *packet++ = 0;
    *packet++ = 0;
    vpDmaTag_Close(packet);
    return base;
}


void NuRndrGradRect2di(int x, int y, int width, int height,
                      unsigned char *colours, struct numtl_s *mtl) {
    x += (PS2_VCNTR_X - (PS2_REZ_W >> 1)) << 4;
    y += (PS2_VCNTR_Y - (PS2_REZ_H >> 1)) << 4;
    if (!mtl) {
        mtl = numtl_defaultmtl2d;
    }

    NuRndrStreamLink(rndrstream_2d);
    NuRndrStreamSetMisc(rndrstream_2d);
    NuRndrStreamAddMtl(rndrstream_2d, mtl);
    rndrstream_2d->cur = vpDmaTag_CntEx(rndrstream_2d->cur,
                                      0x10000000, 0x50000000) - 1;
    rndrstream_2d->cur = vtBeginDirect(rndrstream_2d->cur, 0);
    rndrstream_2d->cur = gsBeginGifPacket(rndrstream_2d->cur,
        0, 1, 1, 4, 0, 9, 1, 2, 5, 1, 5, 1, 5, 1, 5);

    *rndrstream_2d->cur++ = colours[0];
    *rndrstream_2d->cur++ = colours[1];
    *rndrstream_2d->cur++ = colours[2];
    *rndrstream_2d->cur++ = colours[3];
    *rndrstream_2d->cur++ = 0;
    *rndrstream_2d->cur++ = 0;
    *(float *)rndrstream_2d->cur++ = 1.0f;
    rndrstream_2d->cur++;
    *rndrstream_2d->cur++ = x;
    *rndrstream_2d->cur++ = y;
    *rndrstream_2d->cur++ = 0;
    *rndrstream_2d->cur++ = 0;

    *rndrstream_2d->cur++ = colours[4];
    *rndrstream_2d->cur++ = colours[5];
    *rndrstream_2d->cur++ = colours[6];
    *rndrstream_2d->cur++ = colours[7];
    *rndrstream_2d->cur++ = x + width;
    *rndrstream_2d->cur++ = y;
    *rndrstream_2d->cur++ = 0;
    *rndrstream_2d->cur++ = 0;

    *rndrstream_2d->cur++ = colours[8];
    *rndrstream_2d->cur++ = colours[9];
    *rndrstream_2d->cur++ = colours[10];
    *rndrstream_2d->cur++ = colours[11];
    *rndrstream_2d->cur++ = x;
    *rndrstream_2d->cur++ = y + height;
    *rndrstream_2d->cur++ = 0;
    *rndrstream_2d->cur++ = 0;

    *rndrstream_2d->cur++ = colours[12];
    *rndrstream_2d->cur++ = colours[13];
    *rndrstream_2d->cur++ = colours[14];
    *rndrstream_2d->cur++ = colours[15];
    *rndrstream_2d->cur++ = x + width;
    *rndrstream_2d->cur++ = y + height;
    *rndrstream_2d->cur++ = 0;
    *rndrstream_2d->cur++ = 0;

    rndrstream_2d->cur = gsCloseGifPacket(rndrstream_2d->cur);
    rndrstream_2d->cur = vtCloseDirect(rndrstream_2d->cur);
    rndrstream_2d->cur = vpDmaTag_Close(rndrstream_2d->cur);
    rndrstream_free = vpDmaTag_Next(rndrstream_2d->cur, 0);
}


int NuHGobjRndrMtx(void *hobj, void *mtx, int a2, int a3, void *dwa) {
    return NuHGobjRndrMtxDwa(hobj, mtx, a2, a3, dwa, 0);
}


void NuRndrClose(void) {
}


void NuRndrEndScene(void) {
    NuMtlBuildRenderList();
    NuRndrOTFlush();
    NuRndrStreamFlush();
}


void NuRndrEndSceneEx(int flags) {
    if (flags & 1) {
        NuMtlBuildRenderList();
    }
    if (flags & 2) {
        NuRndrOTFlush();
    }
    NuRndrStreamFlush();
}


void NuRndrGlobalMtl(struct numtl_s *mtl) {
    GlobalMtl = mtl;
}


void NuRndrFogColour(u32 colour) {
}


void NuRndrFogDistance(float near, float far) {
}


void NuRndrFogMode(u32 mode) {
}


void NuRndrStarfield(void) {
    NuErrorProlog(D_00614770, 0x5D5)(D_00614790);
}


void NuRndrLine3dDbg(u32 colour, float x0, float y0, float z0, float x1, float y1, float z1) {
}


void NuRndrLine3dDbgFlush(void) {
}


int NuRndrTri3dClip(void *verts, int num, void *mtx, struct numtl_s *mtl) {
    struct numtl_s *m;

    m = mtl ? mtl : numtl_defaultmtl3d;
    return NuRndrStreamTri3dClip(verts, num, mtx, (char *)m + 0x124);
}


int NuRndrTriStrip3dClip(void *verts, int num, void *mtx, struct numtl_s *mtl) {
    struct numtl_s *m;

    m = mtl ? mtl : numtl_defaultmtl3d;
    return NuRndrStreamTriStrip3dClip(verts, num, mtx, (char *)m + 0x124);
}


int NuRndrQuad3d(void *verts, struct numtl_s *mtl) {
    return 1;
}


void NuRndrFlush(int flags) {
    if (flags & 1) {
        NuMtlBuildRenderList();
    }
    if (flags & 2) {
        NuRndrOTFlush();
    }
    NuRndrStreamFlush();
}


void NuRndrShadowOnOff(int on) {
    nurndr_shadowson = on;
}


void NuRndrShadowDirCol(NuVec3 *dir, u32 col) {
    curlight = *dir;
    curlightcol = col;
    NRFanData = NuRndrShadowLightFan();
}


void NuRndrWaterLevelTint(float level, u32 tint) {
    NuRndrWaterLevel = level;
    NuRndrWaterTint = tint;
}


void NuRndrInitWorld(void) {
    int i;

    NuRndrShadMaskCount = 0;
    for (i = 0; i < 64; i++) {
        NuRndrFootData[i].active = 0;
    }
}


void NuRndrSplineSearch(void *spline) {
    int n;

    n = NuRndrShadMaskCount;
    NuRndrShadMaskCount += NuSplineFindAllSub(spline, D_0062ECA8, &NuRndrShadMasks[n], 32 - n);
}


void NuRndrDeformerWeightsArrayInit(void) {
    u32 *p;

    p = vpDmaTag_RetEx(D_00689A80, 0x13000000, 0x01000101);
    *p++ = 0x60010063;
    *p = 0;
    vpDmaTag_Close(p + 1);
}


void OTTriInit(void) {
    memset(ot_tri, 0, 0x4000);
    ot_highest = 0;
}


void NuRndrTest(void) {
}
