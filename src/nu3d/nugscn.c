/*
 * Unit: nu3d/nugscn
 *
 * Functions:
 *   0x001453b8 CvtNuIFFTexAnimSet
 *   0x00145508 CvtNuIFFTextureSet
 *   0x00145890 CvtNuIFFSplineSet
 *   0x00145998 CalcDimsAddFaceOn
 *   0x00145aa0 CvtNuIFFGeomDef
 *   0x00145e30 ReadNuIFFGeomSkin
 *   0x00145fd0 ReadNuIFFGeomVtx
 *   0x001461b0 CvtNuIFFGeomPrim
 *   0x00146350 MakeFaceOnDma
 *   0x001464b8 CvtNuIFFGeomTri
 *   0x00146728 CvtNuIFFGeomTriStrip
 *   0x001469e8 CvtNuIFFInstSet
 *   0x00146b50 CvtNuIFFAnimationLibrary
 *   0x001470e0 CvtNuIFFScene
 *   0x001475b8 CvtNuIFFGScene
 *   0x00147b50 NuGScnConvertScene
 *   0x00147dd0 NuNodeRead
 *   0x00148128 CvtNodes
 *   0x00148528 ReadTextureSet
 *   0x001486a0 ReadMaterialSet
 *   0x00148928 NuGScnDuplicate
 *   0x00148c90 ReadObjSet
 *   0x00149690 ReadTexAnimSet
 *   0x00149918 ReadNuIFFTexAnimSet
 *   0x00149b40 ReadAnimationLibrary
 *   0x00149d50 ReadSplineSet
 *   0x0014a060 DbgBlkType
 *   0x0014a258 NuGScnLoadFix
 *   0x0014a958 NuGScnUpdate
 *   0x0014aca8 NuGScnRndr3
 *   0x0014af00 NuGScnRndr2
 *   0x0014b428 NuGScnRndr
 *   0x0014b670 NuSpecialSetAttr
 *   0x0014baf0 NuGScnSetTmpWkspc
 *   0x0014bb00 NuGScnSetCfg
 *   0x0014bb08 NuGScnRemove
 *   0x0014bbd0 NuGScnLoad
 *   0x0014bcb8 NuGScnRead
 *   0x0014bdb8 NuGScnTranslate
 *   0x0014be20 NuGScnAddExt
 *   0x0014be60 NuExtSpecialDuplicate
 *   0x0014bf98 NuExtSpecialDelete
 *   0x0014bfb0 NuGSceneSetCrossFade
 *   0x0014c000 NuGSceneProcessCrossFade
 *   0x0014c098 NuGScnMtlUpdate
 *   0x0014c0a0 NuSpecialFind
 *   0x0014c130 NuSpecialGetPos
 *   0x0014c160 NuSpecialGetMtx
 *   0x0014c1f8 NuSpecialDrawAt
 *   0x0014c230 NuSplineFind
 *   0x0014c2b8 NuSplineFindAllSub
 *   0x0014c370 NuSplineGetRandomPoint
 *   0x0014c428 NuSceneReAssignTexAnims
 *   0x0014c488 CvtNuIFFGeom
 *   0x0014c5f0 CvtNuIFFFaceOnGeom
 *   0x0014c6d8 CvtNuIFFNodeHierarchy
 *   0x0014c768 FindSplinePtSize
 */

struct numtx_s {
    float _00, _01, _02, _03;
    float _10, _11, _12, _13;
    float _20, _21, _22, _23;
    float _30, _31, _32, _33;
}; /* 0x40 */

struct nuvec_s {
    float x;
    float y;
    float z;
};

struct nufaceon_s {
    struct nuvec_s point;           /* 0x00 */
    float width;                    /* 0x0C */
    float height;                   /* 0x10 */
    int colour;                     /* 0x14 */
}; /* 0x18 */

struct nufaceongeom_s {
    char pad00[0x10];
    struct nufaceon_s *faceons;     /* 0x10 */
    int nfaceons;                   /* 0x14 */
};

struct nuinstance_s {
    struct numtx_s matrix;          /* 0x00 */
    int objid;                      /* 0x40 */
    char pad44[0x0C];
}; /* 0x50 */

