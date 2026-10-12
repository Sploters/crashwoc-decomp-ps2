/*
 * Unit: gamelib/debris
 *
 * Functions:
 *   0x00182ca8 GenericDebinfoDmaTypeUpdate
 *   0x00183418 GenDebIndex
 *   0x00183660 GenDebIndexRadial
 *   0x001838d0 GenDebIndexRadialRotor
 *   0x00183b98 GenDebIndexSpheroid
 *   0x00183e58 GenDebIndexBounceY
 *   0x00184328 GenDebIndexBounceXZ
 *   0x00184790 GenDebIndexPos
 *   0x00184948 GenDebIndexPosRandTime
 *   0x00184b20 GenDebIndexWaterFall
 *   0x00184d68 GenDebIndexWaterFallSplash
 *   0x00184f70 CreateAlphaBlendTexture256
 *   0x00185208 SplashEffect
 *   0x00185388 SplashBodyEffect
 *   0x00185510 SetupDebris
 *   0x00185fa0 DebReAlloc2
 *   0x001864b8 DebFree
 *   0x001867d8 DebFreeInstantly
 *   0x00186960 DebrisStartOffset
 *   0x00186a68 AddVariableShotDebrisEffectMtx2
 *   0x00187098 AddDebrisEffect
 *   0x00187568 DebrisDraw
 *   0x00187800 Debris
 *   0x00188b40 DebrisCollisionCheck
 *   0x00188e08 AddDebEff
 *   0x00188eb0 DebrisOff
 *   0x00188ee0 DebrisOn
 *   0x00188f10 AddFiniteShotDebrisEffect
 *   0x00188f80 AddVariableShotDebrisEffect
 *   0x00189090 AddVariableShotDebrisEffectMtx
 *   0x001891a0 DebReAlloc
 *   0x00189200 DebrisOrientation
 *   0x001892f8 DebrisOrientationMtx
 *   0x001893b8 DebrisPosOrientationMtx
 *   0x001894a0 DebrisEmitterOrientation
 *   0x00189598 DebrisEmiterPos
 *   0x001895d0 DebrisReflectionOrientation
 *   0x00189620 DebrisSetTrigger
 *   0x00189660 DebrisSetGroupID
 *   0x00189690 DebrisSetup
 *   0x001896b0 DebrisSetRenderGroup
 *   0x001896b8 LookupDebrisEffect
 *   0x00189730 DebrisRegisterCutoffCameraVec
 *   0x00189738 CameraEmitterDistance
 *   0x00189768 DebrisFindAllOfType
 *   0x001897f8 FindDebrisEffectStack
 *   0x00189858 RemoveDebrisEffectFromStack
 *   0x001898a0 AddChunkControlToStack
 *   0x001898d0 SolveQuadratic
 *   0x001899b8 GenDebIndexSort
 *   0x001899d8 GenDebMomAdjFromPos
 *   0x00189a38 GenDebMomAdjFromPosAll
 *   0x00189a68 GenDebMomAdjFromPosRev
 *   0x00189a88 GenDebMomAdjFromSplash
 *   0x00189ab0 GenDebMomAdjFromAshRock
 *   0x00189ad8 GenDebMomAdjFromPosRevTree
 *   0x00189bc0 CreateCopyMat
 *   0x00189d80 DebAlloc
 *   0x00189de0 RemoveChunkControlFromStack
 *   0x00189e28 DebFreeWithoutKey
 *   0x00189e70 DebrisEmitterOrientationMtx
 *   0x00189f30 AddDebrisEffectToStack
 *   0x00189f60 DebrisDoSounds
 */

#include "creature.h"

struct debinfo_s;
struct debpart_s;
struct debchunk_s;

struct deb_s {
    struct debchunk_s *chunks[256];
    struct numtx_s rotmtx;
    struct numtx_s emitrotmtx;
    s16 chunkcount;
    s16 debtype;
    s16 on;
    s16 limit;
    u8 pad_488[2];
    s16 index;
    s16 reqchunkcount;
    s16 reqcount;
    struct nuvec_s emitpos;
    struct nuvec_s emitoffset;
    struct debpart_s *(*gensort)(struct deb_s *deb, struct debinfo_s *info, s32 param);
    void (*gencode)(struct deb_s *deb, struct debinfo_s *info, struct debpart_s *part);
    struct deb_s *next;
    u8 pad_4B4[4];
    s16 rotory;
    s16 rotorz;
    s32 instances;
    s16 oncount;
    s16 delay;
    u8 pad_4C4[0x84];
    s32 trigger;
    s32 trigger_param;
    f32 trigger_value;
    s16 reflect_a;
    s16 reflect_b;
    f32 reflect_x;
    f32 reflect_y;
    s16 soundtimer[4];
    s16 groupid;
    u8 pad_56A[2];
};

struct debpart_s {
    struct nuvec_s pos;
    f32 time;
    struct nuvec_s mom;
    f32 rate;
};

struct debsound_s {
    s32 id;
    s32 type;
    s16 delay;
    u8 pad_0A[2];
};

