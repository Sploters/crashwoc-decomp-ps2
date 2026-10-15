#ifndef NU3D_NUGOBJ_H
#define NU3D_NUGOBJ_H

#include "nu_types.h"
#include "numath/numathtypes.h"

struct numtl_s;
struct nutribatch_s;
struct nucntrl_s;
struct nucolourblend_s;
struct SkinDmaTag;
struct SkinV;

struct nuvtx_tc1_s {
    struct nuvec_s pnt;                 /* 0x00 */
    struct nuvec_s nrm;                 /* 0x0C */
    union {
        int diffuse;                    /* 0x18 */
        struct nucolour_s rgba;         /* 0x18 */
    };
    float tc[2];                        /* 0x1C */
}; /* 0x24 */

struct nuprim_s {
    struct nuprim_s *next;              /* 0x00 */
    unsigned int type;                  /* 0x04 */
    int cnt;                            /* 0x08 */
    int max;                            /* 0x0C */
    short *vid;                         /* 0x10 */
    struct nuplane_s *pln;              /* 0x14 */
}; /* 0x18 */

struct nuskin_s {
    struct nuskin_s *next;              /* 0x00 */
    int vtxoffset;                      /* 0x04 */
    int vtxcnt;                         /* 0x08 */
    int mtxcnt;                         /* 0x0C */
    int *mtxid;                         /* 0x10 */
    float *weights;                     /* 0x14 */
    char pad18[0x10];                   /* 0x18 */
}; /* 0x28 */

struct nublendgeom_s {
    int nblends;                        /* 0x00 */
    struct nuvec_s **blend_offsets;     /* 0x04 */
    int *ix;                            /* 0x08 */
    struct nuvec_s *offsets;            /* 0x0C */
}; /* 0x10 */

struct nugeom_s {
    struct nugeom_s *next;              /* 0x00 */
    struct numtl_s *mtl;                /* 0x04 */
    void *dmastream;                    /* 0x08 */
    struct SkinDmaTag *skindma;         /* 0x0C */
    struct nutribatch_s *otbatch;       /* 0x10 */
    int mtl_id;                         /* 0x14 */
    int vtxtype;                        /* 0x18 */
    int vtxflags;                       /* 0x1C */
    int vtxcnt;                         /* 0x20 */
    int vtxmax;                         /* 0x24 */
    void *vtxptr;                       /* 0x28 */
    struct nuprim_s *prim;              /* 0x2C */
    struct nuskin_s *skins;             /* 0x30 */
    void *vtxskininfo;                  /* 0x34 */
    struct nucntrl_s *cntrls;           /* 0x38 */
    int ncntrls;                        /* 0x3C */
    int dmasize;                        /* 0x40 */
    struct SkinV *skinv;                /* 0x44 */
    struct nublendgeom_s *blendgeom;    /* 0x48 */
    int id;                             /* 0x4C */
}; /* 0x50 */

struct nufaceon_s {
    struct nuvec_s point;               /* 0x00 */
    float width;                        /* 0x0C */
    float height;                       /* 0x10 */
    int colour;                         /* 0x14 */
}; /* 0x18 */

struct nufaceongeom_s {
    struct nufaceongeom_s *next;        /* 0x00 */
    struct numtl_s *mtl;                /* 0x04 */
    int mtl_id;                         /* 0x08 */
    int faceon_type;                    /* 0x0C */
    struct nufaceon_s *faceons;         /* 0x10 */
    int nfaceons;                       /* 0x14 */
    float faceon_radius;                /* 0x18 */
    void *dmastream;                    /* 0x1C */
}; /* 0x20 */

struct nugobj_s {
    struct nugobj_s *sysnext;           /* 0x00 */
    struct nugobj_s *syslast;           /* 0x04 */
    int type;                           /* 0x08 */
    struct nugeom_s *geom;              /* 0x0C */
    struct nufaceongeom_s *faceon_geom; /* 0x10 */
    float r;                            /* 0x14 */
    float r2;                           /* 0x18 */
    struct nuvec_s min;                 /* 0x1C */
    struct nuvec_s max;                 /* 0x28 */
    struct nuvec_s cntr;                /* 0x34 */
    float cntr_r;                       /* 0x40 */
    float cntr_r2;                      /* 0x44 */
    int ngobjs;                         /* 0x48 */
    struct nugobj_s *next_gobj;         /* 0x4C */
    struct nuvec_s origin;              /* 0x50 */
    struct nucolourblend_s *blend;      /* 0x5C */
    int tricount;                       /* 0x60 */
    struct nucolourblend_s *faceon_blend; /* 0x64 */
    char pad68[0x04];                   /* 0x68 */
}; /* 0x6C */

void NuGobjDestroy(struct nugobj_s *gobj);
void NuGobjCalcFaceOnDims(struct nugobj_s *gobj);
void NuGobjCalcDims(struct nugobj_s *gobj);
void NuPrimCalcPlanes(struct nuprim_s *prim, struct nugeom_s *geom);
u32 *NuCreateStarfield(int count, float radius);
void NuGobjInit(void);
void NuGobjClose(void);
struct nugobj_s *NuGobjCreate(void);
void NuGobjAddGeom(struct nugobj_s *gobj, struct nugeom_s *geom);
void NuGobjAddFaceOnGeom(struct nugobj_s *gobj, struct nufaceongeom_s *geom);
void NuGobjCalcPlanes(struct nugobj_s *gobj);
int NuGobjApplyCrossFade(struct nugobj_s *gobj, short fade);
struct nugeom_s *NuGeomCreate(void);
void NuGeomDestroy(struct nugeom_s *geom);
struct nufaceongeom_s *NuFaceOnGeomCreate(void);
void NuFaceOnGeomDestroy(struct nufaceongeom_s *geom);
void NuGeomCalcPlanes(struct nugeom_s *geom);
void NuGeomAddPrim(struct nugeom_s *geom, struct nuprim_s *prim);
void NuGeomAddSkin(struct nugeom_s *geom, struct nuskin_s *skin);
struct nuprim_s *NuPrimCreate(int amount, unsigned int type);
void NuPrimDestroy(struct nuprim_s *prim);
int NuVtxStride(int type);
int NuPrimStride(unsigned int type);

#endif /* NU3D_NUGOBJ_H */