struct nuspecial_s {
    struct numtx_s mtx;             /* 0x00 */
    struct nuinstance_s *instance;  /* 0x40 */
    char *name;                     /* 0x44 */
    int flags;                      /* 0x48: bit 0 = ext (own matrix) */
    int pad;                        /* 0x4C */
}; /* 0x50 */

struct nugscn_s {
    char pad00[0x10];
    int numgobj;                    /* 0x10 */
    struct nugobj_s **gobjs;        /* 0x14 */
    int numinstance;                /* 0x18 */
    struct nuinstance_s *instances; /* 0x1C */
    int numspecial;                 /* 0x20 */
    struct nuspecial_s *specials;   /* 0x24 */
    char pad28[0x10];
    int numexspecials;              /* 0x38 */
    struct nuspecial_s *exspecials; /* 0x3C */
    int exallocix;                  /* 0x40 */
    char pad44[0x10];
    short crossfade;                /* 0x54 */
    short crossfaderate;            /* 0x56 */
    int crossfadegobj;              /* 0x58 */
    int crossfadefirst;             /* 0x5C */
    char pad60[0x0C];
}; /* 0x6C */

struct nuhspecial_s {
    struct nugscn_s *scene;         /* 0x00 */
    struct nuspecial_s *special;    /* 0x04 */
};

struct nutexanimenv_s {
    void *prog;                     /* 0x00 */
};

struct nutexanim_s {
    char pad00[0x14];
    struct nutexanimenv_s *env;     /* 0x14 */
    char *ntaname;                  /* 0x18 */
    char *scriptname;               /* 0x1C */
}; /* 0x20 */

struct nusplinecfg_s {
    char *name;                     /* 0x00 */
    int ptsize;                     /* 0x04 */
};

extern void *memset(void *s, int c, unsigned int n);
extern char *strstr(const char *haystack, const char *needle);
extern void NuMtxTranslate(struct numtx_s *m, struct nuvec_s *v);
extern int NuRndrGScnObj(struct nugobj_s *gobj, struct numtx_s *m);
extern void *NuTexAnimProgFind(char *name);
extern void NuTexAnimEnvReset(struct nutexanimenv_s *env);

extern struct nuvec_s nugscn_gobj_min;
extern struct nuvec_s nugscn_gobj_max;

extern void *D_006330B0;
extern int D_006330B8;
extern struct nusplinecfg_s *D_0062EE58;
#define temp_workspace D_006330B0
#define temp_workspace_size D_006330B8
#define spline_cfg D_0062EE58

#define ALIGN16(p) ((char *)(((unsigned int)(p) + 0xF) & 0xFFFFFFF0))


void CalcDimsAddFaceOn(struct nufaceongeom_s *geom)
{
    struct nufaceon_s *faceon;
    float size;
    int i;

    for (i = 0; i < geom->nfaceons; i++) {
        faceon = &geom->faceons[i];
        size = faceon->width;
        if (size < faceon->height) {
            size = faceon->height;
        }
        if (faceon->point.x - size < nugscn_gobj_min.x) {
            nugscn_gobj_min.x = faceon->point.x - size;
        }
        if (faceon->point.y - size < nugscn_gobj_min.y) {
            nugscn_gobj_min.y = faceon->point.y - size;
        }
        if (faceon->point.z - size < nugscn_gobj_min.z) {
            nugscn_gobj_min.z = faceon->point.z - size;
        }
        if (faceon->point.x + size > nugscn_gobj_max.x) {
            nugscn_gobj_max.x = faceon->point.x + size;
        }
        if (faceon->point.y + size > nugscn_gobj_max.y) {
            nugscn_gobj_max.y = faceon->point.y + size;
        }
        if (faceon->point.z + size > nugscn_gobj_max.z) {
            nugscn_gobj_max.z = faceon->point.z + size;
        }
    }
}


struct nugscn_s *NuGScnDuplicate(struct nugscn_s *scene, char **buffer)
{
    struct nugscn_s *dup;
    int i;

    dup = (struct nugscn_s *)ALIGN16(*buffer);
    *buffer = (char *)&dup[1];
    *dup = *scene;