struct debkey_s {
    f32 time;
    f32 value;
};

struct debcolkey_s {
    f32 time;
    f32 r;
    f32 g;
    f32 b;
};

struct debdma_s;

struct debinfo_s {
    char name[0x10];
    u8 pad_10[8];
    s16 ival_on;
    s16 ival_on_ran;
    s16 ival_off;
    s16 ival_off_ran;
    s8 gensort;
    u8 pad_21[0x13];
    f32 mom_ybias;
    struct nuvec_s variable_emit;
    struct nuvec_s variable_scale;
    struct nuvec_s pos_offset;
    struct nuvec_s pos_offset_scale;
    struct nuvec_s pos_range;
    struct nuvec_s pos_base;
    struct nuvec_s mom_range;
    struct nuvec_s mom_base;
    f32 gtime;
    f32 etime;
    u8 pad_A0[8];
    f32 wobble_xfreq;
    f32 wobble_xamp;
    f32 wobble_yfreq;
    f32 wobble_yamp;
    struct debcolkey_s colkeys[8];
    struct debkey_s alphakeys[8];
    u8 pad_178[8];
    struct debkey_s widthkeys[8];
    struct debkey_s heightkeys[8];
    u8 pad_200[8];
    struct debkey_s rotkeys[8];
    u8 pad_248[0x80];
    f32 u0;
    f32 v0;
    f32 u1;
    f32 v1;
    struct debdma_s *dmatype;
    u8 pad_2DC[0x48];
    struct debsound_s sounds[4];
};

union debchannel_u {
    f32 coord;
    u8 colour;
    u32 bits;
};

struct debtri_s {
    union debchannel_u v0x;
    union debchannel_u v0y;
    union debchannel_u v1x;
    union debchannel_u v1y;
    f32 v2x;
    f32 v2y;
    u8 pad_18[8];
};

struct debdma_s {
    u8 pad_00[0x14];
    f32 gtime;
    u8 pad_18[0x18];
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
    s32 u2;
    s32 v2;
    s32 u3;
    s32 v3;
    u8 pad_50[0x10];
    struct debtri_s tri[64];
};

struct debchunk_s {
    u8 pad_00[0x20];
    struct debpart_s parts[32];
};

struct chunkctrl_s {
    struct debchunk_s *chunk;
    s32 life;
    s32 state;
    struct deb_s *deb;
    struct chunkctrl_s *next;
};

struct debrender_s {
    struct debchunk_s *chunk;
    struct debinfo_s *info;
    struct deb_s *deb;
    struct numtx_s mtx;
    struct nuvec_s pos;
};

extern struct deb_s debkeydata[];
extern struct debinfo_s *debtab[128];
extern struct numtx_s numtx_identity;
extern f32 NuTrigTable[];
extern s32 globalframes;
extern struct deb_s *debris_emitter_stack[];
extern s16 freedebkeys[];
extern s32 freedebkeyptr;
extern struct debrender_s ParticleChunkRenderStack[256];
extern struct debchunk_s *freedebchunks[];
extern s32 freedebchkptr;
extern struct chunkctrl_s *freechunkcontrols[];
extern s32 freechunkcontrolsptr;
extern struct chunkctrl_s *debris_chunk_control_stack[32];
extern s32 debris_chunk_control_stack_index;
extern s32 debris_render_group;
extern struct nuvec_s *CutoffCameraVec;
extern f32 D_0062CEB8;
#define deb_mom_scale D_0062CEB8
extern f32 D_0062CEBC;
#define deb_tree_pull D_0062CEBC
extern f32 D_0062CEC0;
#define deb_tree_rand_scale D_0062CEC0
extern s32 debris_sfx;
extern struct debdma_s *DmaDebTypes[];
extern s32 freeDmaDebType;
extern f32 globaltime;
extern f32 D_0062CEA4;
#define deb_randtime_scale D_0062CEA4

void SetupDebris(void);
struct debpart_s *GenDebIndex(struct deb_s *deb, struct debinfo_s *info);
f32 NuVecDist(struct nuvec_s *a, struct nuvec_s *b, void *c);
void DebFree(s32 *key);
void DebFreeInstantly(s32 *key);
struct deb_s **FindDebrisEffectStack(struct deb_s *node);
void RemoveDebrisEffectFromStack(struct deb_s *node, struct deb_s **list);
void DebReAlloc2(struct deb_s *deb);
void AddDebrisEffect(s32 *key, s32 type, f32 x, f32 y, f32 z);
void NuMtxRotateZ(struct numtx_s *m, s32 angle);
void NuMtxRotateY(struct numtx_s *m, s32 angle);
s32 strcmp(const char *a, const char *b);
f32 NuFsqrt(f32 x);
double sqrt(double x);
s32 rand(void);
void edbitsSoundPlay(struct nuvec_s *pos, s32 id);
void NuVecMtxTransform(struct nuvec_s *out, struct nuvec_s *in, struct numtx_s *mtx);
void NuVecRotateY(struct nuvec_s *out, struct nuvec_s *in, s32 angle);
void NuVecRotateZ(struct nuvec_s *out, struct nuvec_s *in, s32 angle);
void GenericDebinfoDmaTypeUpdate(struct debinfo_s *info);


