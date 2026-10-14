#ifndef GAMECODE_GCUTSCN_H
#define GAMECODE_GCUTSCN_H

/*
 * Cutscene system (unit gamelib/gcutscn): the cutscene definition loaded from
 * a .cut file, its character / locator tables, and a running cutscene
 * instance. Shared by gamelib/gcutscn.c and the game-side callbacks in
 * game/cut.c.
 *
 * Needs struct numtx_s and struct nuvec_s: include creature.h
 * first.
 */

typedef struct NuGCutScene NuGCutScene;
typedef struct NuGCutSceneDef NuGCutSceneDef;
typedef struct NuGCutCharDesc NuGCutCharDesc;
typedef struct NuGCutCharDef NuGCutCharDef;
typedef struct NuGCutChar NuGCutChar;
typedef struct NuGCutCharSys NuGCutCharSys;
typedef struct NuGCutLocator NuGCutLocator;
typedef struct NuGCutLocatorData NuGCutLocatorData;
typedef struct NuGCutLocatorDef NuGCutLocatorDef;
typedef struct NuGCutLocatorSys NuGCutLocatorSys;
typedef struct NuGCutLocatorType NuGCutLocatorType;
typedef struct NuGCutCamDef NuGCutCamDef;
typedef struct NuGCutCamSys NuGCutCamSys;
typedef struct NuGCutRigidDef NuGCutRigidDef;
typedef struct NuGCutRigidSys NuGCutRigidSys;
typedef struct NuGCutRigid NuGCutRigid;
typedef struct NuGCutTriggerDef NuGCutTriggerDef;
typedef struct NuGCutTriggerSys NuGCutTriggerSys;
typedef struct NuGCutTriggerList NuGCutTriggerList;
typedef struct NuGCutBounds NuGCutBounds;
typedef struct NuAnimData2 NuAnimData2;

struct nuanimtime_s;

typedef void (*NuGCutSceneEndFn)(NuGCutScene *scene);

/* A running cutscene (instNuGCutSceneCreate allocates 0x94 bytes). */
struct NuGCutScene {
    NuGCutScene *next;              /* 0x00 */
    NuGCutScene *prev;              /* 0x04 */
    char name[16];                  /* 0x08 */
    struct numtx_s mtx;             /* 0x18 valid if NUGCUTSCENE_MATRIXSET */
    NuGCutSceneDef *def;            /* 0x58 */
    struct nuvec_s centre;          /* 0x5C */
    char pad68[4];                  /* 0x68 */
    int flags;                      /* 0x6C NUGCUTSCENE_* */
    float time;                     /* 0x70 current frame */
    float speed;                    /* 0x74 frames per update */
    NuGCutCamSys *camsys;           /* 0x78 */
    NuGCutRigidSys *rigidsys;       /* 0x7C */
    NuGCutCharSys *charsys;         /* 0x80 */
    NuGCutLocatorSys *locatorsys;   /* 0x84 */
    NuGCutTriggerSys *triggersys;   /* 0x88 */
    NuGCutScene *chain;             /* 0x8C started when this one ends */
    NuGCutSceneEndFn endcallback;   /* 0x90 */
};                                  /* 0x94 */

#define NUGCUTSCENE_STARTED   0x001
#define NUGCUTSCENE_RUNNING   0x002
#define NUGCUTSCENE_LOOP      0x004 /* instNuGCutSceneUpdate: restart at the end */
#define NUGCUTSCENE_MATRIXSET 0x010
#define NUGCUTSCENE_VISIBLE   0x080
#define NUGCUTSCENE_DISABLED  0x100
#define NUGCUTSCENE_UPDATED   0x200

/* A cutscene as loaded from its .cut file. */
struct NuGCutSceneDef {
    int version;                    /* 0x00 must be 1 */
    int base;                       /* 0x04 load address; relocation delta once loaded */
    float duration;                 /* 0x08 last frame */
    char *names;                    /* 0x0C string table */
    NuGCutCamDef *camdef;           /* 0x10 */
    NuGCutRigidDef *rigiddef;       /* 0x14 */
    NuGCutCharDef *chardef;         /* 0x18 */
    NuGCutLocatorDef *locatordef;   /* 0x1C */
    NuGCutBounds *bounds;           /* 0x20 */
    NuGCutTriggerDef *triggerdef;   /* 0x24 */
};

/* One animated character of a cutscene definition. */
struct NuGCutCharDesc {
    struct numtx_s mtx;             /* 0x00 start matrix */
    char *name;                     /* 0x40 */
    void *char_anim;                /* 0x44 */
    void *joint_anim;               /* 0x48 */
    void *face_anim;                /* 0x4C */
    void *character;                /* 0x50 model, bound by the find-characters callback */
    NuGCutLocatorData *locators;    /* 0x54 index on disk until fixed up */
    char pad58[4];                  /* 0x58 */
    unsigned char flags;            /* 0x5C 0x2 = per-instance data (CharacterCreateData) */
    char pad5D;                     /* 0x5D */
    unsigned char nlocators;        /* 0x5E */
    char pad5F;                     /* 0x5F */
    unsigned char first_locator;    /* 0x60 0xFF = none */
    char pad61[0x64 - 0x61];        /* 0x61 */
};                                  /* 0x64 */

struct NuGCutCharDef {
    NuGCutCharDesc *chars;          /* 0x00 */
    unsigned short count;           /* 0x04 */
};

