/*
 * Unit: nu3d/nuscene
 *
 * Functions:
 *   0x00117f18 ReadNuIFFTextureSet
 *   0x00118180 ReadNuIFFGSplineSet
 *   0x00118360 ReadNuIFFGeom
 *   0x001185d8 ReadNuIFFGeomSkin
 *   0x00118780 ReadNuIFFBlendShape
 *   0x00118900 NuNodeRead
 *   0x00118ca8 ReadNuIFFScene
 *   0x00119198 ReadNuIFFSpecialObjects
 *   0x00119340 ReadNuIFFGScene
 *   0x001199a0 NuSceneLoad
 *   0x00119b10 NuGSceneDestroy
 *   0x00119d30 NuSceneDestroy
 *   0x00119ed8 NuSceneFindNodeByName
 *   0x00119f90 ReadNuIFFMaterialSet
 *   0x0011a070 ReadNuIFFUnknown
 *   0x0011a0c8 NuSceneMtlUpdate
 *   0x0011a198 ReadNuIFFNameTable
 *   0x0011a210 ReadNuIFFGobjSet
 *   0x0011a2f0 ReadNuIFFInstSet
 *   0x0011a478 ReadNuIFFGeomDef
 *   0x0011a4c0 ReadNuIFFGeomVtx
 *   0x0011a5f8 ReadNuIFFGeomCntrl
 *   0x0011a668 ReadNuIFFGeomPrim
 *   0x0011a788 ReadNuIFFGeomTri
 *   0x0011a800 ReadNuIFFGeomBezTri
 *   0x0011a878 ReadNuIFFFaceOnGeom
 */

struct numtl_s {
    char pad000[0x168];
    struct numtl_s *specmtl;        /* 0x168 */
    char pad16C[0x38];
    int tid;                        /* 0x1A4 */
};

struct nunode_s;
struct nugobj_s;

struct nuprim_s {
    char pad00[0x10];
    unsigned short *vid;            /* 0x10 */
};

struct nucntrl_s {
    char pad00[0x0C];
}; /* 0x0C */

struct nugeom_s {
    char pad00[0x38];
    struct nucntrl_s *cntrls;       /* 0x38 */
    int ncntrls;                    /* 0x3C */
};

struct nufaceon_s {
    char pad00[0x18];
}; /* 0x18 */

struct nufaceongeom_s {
    char pad00[0x0C];
    int faceontype;                 /* 0x0C */
    struct nufaceon_s *faceons;     /* 0x10 */
    int nfaceons;                   /* 0x14 */
    float faceonradius;             /* 0x18 */
};

struct nuscene_s {
    int numnodes;                   /* 0x00 */
    char **names;                   /* 0x04 */
    struct nunode_s **nodes;        /* 0x08 */
    char *nametable;                /* 0x0C */
    int numtids;                    /* 0x10 */
    short *tids;                    /* 0x14 */
    int nummtls;                    /* 0x18 */
    struct numtl_s **mtls;          /* 0x1C */
    int numgobjs;                   /* 0x20 */
    struct nugobj_s **gobjs;        /* 0x24 */
};

struct nutexanim_s {
    char pad00[0x20];
}; /* 0x20 */

struct nugscn_s {
    int *tids;                      /* 0x00 */
    int numtids;                    /* 0x04 */
    struct numtl_s **mtls;          /* 0x08 */
    int nummtls;                    /* 0x0C */
    int numgobjs;                   /* 0x10 */
    struct nugobj_s **gobjs;        /* 0x14 */
    int numinstances;               /* 0x18 */
    void *instances;                /* 0x1C */
    int numspecials;                /* 0x20 */
    void *specials;                 /* 0x24 */
    void *splinedata;               /* 0x28 */
    int numsplines;                 /* 0x2C */
    void *splines;                  /* 0x30 */
    void *nametable;                /* 0x34 */
    int numexspecials;              /* 0x38 */
    void *exspecials;               /* 0x3C */
    int exallocix;                  /* 0x40 */
    void *instanimdata;             /* 0x44 */
    void *animlib;                  /* 0x48 */
    char pad4C[0x14];
    int numtexanims;                /* 0x60 */
    struct nutexanim_s *texanims;   /* 0x64 */
    void *texanimbits;              /* 0x68 */
};

typedef void (*nuerror_fn)(char *fmt, ...);
typedef int (*numtlreadevent_fn)(struct numtl_s *mtl);