void GenericDebinfoDmaTypeUpdate(struct debinfo_s *info) {
    struct debtri_s *tri;
    s32 i;
    s32 j;
    f32 t;
    f32 dt;
    f32 range;
    f32 width;
    f32 height;
    f32 angle;
    f32 wobx;
    f32 woby;
    f32 r;
    f32 g;
    f32 b;
    f32 alpha;

    if (info->dmatype == 0) {
        info->dmatype = DmaDebTypes[freeDmaDebType++];
    }
    info->dmatype->gtime = info->gtime;
    info->dmatype->u0 = info->dmatype->u2 = info->u0 * 16.0f;
    info->dmatype->u1 = info->dmatype->u3 = info->u1 * 16.0f;
    info->dmatype->v0 = info->dmatype->v1 = info->v0 * 16.0f;
    info->dmatype->v2 = info->dmatype->v3 = info->v1 * 16.0f;
    for (i = 0; i < 64; i++) {
        t = (f32)i * 0.015625f;

        j = 0;
        while (j < 7 && !(info->widthkeys[j].time <= t && t <= info->widthkeys[j + 1].time)) {
            j++;
        }
        dt = t - info->widthkeys[j].time;
        range = info->widthkeys[j + 1].time - info->widthkeys[j].time;
        width = info->widthkeys[j].value + dt / range * (info->widthkeys[j + 1].value - info->widthkeys[j].value);

        j = 0;
        while (j < 7 && !(info->heightkeys[j].time <= t && t <= info->heightkeys[j + 1].time)) {
            j++;
        }
        dt = t - info->heightkeys[j].time;
        range = info->heightkeys[j + 1].time - info->heightkeys[j].time;
        height = info->heightkeys[j].value + dt / range * (info->heightkeys[j + 1].value - info->heightkeys[j].value);

        j = 0;
        while (j < 7 && !(info->rotkeys[j].time <= t && t <= info->rotkeys[j + 1].time)) {
            j++;
        }
        dt = t - info->rotkeys[j].time;
        range = info->rotkeys[j + 1].time - info->rotkeys[j].time;
        angle = info->rotkeys[j].value + dt / range * (info->rotkeys[j + 1].value - info->rotkeys[j].value);

        tri = &info->dmatype->tri[i];
        wobx = info->wobble_xamp * NuTrigTable[(u16)(s32)(info->wobble_xfreq * t * 65536.0f)];
        woby = info->wobble_yamp * NuTrigTable[(u16)(s32)(info->wobble_yfreq * t * 65536.0f)];
        tri->v0x.coord = (-(width * 0.5f) * NuTrigTable[(u16)(s32)(angle + 16384.0f)]
                          - height * 0.5f * NuTrigTable[(u16)(s32)angle] + wobx) * 640.0f * 0.00390625f;
        tri->v0y.coord = width * 0.5f * NuTrigTable[(u16)(s32)angle]
                       - height * 0.5f * NuTrigTable[(u16)(s32)(angle + 16384.0f)] + woby;
        tri->v2x = (width * 0.5f * NuTrigTable[(u16)(s32)(angle + 16384.0f)]
                    - height * 0.5f * NuTrigTable[(u16)(s32)angle] + wobx) * 640.0f * 0.00390625f;
        tri->v2y = -(width * 0.5f) * NuTrigTable[(u16)(s32)angle]
                 - height * 0.5f * NuTrigTable[(u16)(s32)(angle + 16384.0f)] + woby;
        tri->v1x.coord = height * NuTrigTable[(u16)(s32)angle] * 640.0f * 0.00390625f;
        tri->v1y.coord = height * NuTrigTable[(u16)(s32)(angle + 16384.0f)];

        j = 0;
        while (j < 7 && !(info->colkeys[j].time <= t && t <= info->colkeys[j + 1].time)) {
            j++;
        }
        dt = t - info->colkeys[j].time;
        range = info->colkeys[j + 1].time - info->colkeys[j].time;
        r = info->colkeys[j].r + dt / range * (info->colkeys[j + 1].r - info->colkeys[j].r);
        g = info->colkeys[j].g + dt / range * (info->colkeys[j + 1].g - info->colkeys[j].g);
        b = info->colkeys[j].b + dt / range * (info->colkeys[j + 1].b - info->colkeys[j].b);

        j = 0;
        while (j < 7 && !(info->alphakeys[j].time <= t && t <= info->alphakeys[j + 1].time)) {
            j++;
        }
        dt = t - info->alphakeys[j].time;
        range = info->alphakeys[j + 1].time - info->alphakeys[j].time;
        alpha = info->alphakeys[j].value + dt / range * (info->alphakeys[j + 1].value - info->alphakeys[j].value);

        tri->v0x.bits = (tri->v0x.bits & 0xFFFFFF00) + ((s32)r & 0xFF);
        tri->v0y.bits = (tri->v0y.bits & 0xFFFFFF00) + ((s32)g & 0xFF);
        tri->v1x.bits = (tri->v1x.bits & 0xFFFFFF00) + ((s32)b & 0xFF);
        tri->v1y.bits = (tri->v1y.bits & 0xFFFFFF00) + ((s32)alpha & 0xFF);
    }
}