    *buffer = ALIGN16(*buffer);
    dup->instances = (struct nuinstance_s *)*buffer;
    *buffer += dup->numinstance * sizeof(struct nuinstance_s);
    for (i = 0; i < dup->numinstance; i++) {
        dup->instances[i] = scene->instances[i];
    }

    *buffer = ALIGN16(*buffer);
    dup->specials = (struct nuspecial_s *)*buffer;
    *buffer += dup->numspecial * sizeof(struct nuspecial_s);
    for (i = 0; i < dup->numspecial; i++) {
        dup->specials[i] = scene->specials[i];
        dup->specials[i].instance = &dup->instances[scene->specials[i].instance - scene->instances];
    }
    return dup;
}


void NuGScnSetTmpWkspc(void *workspace, int size)
{
    temp_workspace = workspace;
    temp_workspace_size = size;
}


void NuGScnSetCfg(struct nusplinecfg_s *cfg)
{
    spline_cfg = cfg;
}


void NuGScnTranslate(struct nugscn_s *scene, struct nuvec_s *offset)
{
    struct nuinstance_s *instance;
    int i;

    i = scene->numinstance;
    instance = &scene->instances[i];
    while (i != 0) {
        i--;
        instance--;
        NuMtxTranslate(&instance->matrix, offset);
    }
}


void NuGScnAddExt(struct nugscn_s *scene, int count, struct nuspecial_s *specials)
{
    scene->numexspecials = count;
    scene->exspecials = specials;
    scene->exallocix = 0;
    memset(specials, 0, count * sizeof(struct nuspecial_s));
}


int NuExtSpecialDuplicate(struct nuhspecial_s *dst, struct nuhspecial_s *src)
{
    struct nugscn_s *scene;
    struct nuspecial_s *special;
    int start;

    scene = src->scene;
    start = scene->exallocix;
    do {
        scene->exallocix++;
        if (scene->exallocix == scene->numexspecials) {
            scene->exallocix = 0;
        }
        special = &scene->exspecials[scene->exallocix];
        if ((special->flags & 1) == 0) {
            dst->scene = scene;
            dst->special = special;
            *special = *src->special;
            special->flags |= 3;
            return 1;
        }
    } while (scene->exallocix != start);
    return 0;
}


void NuExtSpecialDelete(struct nuhspecial_s *sph)
{
    sph->special->flags &= ~1;
}


void NuGSceneSetCrossFade(struct nugscn_s *scene, short crossfade, short rate)
{
    if (crossfade != scene->crossfade) {
        scene->crossfade = crossfade;
        scene->crossfaderate = rate;
        scene->crossfadefirst = scene->crossfadegobj;
        scene->crossfadegobj++;
        if (scene->crossfadegobj >= scene->numgobj) {
            scene->crossfadegobj = 0;
        }
    }
}


void NuGScnMtlUpdate(void)
{
}


void NuSpecialGetPos(struct nuhspecial_s *sph, struct nuvec_s *pos)
{
    struct numtx_s *mtx;

    mtx = &sph->special->mtx;
    if ((sph->special->flags & 1) == 0) {
        mtx = &sph->special->instance->matrix;
    }
    pos->x = mtx->_30;
    pos->y = mtx->_31;
    pos->z = mtx->_32;
}


void NuSpecialGetMtx(struct nuhspecial_s *sph, struct numtx_s *mtx)
{
    struct numtx_s *src;

    src = &sph->special->mtx;
    if ((sph->special->flags & 1) == 0) {
        src = &sph->special->instance->matrix;
    }
    *mtx = *src;
}


int NuSpecialDrawAt(struct nuhspecial_s *sph, struct numtx_s *mtx)
{
    struct nugscn_s *scene;

    scene = sph->scene;
    return NuRndrGScnObj(scene->gobjs[sph->special->instance->objid], mtx);
}


void NuSceneReAssignTexAnims(struct nutexanim_s *texanims, int count)
{
    int i;

    if (texanims != 0) {
        for (i = 0; i < count; i++) {
            texanims->env->prog = NuTexAnimProgFind(texanims->scriptname);
            NuTexAnimEnvReset(texanims->env);
            texanims++;
        }
    }
}