extern nuerror_fn NuErrorProlog(char *file, int line);
extern nuerror_fn NuDebugMsgProlog(char *file, int line);
extern void *NuMemAllocFn(int size, char *file, int line);
extern void NuMemFreeFn(void *ptr, char *file, int line);
extern void *memset(void *s, int c, unsigned int n);
extern int strcmp(const char *s1, const char *s2);
extern int NuFileRead(int fh, void *buf, int size);
extern int NuFileReadInt(int fh);
extern float NuFileReadFloat(int fh);
extern void NuTexDestroy(int tid);
extern void NuMtlDestroy(struct numtl_s *mtl);
extern struct numtl_s *NuMtlRead(int fh);
extern void NuMtlUpdate(struct numtl_s *mtl);
extern void NuGobjDestroy(struct nugobj_s *gobj);
extern void NuTexAnimDestroy(struct nutexanim_s *texanim);
extern struct nuprim_s *NuPrimCreate(int nvids, int type);
extern void NuGeomAddPrim(struct nugeom_s *geom, struct nuprim_s *prim);
extern struct nugobj_s *ReadNuIFFGeom(int fh, struct numtl_s **mtls);
void NuSceneMtlUpdate(struct nuscene_s *scene);
void ReadNuIFFGeomDef(int fh, struct nuscene_s *scene);

extern int show_strip;
extern struct numtl_s *specmtl;
extern numtlreadevent_fn NuMtlReadEvent;

extern char D_006142C8[];
extern char D_00614328[];
extern char D_00614358[];
extern char D_00614378[];
extern char D_00614408[];
#define nuscene_srcfile D_006142C8
#define reading_mtlset_msg D_00614328
#define reading_gobjset_msg D_00614358
#define gobj_defined_msg D_00614378
#define unknown_block_msg D_00614408


void NuGSceneDestroy(struct nugscn_s *gscene) {
    int i;

    for (i = 0; i < gscene->numtids; i++) {
        NuTexDestroy(gscene->tids[i]);
    }
    NuMemFreeFn(gscene->tids, nuscene_srcfile, 0x67E);
    for (i = 0; i < gscene->nummtls; i++) {
        NuMtlDestroy(gscene->mtls[i]);
    }
    NuMemFreeFn(gscene->mtls, nuscene_srcfile, 0x683);
    for (i = 0; i < gscene->numgobjs; i++) {
        NuGobjDestroy(gscene->gobjs[i]);
    }
    NuMemFreeFn(gscene->gobjs, nuscene_srcfile, 0x688);
    if (gscene->instances) {
        NuMemFreeFn(gscene->instances, nuscene_srcfile, 0x68C);
    }
    if (gscene->instances) {
        NuMemFreeFn(gscene->instances, nuscene_srcfile, 0x690);
    }
    if (gscene->instanimdata) {
        NuMemFreeFn(gscene->instanimdata, nuscene_srcfile, 0x694);
    }
    if (gscene->animlib) {
        NuMemFreeFn(gscene->animlib, nuscene_srcfile, 0x697);
    }
    if (gscene->numsplines) {
        NuMemFreeFn(gscene->splinedata, nuscene_srcfile, 0x69C);
        NuMemFreeFn(gscene->splines, nuscene_srcfile, 0x69D);
    }
    if (gscene->nametable) {
        NuMemFreeFn(gscene->nametable, nuscene_srcfile, 0x6A2);
    }
    if (gscene->specials) {
        NuMemFreeFn(gscene->exspecials, nuscene_srcfile, 0x6A6);
    }
    for (i = 0; i < gscene->numtexanims; i++) {
        NuTexAnimDestroy(&gscene->texanims[i]);
    }
    NuMemFreeFn(gscene->texanims, nuscene_srcfile, 0x6AB);
    NuMemFreeFn(gscene->texanimbits, nuscene_srcfile, 0x6AC);
    NuMemFreeFn(gscene, nuscene_srcfile, 0x6AF);
}


struct nunode_s *NuSceneFindNodeByName(struct nuscene_s *scene, char *name) {
    int lo;
    int hi;
    int mid;
    int cmp;