struct debpart_s *GenDebIndex(struct deb_s *deb, struct debinfo_s *info) {
    struct nuvec_s v;
    struct debpart_s *part;

    if (deb->index >= deb->limit) {
        deb->index = 0;
    }
    part = deb->chunks[deb->index / 32]->parts;
    part += (s16)(deb->index % 32);
    deb->index++;
    part->time = globaltime;
    part->rate = 64.0f / info->etime;
    v.x = (f32)rand() * info->pos_offset_scale.x - info->pos_offset.x;
    v.y = (f32)rand() * info->pos_offset_scale.y - info->pos_offset.y;
    v.z = (f32)rand() * info->pos_offset_scale.z - info->pos_offset.z;
    NuVecMtxTransform(&v, &v, &deb->emitrotmtx);
    part->pos.x = v.x;
    part->pos.y = v.y;
    part->pos.z = v.z;
    v.x = (f32)rand() * info->variable_scale.x - info->variable_emit.x;
    v.y = (f32)rand() * info->variable_scale.y - info->variable_emit.y + info->mom_ybias;
    v.z = (f32)rand() * info->variable_scale.z - info->variable_emit.z;
    NuVecMtxTransform(&v, &v, &deb->emitrotmtx);
    part->mom.x = v.x;
    part->mom.y = v.y;
    part->mom.z = v.z;
    if (deb->gencode != 0) {
        deb->gencode(deb, info, part);
    }
    part->pos.x += deb->emitoffset.x;
    part->pos.y += deb->emitoffset.y;
    part->pos.z += deb->emitoffset.z;
    if (info->dmatype == 0) {
        GenericDebinfoDmaTypeUpdate(info);
    }
    return part;
}


struct debpart_s *GenDebIndexRadial(struct deb_s *deb, struct debinfo_s *info) {
    struct nuvec_s v;
    struct debpart_s *part;
    s32 roty;
    s32 rotz;
    f32 zero;

    if (deb->index >= deb->limit) {
        deb->index = 0;
    }
    part = deb->chunks[deb->index / 32]->parts;
    part += (s16)(deb->index % 32);
    deb->index++;
    part->time = globaltime;
    part->rate = 64.0f / info->etime;
    zero = 0.0f;
    roty = (f32)rand() * info->variable_scale.y - info->variable_emit.y + info->pos_offset.y;
    rotz = (f32)rand() * info->variable_scale.z - info->variable_emit.z + info->pos_offset.z;
    v.y = info->pos_offset.x;
    v.z = zero;
    v.x = zero;
    NuVecRotateZ(&v, &v, rotz);
    NuVecRotateY(&v, &v, roty);
    NuVecMtxTransform(&v, &v, &deb->emitrotmtx);
    part->pos.x = v.x;
    part->pos.y = v.y;
    part->pos.z = v.z;
    v.y = (f32)rand() * info->variable_scale.x - info->variable_emit.x + info->mom_ybias;
    v.z = zero;
    v.x = zero;
    NuVecRotateZ(&v, &v, rotz);
    NuVecRotateY(&v, &v, roty);
    NuVecMtxTransform(&v, &v, &deb->emitrotmtx);
    part->mom.x = v.x;
    part->mom.y = v.y;
    part->mom.z = v.z;
    if (deb->gencode != 0) {
        deb->gencode(deb, info, part);
    }
    part->pos.x += deb->emitoffset.x;
    part->pos.y += deb->emitoffset.y;
    part->pos.z += deb->emitoffset.z;
    if (info->dmatype == 0) {
        GenericDebinfoDmaTypeUpdate(info);
    }
    return part;
}


struct debpart_s *GenDebIndexPos(struct deb_s *deb, struct debinfo_s *info) {
    struct debpart_s *part;

    if (deb->index >= deb->limit) {
        deb->index = 0;
    }
    part = deb->chunks[deb->index / 32]->parts;
    part += (s16)(deb->index % 32);
    deb->index++;
    part->time = globaltime;
    part->rate = 64.0f / info->etime;
    part->pos.x = (f32)rand() * info->pos_range.x + info->pos_base.x;
    part->pos.y = (f32)rand() * info->pos_range.y + info->pos_base.y;
    part->pos.z = (f32)rand() * info->pos_range.z + info->pos_base.z;
    part->mom.x = (f32)rand() * info->mom_range.x + info->mom_base.x;
    part->mom.y = (f32)rand() * info->mom_range.y + info->mom_base.y;
    part->mom.z = (f32)rand() * info->mom_range.z + info->mom_base.z;
    if (deb->gencode != 0) {
        deb->gencode(deb, info, part);
    }
    if (info->dmatype == 0) {
        GenericDebinfoDmaTypeUpdate(info);
    }
    return part;
}