/* Per-instance state of one character (instNuCGutCharSysCreate, stride 0x18). */
struct NuGCutChar {
    void *data;                     /* 0x00 NuGCutCharDesc.character, or the
                                     * NuCutSceneCharacterCreateData result */
    char pad4[0x15 - 0x4];          /* 0x04 */
    unsigned char prev_animix;      /* 0x15 */
    unsigned char current_animix;   /* 0x16 */
    char pad17;                     /* 0x17 */
};                                  /* 0x18 */

struct NuGCutCharSys {
    NuGCutChar *chars;              /* 0x00 */
};

/* Per-instance state of one locator (NuCGutLocatorSysCreateInst, stride 8). */
struct NuGCutLocator {
    float timer;                    /* 0x00 debris emission accumulator */
    int flags;                      /* 0x04 owned by the locator function */
};                                  /* 0x08 */

/* One locator of a cutscene definition. */
struct NuGCutLocatorData {
    char pad[0x4C];                 /* 0x00 */
    float value;                    /* 0x4C */
    NuAnimData2 *anim;              /* 0x50 */
    char pad54[4];                  /* 0x54 */
    unsigned char flags;            /* 0x58 */
    unsigned char type;             /* 0x59 index into NuGCutLocatorDef.types */
    unsigned char joint_ix;         /* 0x5A 0xFF = not on a joint */
    char pad5B[0x64 - 0x5B];        /* 0x5B */
};                                  /* 0x64 */

#define NUGCUTLOCATOR_VISIBLE 0x8

struct NuGCutLocatorDef {
    NuGCutLocatorData *locators;    /* 0x00 */
    NuGCutLocatorType *types;       /* 0x04 */
    unsigned char count;            /* 0x08 */
    unsigned char ntypes;           /* 0x09 */
};

struct NuGCutLocatorSys {
    NuGCutLocator *locators;        /* 0x00 */
};

/* Entry of the application's locator function table (NuGCutSceneSysInit). */
typedef void (*NuGCutLocatorUpdateFn)(NuGCutScene *scene, NuGCutLocatorDef *def,
                                      NuGCutLocator *locator, NuGCutLocatorData *data,
                                      struct numtx_s *mtx, float time);

typedef struct NuGCutLocatorFn {
    const char *name;
    NuGCutLocatorUpdateFn func;
} NuGCutLocatorFn;

typedef void (*NuCutSceneCharacterRenderFn)(NuGCutScene *scene, NuGCutSceneDef *def,
                                            NuGCutChar *character, NuGCutCharDesc *desc,
                                            float time);
typedef void (*NuCutSceneCharacterReleaseFn)(NuGCutChar *character);
typedef void (*NuCutSceneFindCharactersFn)(NuGCutSceneDef *def);
typedef void (*NuCutSceneCharacterCreateDataFn)(NuGCutCharDesc *desc, NuGCutChar *character,
                                                char **heap);
typedef void (*NuCutSceneRigidCollisionCheckFn)(NuGCutRigid *rigid, struct numtx_s *mtx);

NuGCutSceneDef *NuGCutSceneLoad(char *name, char **heap, char **heapend);
void NuGCutCharAnimProcess(NuGCutCharDesc *desc, float time, struct numtx_s *mtx,
                           int *is_visible, unsigned int *state, float *value1,
                           float *value2);
void NuGCutSceneSysInit(NuGCutLocatorFn *fns);
void NuGCutSceneSysUpdate(int paused);
void NuGCutSceneSysRender(void);
void NuSetCutSceneCharacterRenderFn(NuCutSceneCharacterRenderFn fn);
void NuSetCutSceneCharacterReleaseFn(NuCutSceneCharacterReleaseFn fn);
void NuSetCutSceneFindCharactersFn(NuCutSceneFindCharactersFn fn);
void NuSetCutSceneCharacterCreateDataFn(NuCutSceneCharacterCreateDataFn fn);
void NuSetCutSceneRigidCollisionCheckFn(NuCutSceneRigidCollisionCheckFn fn);
int instNuGCutSceneCharGetStartMtx(NuGCutScene *scene, char *name, struct numtx_s *mtx);
void NuGCutSceneFixUp(NuGCutSceneDef *def, void *gscene, void *triggers);
NuGCutScene *instNuGCutSceneCreate(NuGCutSceneDef *def, void *gscene,
                                   NuGCutTriggerList *triggers, char *name, char **heap);
NuGCutScene *instNuGCutSceneFind(const char *name);
void instNuGCutSceneDestroy(NuGCutScene *scene);
int instNuGCutSceneIsFinished(NuGCutScene *scene);
void instNuGCutSceneStart(NuGCutScene *scene);
void instNuGCutSceneEnd(NuGCutScene *scene);
void instNuGCutSceneReset(NuGCutScene *scene);
void instNuGCutSceneChain(NuGCutScene *scene, NuGCutScene *chain);
void instNuGCutSceneSetEndCallback(NuGCutScene *scene, NuGCutSceneEndFn callback);
void instNuGCutSceneEnable(NuGCutScene *scene);
void instNuGCutSceneDisable(NuGCutScene *scene);
int NuGCutLocatorIsVisble(NuGCutLocatorData *data, float time, struct nuanimtime_s *atime,
                          float *value);
void instNuGCutLocatorUpdate(NuGCutScene *scene, NuGCutLocatorDef *def, NuGCutLocator *locator,
                             NuGCutLocatorData *data, float time, struct numtx_s *mtx);

#endif /* GAMECODE_GCUTSCN_H */