    lo = 0;
    hi = scene->numnodes - 1;
    do {
        mid = (lo + hi) / 2;
        cmp = strcmp(name, scene->names[mid]);
        if (cmp == 0) {
            break;
        }
        if (cmp < 0) {
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    } while (lo <= hi);
    if (cmp != 0) {
        return 0;
    }
    return scene->nodes[mid];
}


void ReadNuIFFMaterialSet(int fh, struct nuscene_s *scene) {
    int count;
    int i;

    NuDebugMsgProlog(nuscene_srcfile, 0x1E6)(reading_mtlset_msg);
    count = NuFileReadInt(fh);
    if (count) {
        scene->mtls = NuMemAllocFn(count * sizeof(struct numtl_s *), nuscene_srcfile, 0x1EF);
        scene->nummtls = count;
        for (i = 0; i < count; i++) {
            scene->mtls[i] = NuMtlRead(fh);
            if (show_strip) {
                scene->mtls[i]->tid = -1;
            }
        }
        NuSceneMtlUpdate(scene);
    }
}


void ReadNuIFFUnknown(int fh, int blk) {
    NuDebugMsgProlog(nuscene_srcfile, 0x57A)(unknown_block_msg, blk & 0xFF, (blk >> 8) & 0xFF,
                                             (blk >> 16) & 0xFF, (unsigned int)blk >> 24);
}


void NuSceneMtlUpdate(struct nuscene_s *scene) {
    struct numtl_s *mtl;
    int i;

    for (i = 0; i < scene->nummtls; i++) {
        mtl = scene->mtls[i];
        if (mtl->tid == -1) {
            mtl->tid = 0;
        } else {
            mtl->tid = scene->tids[mtl->tid];
        }
        scene->mtls[i]->specmtl = specmtl;
        if (!NuMtlReadEvent(scene->mtls[i])) {
            NuMtlUpdate(scene->mtls[i]);
        }
    }
}


char *ReadNuIFFNameTable(int fh, int *size) {
    int tablesize;
    char *table;

    tablesize = NuFileReadInt(fh);
    table = NuMemAllocFn(tablesize, nuscene_srcfile, 0x6EF);
    NuFileRead(fh, table, tablesize);
    if (size) {
        *size = tablesize;
    }
    return table;
}


void ReadNuIFFGobjSet(int fh, struct nuscene_s *scene) {
    int count;
    int i;

    NuDebugMsgProlog(nuscene_srcfile, 0x2A4)(reading_gobjset_msg);
    count = NuFileReadInt(fh);
    if (scene->gobjs) {
        NuErrorProlog(nuscene_srcfile, 0x2AB)(gobj_defined_msg);
    }
    scene->gobjs = NuMemAllocFn(count * sizeof(struct nugobj_s *), nuscene_srcfile, 0x2AC);
    memset(scene->gobjs, 0, count * sizeof(struct nugobj_s *));
    scene->numgobjs = 0;
    for (i = 0; i < count; i++) {
        ReadNuIFFGeomDef(fh, scene);
    }
}


void ReadNuIFFGeomDef(int fh, struct nuscene_s *scene) {
    struct nugobj_s *gobj;

    gobj = ReadNuIFFGeom(fh, scene->mtls);
    scene->gobjs[scene->numgobjs++] = gobj;
}


void ReadNuIFFGeomCntrl(int fh, struct nugeom_s *geom) {
    int count;
    int size;

    count = NuFileReadInt(fh);
    geom->ncntrls = count;
    if (count) {
        size = count * sizeof(struct nucntrl_s);
        geom->cntrls = NuMemAllocFn(size, nuscene_srcfile, 0x398);
        NuFileRead(fh, geom->cntrls, size);
    }
}


void ReadNuIFFGeomTri(int fh, struct nugeom_s *geom, int type) {
    int nvids;
    struct nuprim_s *prim;

    nvids = NuFileReadInt(fh);
    prim = NuPrimCreate(nvids, type);
    NuFileRead(fh, prim->vid, nvids * sizeof(unsigned short));
    NuGeomAddPrim(geom, prim);
}


void ReadNuIFFGeomBezTri(int fh, struct nugeom_s *geom) {
    int nvids;
    int ntris;
    struct nuprim_s *prim;

    nvids = NuFileReadInt(fh);
    prim = NuPrimCreate(nvids, 8);
    ntris = nvids / 6;
    NuFileRead(fh, prim->vid, nvids * sizeof(unsigned short));
    NuGeomAddPrim(geom, prim);
}


void ReadNuIFFFaceOnGeom(int fh, struct nufaceongeom_s *fgeom) {
    int size;

    fgeom->nfaceons = NuFileReadInt(fh);
    fgeom->faceontype = NuFileReadInt(fh);
    fgeom->faceonradius = NuFileReadFloat(fh);
    size = fgeom->nfaceons * sizeof(struct nufaceon_s);
    fgeom->faceons = NuMemAllocFn(size, nuscene_srcfile, 0x3AA);
    NuFileRead(fh, fgeom->faceons, size);
}