struct debpart_s *GenDebIndexPosRandTime(struct deb_s *deb, struct debinfo_s *info) {
    struct debpart_s *part;

    if (deb->index >= deb->limit) {
        deb->index = 0;
    }
    part = deb->chunks[deb->index / 32]->parts;
    part += (s16)(deb->index % 32);
    deb->index++;
    part->time = globaltime;
    part->rate = 64.0f / (info->etime + info->etime * rand() / deb_randtime_scale);
    part->pos.x = (f32)rand() * info->pos_range.x + info->pos_base.x;
    part->pos.y = (f32)rand() * info->pos_range.y + info->pos_base.y;
    part->pos.z = (f32)rand() * info->pos_range.z + info->pos_base.z;
    part->mom.x = (f32)rand() * info->mom_range.x + info->mom_base.x;
    part->mom.y = (f32)rand() * info->mom_range.y + info->mom_base.y;
    part->mom.z = (f32)rand() * info->mom_range.z + info->mom_base.z;
    if (deb->gencode != 0) {
        deb->gencode(deb, info, part);
    }
    if (info->dmatype == 0) {
        GenericDebinfoDmaTypeUpdate(info);
    }
    return part;
}


void SplashEffect(struct debdma_s *dma) {
    struct debtri_s *tri;
    s32 i;

    dma->gtime = -0.6f;
    dma->u0 = 0xC00;
    dma->v0 = 0x400;
    dma->u1 = 0xDF0;
    dma->v1 = 0x400;
    dma->u2 = 0xC00;
    dma->v2 = 0x5F0;
    dma->u3 = 0xDF0;
    dma->v3 = 0x5F0;
    tri = dma->tri;
    for (i = 0; i < 64; i++) {
        tri->v0x.coord = (NuTrigTable[(u16)((i << 9) + 0x4000)] * -75.0f
                          - NuTrigTable[(u16)(i << 9)] * 75.0f) * 640.0f * 0.00390625f;
        tri->v0y.coord = NuTrigTable[(u16)(i << 9)] * -75.0f
                       + NuTrigTable[(u16)((i << 9) + 0x4000)] * 75.0f;
        tri->v2x = (NuTrigTable[(u16)((i << 9) + 0x4000)] * 75.0f
                    - NuTrigTable[(u16)(i << 9)] * 75.0f) * 640.0f * 0.00390625f;
        tri->v2y = NuTrigTable[(u16)(i << 9)] * 75.0f
                 + NuTrigTable[(u16)((i << 9) + 0x4000)] * 75.0f;
        tri->v1x.coord = NuTrigTable[(u16)(i << 9)] * 150.0f * 640.0f * 0.00390625f;
        tri->v1y.coord = NuTrigTable[(u16)((i << 9) + 0x4000)] * -150.0f;
        tri->v0x.colour = 0x1E;
        tri->v0y.colour = 0x20;
        tri->v1x.colour = 0x30;
        if (i < 41) {
            tri->v1y.colour = 0x30;
        } else {
            tri->v1y.colour = (64 - i) * 2;
        }
        tri++;
    }
}


void SplashBodyEffect(struct debdma_s *dma) {
    struct debtri_s *tri;
    s32 i;
    s32 size;

    dma->gtime = 0.0f;
    dma->u0 = 0;
    dma->v0 = 0;
    dma->u1 = 0x3F0;
    dma->v1 = 0;
    dma->u2 = 0;
    dma->v2 = 0x3F0;
    dma->u3 = 0x3F0;
    dma->v3 = 0x3F0;
    tri = dma->tri;
    for (i = 0; i < 64; i++, tri++) {
        size = 75 + i * 2;
        tri->v0x.coord = ((f32)-size * NuTrigTable[0x8000] - (f32)size * NuTrigTable[0x4000])
                       * 640.0f * 0.00390625f;
        tri->v0y.coord = (f32)-size * NuTrigTable[0x4000] + (f32)size * NuTrigTable[0x8000];
        tri->v2x = ((f32)size * NuTrigTable[0x8000] - (f32)size * NuTrigTable[0x4000])
                 * 640.0f * 0.00390625f;
        tri->v2y = (f32)size * NuTrigTable[0x4000] + (f32)size * NuTrigTable[0x8000];
        tri->v1x.coord = (f32)(size * 2) * NuTrigTable[0x4000] * 640.0f * 0.00390625f;
        tri->v1y.coord = (f32)(-size * 2) * NuTrigTable[0x8000];
        tri->v0x.colour = 0x1E;
        tri->v0y.colour = 0x20;
        tri->v1x.colour = 0x30;
        tri->v1y.colour = (64 - i) * 80 / 64;
    }
}