int FindSplinePtSize(char *name)
{
    struct nusplinecfg_s *cfg;

    cfg = spline_cfg;
    if (cfg != 0) {
        while (cfg->name != 0) {
            if (strstr(name, cfg->name) != 0) {
                return cfg->ptsize;
            }
            cfg++;
        }
    }
    return 12;
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
s32 (*NuDebugMsgProlog(s32 *, s32))(s32 *);
s32 NuFileRead(s32, void *, s32);
s32 NuFileReadInt(s32);
void *NuMemAllocFn(s32, s32 *, s32);
extern s32 D_00614D00[];
extern s32 D_00614E08[];
extern s32 cvt_numsplines;
extern void *cvt_spline_cvs;
extern void *cvt_splines;

void CvtNuIFFSplineSet(s32 arg0) {
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_v0;
    s32 temp_v0_3;
    s32 temp_v0_5;
    s32 var_a0;
    void *temp_v0_2;
    void *temp_v0_4;
    void *var_lo;
    void *var_v1;

    NuDebugMsgProlog(&D_00614D00[0], 0x31E)(&D_00614E08[0]);
    temp_v0 = NuFileReadInt(arg0);
    temp_s0 = temp_v0 * 0x3C;
    temp_v0_2 = NuMemAllocFn(temp_s0, &D_00614D00[0], 0x324);
    cvt_numsplines = temp_v0;
    cvt_splines = temp_v0_2;
    if (temp_v0 != 0) {
        NuFileRead(arg0, temp_v0_2, temp_s0);
        temp_v0_3 = NuFileReadInt(arg0);
        if (temp_v0_3 != 0) {
            temp_s0_2 = temp_v0_3 * 0xC;
            temp_v0_4 = NuMemAllocFn(temp_s0_2, &D_00614D00[0], 0x32C);
            cvt_spline_cvs = temp_v0_4;
            NuFileRead(arg0, temp_v0_4, temp_s0_2);
            var_lo = cvt_spline_cvs;
            if (temp_v0 > 0) {
                var_v1 = cvt_splines;
                var_a0 = temp_v0;
                do {
                    temp_v0_5 = (*( s32 *)((s8 *)(var_v1) + ( 0)));
                    (*( void **)((s8 *)(var_v1) + ( 4))) = var_lo;
                    var_a0 -= 1;
                    var_v1 += 0x3C;
                    var_lo += temp_v0_5 * 0xC;
                } while (var_a0 != 0);
            }
        }
    }
}



s32 (*NuDebugMsgProlog(s32 *, s32))(s32 *);
s32 NuFileBeginBlkWrite(s32, s32, s32);
s32 NuFileEndBlkWrite(s32);
s32 NuFileWrite(s32, s32, s32);
s32 NuFileWriteInt(s32, s32);
s32 NuMemFreeFn(s32, s32 *, s32);
extern s32 D_00614D70[];
extern s32 D_006330C4;

void CvtNuIFFTexAnimSet(s32 arg0) {
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_s1;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    NuDebugMsgProlog(&D_00614D00[0], 0x234)(&D_00614D70[0]);
    NuFileBeginBlkWrite(D_006330C4, 0x30534154, 0);
    temp_s0 = NuFileReadInt(arg0);
    NuFileReadInt(arg0);
    temp_s1 = temp_s0 << 5;
    NuFileWriteInt(D_006330C4, temp_s0);
    NuFileWriteInt(D_006330C4, 0);
    temp_v0 = NuMemAllocFn(temp_s1, &D_00614D00[0], 0x23F);
    NuFileRead(arg0, temp_v0, temp_s1);
    NuFileWrite(D_006330C4, temp_v0, temp_s1);
    temp_v0_2 = NuFileReadInt(arg0);
    NuFileWriteInt(D_006330C4, temp_v0_2);
    temp_s0_2 = temp_v0_2 * 2;
    temp_v0_3 = NuMemAllocFn(temp_s0_2, &D_00614D00[0], 0x246);
    NuFileRead(arg0, temp_v0_3, temp_s0_2);
    NuFileWrite(D_006330C4, temp_v0_3, temp_s0_2);
    NuFileEndBlkWrite(D_006330C4);
    NuMemFreeFn(temp_v0, &D_00614D00[0], 0x24D);
    NuMemFreeFn(temp_v0_3, &D_00614D00[0], 0x24E);
}
