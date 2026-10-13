/*
 * Unit: game/particle
 *
 * Functions:
 *   0x001cb678 DrawParticles
 *   0x001cb798 InitParticles
 *   0x001cb7b8 CloseParticles
 *   0x001cb7e0 AddParticle
 *   0x001cb8c8 DestroyParticlesByHandle
 *   0x001cb958 ProcessParticles
 */

#include "creature.h"

struct nulsthdr_s;
struct nulnkhdr_s;
struct numtl_s;

/* What a particle looks like (layout from DrawParticles/AddParticle). */
struct ptltype_s {
    s32 life;               /* 0x00 frames */
    f32 width;              /* 0x04 */
    f32 height;             /* 0x08 */
    f32 uv[4];              /* 0x0C */
    u8 pad1C[0x20 - 0x1C];
    struct numtl_s *mtl;    /* 0x20 */
};

struct particle_s {
    u8 pad00[0x8];          /* 0x00 */
    struct nuvec_s pos;     /* 0x08 */
    struct nuvec_s vel;     /* 0x14 */
    struct nuvec_s acc;     /* 0x20 */
    s16 ang;                /* 0x2C */
    s16 angvel;             /* 0x2E */
    s32 life;               /* 0x30 frames left */
    s16 handle;             /* 0x34 */
    u8 pad36[2];
    struct ptltype_s *type; /* 0x38 */
}; /* 0x3C */

/* Face-on sprite as NuRndrFaceOn takes it. */
struct ptlfaceon_s {
    struct nuvec_s pos;     /* 0x00 */
    f32 w;                  /* 0x0C */
    f32 uv[4];              /* 0x10 */
    f32 width;              /* 0x20 */
    f32 height;             /* 0x24 */
    f32 sin;                /* 0x28 */
    f32 cos;                /* 0x2C */
    s32 r, g, b, a;         /* 0x30 */
}; /* 0x40 */

extern struct nulsthdr_s *NuLstCreate(int count, int size);
extern void NuLstDestroy(struct nulsthdr_s *list);
extern struct nulnkhdr_s *NuLstAlloc(struct nulsthdr_s *list);
extern void NuLstFree(struct nulnkhdr_s *lnk);
extern struct nulnkhdr_s *NuLstGetNext(struct nulsthdr_s *list, struct nulnkhdr_s *lnk);
extern void *NuScratchAlloc32(s32 size);
extern void NuScratchRelease(void);
extern void NuRndrFaceOn(struct ptlfaceon_s *faceon, struct numtl_s *mtl, s32 n);
extern void NuVecAdd(struct nuvec_s *dst, struct nuvec_s *a, struct nuvec_s *b);
extern f32 NuTrigTable[];

/* Retail-owned (no Data-from-C yet). */
extern struct nulsthdr_s *D_0062FCF0;
#define ParticleList D_0062FCF0

#define FIRST_PARTICLE() ((struct particle_s *)NuLstGetNext(ParticleList, 0))
#define NEXT_PARTICLE(p) ((struct particle_s *)NuLstGetNext(ParticleList, (struct nulnkhdr_s *)(p)))

void DrawParticles(void) {
    struct ptlfaceon_s *faceon;
    struct particle_s *p;
    struct particle_s *next;
    struct ptltype_s *type;

    faceon = NuScratchAlloc32(sizeof(struct ptlfaceon_s));
    for (p = FIRST_PARTICLE(); p != 0; p = next) {
        next = NEXT_PARTICLE(p);
        type = p->type;
        faceon->pos = p->pos;
        faceon->w = 0.0f;
        faceon->uv[0] = type->uv[0];
        faceon->uv[1] = type->uv[1];
        faceon->uv[2] = type->uv[2];
        faceon->uv[3] = type->uv[3];
        faceon->width = type->width;
        faceon->height = type->height;
        faceon->sin = NuTrigTable[(u16)p->ang];
        faceon->cos = NuTrigTable[(p->ang + 0x4000) & 0xFFFF];
        faceon->r = faceon->g = faceon->b = faceon->a = 0xFF;
        NuRndrFaceOn(faceon, type->mtl, 0);
    }
    NuScratchRelease();
}

void InitParticles(s32 count) {
    ParticleList = NuLstCreate(count, 0x3C);
}

void CloseParticles(void) {
    if (ParticleList != 0) {
        NuLstDestroy(ParticleList);
    }
    ParticleList = 0;
}

struct particle_s *AddParticle(struct nuvec_s *pos, struct nuvec_s *vel, struct nuvec_s *acc,
                               s16 ang, s16 angvel, s16 handle, struct ptltype_s *type) {
    struct particle_s *p;

    p = (struct particle_s *)NuLstAlloc(ParticleList);
    if (p != 0) {
        p->pos = *pos;
        p->vel = *vel;
        p->acc = *acc;
        p->ang = ang;
        p->angvel = angvel;
        p->handle = handle;
        p->type = type;
        p->life = type->life;
    }
    return p;
}

/* Free every particle with this handle; returns how many there were. */
s32 DestroyParticlesByHandle(s16 handle) {
    struct particle_s *p;
    struct particle_s *next;
    s32 count;

    count = 0;
    for (p = FIRST_PARTICLE(); p != 0; p = next) {
        next = NEXT_PARTICLE(p);
        if (p->handle == handle) {
            NuLstFree((struct nulnkhdr_s *)p);
            count++;
        }
    }
    return count;
}

void ProcessParticles(void) {
    struct particle_s *p;
    struct particle_s *next;

    for (p = FIRST_PARTICLE(); p != 0; p = next) {
        next = NEXT_PARTICLE(p);
        NuVecAdd(&p->vel, &p->acc, &p->vel);
        NuVecAdd(&p->pos, &p->vel, &p->pos);
        p->ang += p->angvel;
        if (--p->life == 0) {
            NuLstFree((struct nulnkhdr_s *)p);
        }
    }
}