void DebFree(s32 *key) {
    struct chunkctrl_s **list;
    struct chunkctrl_s *cur;
    struct chunkctrl_s *node;
    struct deb_s **stack;
    s32 i;

    if (*key != -1) {
        if (freechunkcontrolsptr + debkeydata[*key].chunkcount > 512) {
            DebFreeInstantly(key);
            return;
        }
        for (i = 0; i < debkeydata[*key].chunkcount; i++) {
            freechunkcontrols[freechunkcontrolsptr]->chunk = debkeydata[*key].chunks[i];
            freechunkcontrols[freechunkcontrolsptr]->life = debtab[debkeydata[*key].debtype]->etime * 60.0f;
            freechunkcontrols[freechunkcontrolsptr]->state = 0;
            freechunkcontrols[freechunkcontrolsptr]->deb = &debkeydata[*key];
            list = &debris_chunk_control_stack[debris_chunk_control_stack_index];
            node = freechunkcontrols[freechunkcontrolsptr];
            if (*list != 0) {
                do {
                    cur = *list;
                    list = &cur->next;
                } while (cur->next != 0);
            }
            *list = node;
            node->next = 0;
            freechunkcontrolsptr++;
        }
        for (i = 0; i < 256; i++) {
            if (ParticleChunkRenderStack[i].chunk == debkeydata[*key].chunks[0]) {
                ParticleChunkRenderStack[i].deb = 0;
                ParticleChunkRenderStack[i].mtx = debkeydata[*key].rotmtx;
                ParticleChunkRenderStack[i].pos.x = debkeydata[*key].emitpos.x;
                ParticleChunkRenderStack[i].pos.y = debkeydata[*key].emitpos.y;
                ParticleChunkRenderStack[i].pos.z = debkeydata[*key].emitpos.z;
                break;
            }
        }
        stack = FindDebrisEffectStack(&debkeydata[*key]);
        RemoveDebrisEffectFromStack(&debkeydata[*key], stack);
        debkeydata[*key].debtype = 0;
        freedebkeyptr--;
        freedebkeys[freedebkeyptr] = *key;
        *key = -1;
    }
}


void DebFreeInstantly(s32 *key) {
    struct deb_s **stack;
    s32 i;
    s32 j;

    if (*key != -1) {
        freedebchkptr -= debkeydata[*key].chunkcount;
        for (i = 0; i < debkeydata[*key].chunkcount; i++) {
            freedebchunks[i + freedebchkptr] = debkeydata[*key].chunks[i];
            for (j = 0; j < 256; j++) {
                if (ParticleChunkRenderStack[j].chunk == debkeydata[*key].chunks[i]) {
                    ParticleChunkRenderStack[j].chunk = 0;
                    ParticleChunkRenderStack[j].info = 0;
                }
            }
        }
        stack = FindDebrisEffectStack(&debkeydata[*key]);
        RemoveDebrisEffectFromStack(&debkeydata[*key], stack);
        debkeydata[*key].debtype = 0;
        freedebkeyptr--;
        freedebkeys[freedebkeyptr] = *key;
        *key = -1;
    }
}


void DebrisStartOffset(s32 key, s32 offset) {
    struct debinfo_s *type;
    s32 looptime;
    s32 loopfrac;

    if (key != -1) {
        type = debtab[debkeydata[key].debtype];
        if (type->ival_on_ran != 0 || type->ival_off_ran != 0) {
            debkeydata[key].delay = 0;
        } else {
            if (type->gensort == 7) {
                debkeydata[key].delay = 0;
                debkeydata[key].rotory = offset * type->variable_emit.y;
                debkeydata[key].rotorz = offset * type->variable_emit.z;
            }
            looptime = type->ival_on + type->ival_off;
            loopfrac = globalframes % looptime - offset;
            if (loopfrac == 0) {
                debkeydata[key].delay = 0;
            } else {
                debkeydata[key].delay = looptime - loopfrac;
            }
            debkeydata[key].oncount = type->ival_on;
        }
    }
}


void AddDebEff(s32 key, s32 param, s32 count) {
    s32 i;

    if (debkeydata[key].on != 0) {
        for (i = 0; i < count; i++) {
            debkeydata[key].gensort(&debkeydata[key], debtab[debkeydata[key].debtype], param);
        }
    }
}


void DebrisOff(s32 *key) {
    struct deb_s *deb;

    if (*key != -1) {
        deb = &debkeydata[*key];
        deb->on = 0;
    }
}


void DebrisOn(s32 *key) {
    struct deb_s *deb;

    if (*key != -1) {
        deb = &debkeydata[*key];
        deb->on = 1;
    }
}


void AddFiniteShotDebrisEffect(s32 *key, s32 type, struct nuvec_s *pos, s32 instances) {
    AddDebrisEffect(key, type, pos->x, pos->y, pos->z);
    if (*key != -1) {
        debkeydata[*key].instances = instances;
        debkeydata[*key].delay = 0;
    }
}


void DebReAlloc(struct deb_s *deb, s32 count) {
    deb->reqcount = count;
    deb->reqchunkcount = (count + 31) / 32;
    if (deb->reqchunkcount == deb->chunkcount) {
        deb->limit = count;
    } else {
        DebReAlloc2(deb);
    }
}


void DebrisOrientation(s32 key, s16 rotz, s16 roty) {
    if (key != -1) {
        debkeydata[key].rotmtx = numtx_identity;
        NuMtxRotateZ(&debkeydata[key].rotmtx, rotz);
        NuMtxRotateY(&debkeydata[key].rotmtx, roty);
    }
}


void DebrisOrientationMtx(s32 key, struct numtx_s *mtx) {
    if (key != -1) {
        debkeydata[key].rotmtx = *mtx;
        debkeydata[key].rotmtx._30 = 0.0f;
        debkeydata[key].rotmtx._31 = 0.0f;
        debkeydata[key].rotmtx._32 = 0.0f;
    }
}


void DebrisPosOrientationMtx(s32 key, struct numtx_s *mtx) {
    if (key != -1) {
        debkeydata[key].rotmtx = *mtx;
        debkeydata[key].emitpos.x = mtx->_30;
        debkeydata[key].emitpos.y = mtx->_31;
        debkeydata[key].emitpos.z = mtx->_32;
        debkeydata[key].rotmtx._30 = 0.0f;
        debkeydata[key].rotmtx._31 = 0.0f;
        debkeydata[key].rotmtx._32 = 0.0f;
    }
}


void DebrisEmitterOrientation(s32 key, s16 rotz, s16 roty) {
    if (key != -1) {
        debkeydata[key].emitrotmtx = numtx_identity;
        NuMtxRotateZ(&debkeydata[key].emitrotmtx, rotz);
        NuMtxRotateY(&debkeydata[key].emitrotmtx, roty);
    }
}


void DebrisEmiterPos(s32 key, f32 x, f32 y, f32 z) {
    if (key != -1) {
        debkeydata[key].emitpos.x = x;
        debkeydata[key].emitpos.y = y;
        debkeydata[key].emitpos.z = z;
    }
}


void DebrisReflectionOrientation(s32 key, s16 a, s16 b, f32 x, f32 y) {
    if (key != -1) {
        debkeydata[key].reflect_a = a;
        debkeydata[key].reflect_b = b;
        debkeydata[key].reflect_x = x;
        debkeydata[key].reflect_y = y;
    }
}


void DebrisSetTrigger(s32 key, s32 trigger, s32 param, f32 value) {
    if (key != -1) {
        debkeydata[key].trigger = trigger;
        debkeydata[key].trigger_param = param;
        debkeydata[key].trigger_value = value;
    }
}


void DebrisSetGroupID(s32 key, s16 groupid) {
    struct deb_s *deb;

    if (key != -1) {
        deb = &debkeydata[key];
        deb->groupid = groupid;
    }
}


void DebrisSetup(void) {
    SetupDebris();
}


void DebrisSetRenderGroup(s32 group) {
    debris_render_group = group;
}


s32 LookupDebrisEffect(char *name) {
    s32 i;

    for (i = 1; i < 128; i++) {
        if (debtab[i] != 0 && strcmp(debtab[i]->name, name) == 0) {
            return i;
        }
    }
    return -1;
}


void DebrisRegisterCutoffCameraVec(struct nuvec_s *vec) {
    CutoffCameraVec = vec;
}


f32 CameraEmitterDistance(struct nuvec_s *pos) {
    if (CutoffCameraVec == 0) {
        return 0.0f;
    }
    return NuVecDist(pos, CutoffCameraVec, 0);
}


s32 DebrisFindAllOfType(s32 type, struct nuvec_s **list, s32 max, s32 skip, s32 activeonly) {
    s32 i;
    s32 found;

    found = 0;
    for (i = 0; i < 256; i++) {
        if (debkeydata[i].debtype == type
            && (!activeonly || debkeydata[i].chunkcount != 0)) {
            if (found - skip >= 0 && found - skip < max) {
                list[found - skip] = &debkeydata[i].emitpos;
            }
            found++;
            if (found - skip >= max) {
                break;
            }
        }
    }
    return found - skip;
}


struct deb_s **FindDebrisEffectStack(struct deb_s *node) {
    struct deb_s **list;
    struct deb_s *cur;
    s32 i;

    for (i = 0; i < 32; i++) {
        list = &debris_emitter_stack[i];
        if (*list != 0) {
            do {
                cur = *list;
                if (cur == node) {
                    return &debris_emitter_stack[i];
                }
                list = &cur->next;
            } while (*list != 0);
        }
    }
    return 0;
}


void RemoveDebrisEffectFromStack(struct deb_s *node, struct deb_s **list) {
    struct deb_s *cur;

    if (*list != 0) {
        if (*list == node) {
            *list = node->next;
        } else {
            do {
                cur = *list;
                list = &cur->next;
                if (cur->next == 0) {
                    break;
                }
                if (cur->next == node) {
                    cur->next = node->next;
                    break;
                }
            } while (1);
        }
    }
    node->next = 0;
}


void AddChunkControlToStack(struct chunkctrl_s *node, struct chunkctrl_s **list) {
    struct chunkctrl_s *cur;

    if (*list != 0) {
        do {
            cur = *list;
            list = &cur->next;
        } while (cur->next != 0);
    }
    *list = node;
    node->next = 0;
}


s32 SolveQuadratic(f32 a, f32 b, f32 c, f32 *x1, f32 *x2) {
    f32 bb;
    f32 ac4;
    f32 root;

    if (a == 0.0f) {
        if (b == 0.0f) {
            return 0;
        }
        *x1 = *x2 = -c / b;
        return 1;
    }
    bb = b * b;
    ac4 = a * 4.0f * c;
    if (bb < ac4) {
        return 0;
    }
    if (ac4 == bb) {
        *x1 = *x2 = -b / (a + a);
        return 1;
    }
    root = NuFsqrt(bb - ac4);
    *x1 = (-b - root) / (a + a);
    *x2 = (-b + root) / (a + a);
    return 1;
}


struct debpart_s *GenDebIndexSort(struct deb_s *deb, struct debinfo_s *info) {
    return GenDebIndex(deb, info);
}


void GenDebMomAdjFromPos(struct deb_s *deb, struct debinfo_s *info, struct debpart_s *part) {
    part->mom.x = part->pos.x * 2.0;
    part->mom.z = part->pos.z * 2.0;
}


void GenDebMomAdjFromPosAll(struct deb_s *deb, struct debinfo_s *info, struct debpart_s *part) {
    part->mom.x = part->pos.x * deb_mom_scale;
    part->mom.y = part->pos.y * deb_mom_scale;
    part->mom.z = part->pos.z * deb_mom_scale;
}


void GenDebMomAdjFromPosRev(struct deb_s *deb, struct debinfo_s *info, struct debpart_s *part) {
    part->mom.x = -part->pos.x;
    part->mom.z = -part->pos.z;
}


void GenDebMomAdjFromSplash(struct deb_s *deb, struct debinfo_s *info, struct debpart_s *part) {
    part->mom.x = part->pos.x * 4.0f;
    part->mom.z = part->pos.z * 4.0f;
}


void GenDebMomAdjFromAshRock(struct deb_s *deb, struct debinfo_s *info, struct debpart_s *part) {
    part->mom.x = part->pos.x * 16.0f;
    part->mom.z = part->pos.z * 16.0f;
}


void GenDebMomAdjFromPosRevTree(struct deb_s *deb, struct debinfo_s *info, struct debpart_s *part) {
    part->mom.x += part->pos.x * deb_tree_pull;
    part->mom.z += part->pos.z * deb_tree_pull;
    part->pos.y = part->pos.y - sqrt(part->pos.x * part->pos.x + part->pos.z * part->pos.z) * 0.4f;
    part->rate = 64.0f / (info->etime + info->etime * rand() / deb_tree_rand_scale);
}


s32 DebAlloc(void) {
    s32 key;
    struct deb_s *deb;

    if (freedebkeyptr >= 256) {
        return -1;
    }
    key = freedebkeys[freedebkeyptr++];
    deb = &debkeydata[key];
    deb->chunkcount = 0;
    deb->limit = 0;
    deb->reqchunkcount = 0;
    deb->reqcount = 0;
    deb->chunks[0] = 0;
    return key;
}


void RemoveChunkControlFromStack(struct chunkctrl_s *node, struct chunkctrl_s **list) {
    struct chunkctrl_s *cur;

    if (*list != 0) {
        if (*list == node) {
            *list = node->next;
        } else {
            do {
                cur = *list;
                list = &cur->next;
                if (cur->next == 0) {
                    break;
                }
                if (cur->next == node) {
                    cur->next = node->next;
                    break;
                }
            } while (1);
        }
    }
    node->next = 0;
}


void DebFreeWithoutKey(struct deb_s *deb) {
    s32 i;
    s32 key;

    for (i = 0; i < 256; i++) {
        if (&debkeydata[i] == deb) {
            key = i;
            DebFree(&key);
            break;
        }
    }
}


void DebrisEmitterOrientationMtx(s32 key, struct numtx_s *mtx) {
    if (key != -1) {
        debkeydata[key].emitrotmtx = *mtx;
        debkeydata[key].emitrotmtx._30 = 0.0f;
        debkeydata[key].emitrotmtx._31 = 0.0f;
        debkeydata[key].emitrotmtx._32 = 0.0f;
    }
}


void AddDebrisEffectToStack(struct deb_s *node, struct deb_s **list) {
    struct deb_s *cur;

    if (node->next != 0) {
        node->next = 0;
    }
    if (*list != 0) {
        do {
            cur = *list;
            list = &cur->next;
        } while (cur->next != 0);
    }
    *list = node;
}


void DebrisDoSounds(struct deb_s *deb, s32 type) {
    struct debinfo_s *info;
    s32 i;

    info = debtab[deb->debtype];
    for (i = 0; i < 4; i++) {
        if (info->sounds[i].id != -1 && info->sounds[i].type == type && deb->soundtimer[i] <= 0) {
            debris_sfx = 1;
            edbitsSoundPlay(&deb->emitpos, info->sounds[i].id);
            if (type == 1 || type == 2) {
                deb->soundtimer[i] = 9999;
            } else {
                deb->soundtimer[i] = info->sounds[i].delay;
            }
        }
    }
}
