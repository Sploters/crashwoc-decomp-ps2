/*
 * Unit: gamelib/gcutscn
 *
 * Functions:
 *   0x001aae88 NuGCutSceneLoad
 *   0x001aaff8 instNuGCutSceneUpdate
 *   0x001ab538 instNuGCutCamSysUpdate
 *   0x001abaa8 DebugRndrEmitter
 *   0x001abe00 NuGCutLocatorCalcMtx
 *   0x001ac018 instNuGCutDebrisLocatorUpdate
 *   0x001ac380 instNuCGutRigidSysCreate
 *   0x001ac530 NuGCutRigidCalcMtx
 *   0x001ac7a0 instNuGCutRigidSysUpdate
 *   0x001acaa0 instNuGCutRigidSysRender
 *   0x001acd30 NuGCutCharAnimProcess
 *   0x001ad0b8 NuGCutSceneSysInit
 *   0x001ad0c8 NuGCutSceneSysUpdate
 *   0x001ad130 NuGCutSceneSysRender
 *   0x001ad188 NuSetCutSceneCharacterRenderFn
 *   0x001ad190 NuSetCutSceneCharacterReleaseFn
 *   0x001ad198 NuSetCutSceneFindCharactersFn
 *   0x001ad1a0 NuSetCutSceneCharacterCreateDataFn
 *   0x001ad1a8 NuSetCutSceneRigidCollisionCheckFn
 *   0x001ad1b0 instNuGCutSceneCharGetStartMtx
 *   0x001ad2d0 NuGCutSceneFixUp
 *   0x001ad358 instNuGCutSceneCreate
 *   0x001ad4c0 instNuGCutSceneFind
 *   0x001ad518 instNuGCutSceneDestroy
 *   0x001ad580 instNuGCutSceneIsFinished
 *   0x001ad598 instNuGCutSceneStart
 *   0x001ad630 instNuGCutSceneEnd
 *   0x001ad6c8 instNuGCutSceneReset
 *   0x001ad720 instNuGCutSceneCreateCamTgtArray
 *   0x001ad788 instNuGCutSceneAddCamTgt
 *   0x001ad7e0 instNuGCutSceneSetPos
 *   0x001ad830 instNuGCutSceneTranslate
 *   0x001ad8a0 instNuGCutSceneRotateY
 *   0x001ad8f0 instNuGCutSceneSetMtx
 *   0x001ad9a0 instNuGCutSceneChain
 *   0x001ad9a8 instNuGCutSceneSetEndCallback
 *   0x001ad9b0 instNuGCutSceneEnable
 *   0x001ad9c8 instNuGCutSceneDisable
 *   0x001ad9d8 NuGCutLocatorIsVisble
 *   0x001ada90 CalculateLocatorDirection
 *   0x001adb98 instNuGCutLocatorUpdate
 *   0x001adc20 NuGCutCamsSysFixPtrs
 *   0x001adc90 instNuGCutSceneCalculateCentre
 *   0x001add20 instNuCGutCamSysCreate
 *   0x001adde0 instNuGCutCamSysStart
 *   0x001ade40 NuGCutLocatorSysFixPtrs
 *   0x001adf50 NuGCutLocatorSysFixUp
 *   0x001ae098 NuCGutLocatorSysCreateInst
 *   0x001ae138 instNuGCutLocatorSysEnd
 *   0x001ae170 instNuGCutLocatorSysStart
 *   0x001ae1a8 NuGCutRigidSysFixPtrs
 *   0x001ae290 NuGCutRigidSysFixUp
 *   0x001ae3b0 instNuGCutRigidSysEnd
 *   0x001ae510 instNuGCutRigidSysReset
 *   0x001ae630 instNuGCutRigidSysStart
 *   0x001ae668 NuGCutCharSysFixPtrs
 *   0x001ae770 NuGCutCharSysFixUp
 *   0x001ae798 instNuCGutCharSysCreate
 *   0x001ae8f0 instNuGCutCharSysEnd
 *   0x001ae9e8 NuGCutTriggerSysFixPtrs
 *   0x001aeaa0 NuGCutTriggerSysFixUp
 *   0x001aebb0 instNuCGutTriggerSysCreate
 *   0x001aec50 instNuGCutTriggerSysUpdate
 *   0x001aed38 instNuGCutTriggerSysStart
 *   0x001aed80 instNuGCutSceneRender
 *   0x001aee28 StateAnimEvaluate
 *   0x001aeef8 StateAnimFixPtrs
 *   0x001aef38 instNuGCutLocatorSysUpdate
 *   0x001af030 instNuGCutCharSysRender
 */

typedef struct NuMtx {
    float m[4][4];
} NuMtx;

typedef struct NuVec {
    float x;
    float y;
    float z;
} NuVec;

typedef struct NuVec4 {
    float x;
    float y;
    float z;
    float w;
} NuVec4;

typedef struct NuInstance {
    NuMtx matrix;                 /* 0x00 */
    int objid;                    /* 0x40 */
    int flags;                    /* 0x44 */
} NuInstance;

typedef struct NuSpecial {
    NuMtx mtx;                    /* 0x00 */
    NuInstance *instance;         /* 0x40 */
    char *name;                   /* 0x44 */
} NuSpecial;

typedef struct NuHSpecial {
    void *scene;                  /* 0x00 */
    NuSpecial *special;           /* 0x04 */
} NuHSpecial;

#define NUINSTANCE_VISIBLE 0x1

typedef struct NuGCutScene NuGCutScene;
typedef struct NuGCutSceneDef NuGCutSceneDef;
typedef struct NuGCutCharDesc NuGCutCharDesc;
typedef struct NuGCutCharDef NuGCutCharDef;
typedef struct NuGCutChar NuGCutChar;
typedef struct NuGCutRigid NuGCutRigid;
typedef struct NuGCutCamSys NuGCutCamSys;
typedef struct NuGCutCamDef NuGCutCamDef;
typedef struct NuGCutRigidSys NuGCutRigidSys;
typedef struct NuGCutRigidDef NuGCutRigidDef;
typedef struct NuGCutCharSys NuGCutCharSys;
typedef struct NuGCutLocator NuGCutLocator;
typedef struct NuGCutLocatorData NuGCutLocatorData;
typedef struct NuGCutLocatorDef NuGCutLocatorDef;
typedef struct NuGCutLocatorSys NuGCutLocatorSys;
typedef struct NuGCutTriggerDef NuGCutTriggerDef;
typedef struct NuGCutTriggerSys NuGCutTriggerSys;

typedef struct NuAnimTime NuAnimTime;

typedef void (*NuGCutSceneEndFn)(NuGCutScene *scene);

struct NuGCutScene {
    NuGCutScene *next;              /* 0x00 */
    NuGCutScene *prev;              /* 0x04 */
    char name[16];                  /* 0x08 */
    NuMtx mtx;                      /* 0x18 */
    NuGCutSceneDef *def;            /* 0x58 */
    NuVec centre;                   /* 0x5C */
    char pad68[4];                  /* 0x68 */
    int flags;                      /* 0x6C */
    float time;                     /* 0x70 */
    float speed;                    /* 0x74 */
    NuGCutCamSys *camsys;           /* 0x78 */
    NuGCutRigidSys *rigidsys;       /* 0x7C */
    NuGCutCharSys *charsys;         /* 0x80 */
    NuGCutLocatorSys *locatorsys;   /* 0x84 */
    NuGCutTriggerSys *triggersys;   /* 0x88 */
    NuGCutScene *chain;             /* 0x8C */
    NuGCutSceneEndFn endcallback;   /* 0x90 */
};                                  /* 0x94 */

#define NUGCUTSCENE_STARTED   0x001
#define NUGCUTSCENE_RUNNING   0x002
#define NUGCUTSCENE_MATRIXSET 0x010
#define NUGCUTSCENE_VISIBLE   0x080
#define NUGCUTSCENE_DISABLED  0x100
#define NUGCUTSCENE_UPDATED   0x200

typedef void (*NuGCutLocatorUpdateFn)(NuGCutScene *scene, NuGCutLocatorDef *def,
                                      NuGCutLocator *locator, NuGCutLocatorData *data,
                                      NuMtx *mtx, float time);

typedef struct NuGCutLocatorFn {
    const char *name;
    NuGCutLocatorUpdateFn func;
} NuGCutLocatorFn;

typedef void (*NuCutSceneCharacterRenderFn)(NuGCutScene *scene, NuGCutSceneDef *def,
                                            NuGCutChar *character, NuGCutCharDesc *desc,
                                            float time);
typedef void (*NuCutSceneCharacterReleaseFn)(NuGCutChar *character);
typedef void (*NuCutSceneFindCharactersFn)(void);
typedef void (*NuCutSceneCharacterCreateDataFn)(NuGCutCharDesc *desc, NuGCutChar *character,
                                                void *heap);
typedef void (*NuCutSceneRigidCollisionCheckFn)(NuGCutRigid *rigid, NuMtx *mtx);

extern NuCutSceneCharacterRenderFn NuCutSceneCharacterRender;
extern NuCutSceneCharacterReleaseFn NuCutSceneCharacterRelease;
extern NuCutSceneFindCharactersFn NuCutSceneFindCharacters;
extern NuCutSceneCharacterCreateDataFn NuCutSceneCharacterCreateData;
extern NuCutSceneRigidCollisionCheckFn NuCutSceneRigidCollisionCheck;
extern NuGCutLocatorFn *locatorfns;

extern NuGCutScene *D_0063322C;
#define cutscenelist D_0063322C

extern int D_0062F7CC;
extern int D_0062F7D0;
extern int D_0062F7D4;
#define cutscene_rndr_rigids   D_0062F7CC
#define cutscene_rndr_chars    D_0062F7D0
#define cutscene_rndr_locators D_0062F7D4

struct NuGCutLocator {
    void *object;                 /* 0x00 */
    int flags;                    /* 0x04 */
};

typedef struct StateAnim {
    int count;                    /* 0x00 */
    float *times;                 /* 0x04 */
    unsigned char *values;        /* 0x08 */
} StateAnim;

typedef struct NuAnimCurve2 {
    int data;                     /* 0x00 */
} NuAnimCurve2;

typedef struct NuAnimData2 {
    char pad[0xC];                /* 0x00 */
    NuAnimCurve2 *curves;         /* 0x0C */
    signed char *curveflags;      /* 0x10 */
} NuAnimData2;

#define NUANIM_CURVE_VALUE   6
#define NUANIM_CURVE_VISIBLE 7

struct NuGCutLocatorData {
    char pad[0x4C];               /* 0x00 */
    float value;                  /* 0x4C */
    NuAnimData2 *anim;            /* 0x50 */
    char pad54[4];                /* 0x54 */
    unsigned char flags;          /* 0x58 */
    unsigned char type;           /* 0x59 */
    char pad5A[0x64 - 0x5A];      /* 0x5A */
};

typedef struct NuGCutLocatorType {
    char *name;                   /* 0x00 */
    unsigned char flags;          /* 0x04 */
    char pad5;                    /* 0x05 */
    unsigned short fn;            /* 0x06 */
    char pad8[4];                 /* 0x08 */
} NuGCutLocatorType;

#define NUGCUTLOCATOR_DEBRIS 0x1
#define NUGCUTLOCATOR_FUNC   0x2
#define NUGCUTLOCATOR_NOFN   0xFFFF

#define NUGCUTLOCATOR_VISIBLE 0x8

struct NuGCutRigid {
    NuHSpecial obj;               /* 0x00 */
    unsigned char state;          /* 0x08 */
    char pad[3];                  /* 0x09 */
};

typedef struct NuGCutRigidData {
    NuMtx mtx;                    /* 0x00 */
    char *name;                   /* 0x40 */
    NuHSpecial obj;               /* 0x44 */
    NuAnimData2 *anim;            /* 0x4C */
    StateAnim *stateanim;         /* 0x50 */
    struct NuGCutLocatorData *locators; /* 0x54 index on disk until fixed up */
    unsigned char flags;          /* 0x58 */
    unsigned char nlocators;      /* 0x59 */
    unsigned char first_locator;  /* 0x5A 0xFF = none */
    char pad5B;                   /* 0x5B */
} NuGCutRigidData;

#define NUGCUTRIGID_VISIBLE  0x1
#define NUGCUTRIGID_ANIMATED 0x2
#define NUGCUTRIGID_FOUND    0x4

typedef struct NuGCutTrigger {
    unsigned char state;          /* 0x00 */
    char pad[3];                  /* 0x01 */
} NuGCutTrigger;

struct NuGCutLocatorDef {
    NuGCutLocatorData *locators;  /* 0x00 */
    NuGCutLocatorType *types;     /* 0x04 */
    unsigned char count;          /* 0x08 */
    unsigned char ntypes;         /* 0x09 */
};

struct NuGCutRigidDef {
    NuGCutRigidData *rigids;      /* 0x00 */
    unsigned short count;         /* 0x04 */
};

typedef struct NuGCutTriggerData {
    short target;                 /* 0x00 */
    char pad2[2];                 /* 0x02 */
    char *name;                   /* 0x04 */
    StateAnim *anim;              /* 0x08 */
} NuGCutTriggerData;

struct NuGCutTriggerDef {
    int count;                    /* 0x00 */
    NuGCutTriggerData *triggers;  /* 0x04 */
};

struct NuGCutLocatorSys {
    NuGCutLocator *locators;      /* 0x00 */
};

struct NuGCutRigidSys {
    NuGCutRigid *rigids;          /* 0x00 */
};

struct NuGCutChar {
    void *data;                   /* 0x00 */
    char pad4[0x15 - 0x4];        /* 0x04 */
    unsigned char prev_animix;    /* 0x15 */
    unsigned char current_animix; /* 0x16 */
    char pad17;                   /* 0x17 */
};

struct NuGCutCharDesc {
    NuMtx mtx;                    /* 0x00 */
    char *name;                   /* 0x40 */
    void *char_anim;              /* 0x44 */
    void *joint_anim;             /* 0x48 */
    void *face_anim;              /* 0x4C */
    void *character;              /* 0x50 */
    char pad54[0x5C - 0x54];      /* 0x54 */
    unsigned char flags;          /* 0x5C */
    char pad5D[0x64 - 0x5D];      /* 0x5D */
};

struct NuGCutCharDef {
    NuGCutCharDesc *chars;        /* 0x00 */
    unsigned short count;         /* 0x04 */
};

struct NuGCutCharSys {
    NuGCutChar *chars;            /* 0x00 */
};

typedef struct NuGCutBounds {
    NuVec min;                    /* 0x00 */
    NuVec max;                    /* 0x0C */
} NuGCutBounds;

typedef struct NuGCutTriggerTarget {
    char pad[2];                  /* 0x00 */
    unsigned char flags;          /* 0x02 */
    char pad3;                    /* 0x03 */
} NuGCutTriggerTarget;

typedef struct NuGCutTriggerList {
    char pad[0xC];                /* 0x00 */
    NuGCutTriggerTarget *targets; /* 0x0C */
} NuGCutTriggerList;

struct NuGCutTriggerSys {
    NuGCutTriggerList *list;      /* 0x00 */
    NuGCutTrigger *triggers;      /* 0x04 */
};

struct NuGCutSceneDef {
    int version;                  /* 0x00 */
    int base;                     /* 0x04 load address; relocation delta once loaded */
    float duration;               /* 0x08 */
    char *names;                  /* 0x0C */
    NuGCutCamDef *camdef;         /* 0x10 */
    NuGCutRigidDef *rigiddef;     /* 0x14 */
    NuGCutCharDef *chardef;       /* 0x18 */
    NuGCutLocatorDef *locatordef; /* 0x1C */
    NuGCutBounds *bounds;         /* 0x20 */
    NuGCutTriggerDef *triggerdef; /* 0x24 */
};

typedef struct NuGCutCamTgt {
    void *object;                 /* 0x00 */
    float weight;                 /* 0x04 */
    float time;                   /* 0x08 */
    char flags;                   /* 0x0C */
    char pad[3];                  /* 0x0D */
} NuGCutCamTgt;

typedef struct NuGCutCam {
    unsigned char flags;          /* 0x00 */
    unsigned char state;          /* 0x01 */
    char pad[2];                  /* 0x02 */
} NuGCutCam;

struct NuGCutCamDef {
    int count;                    /* 0x00 */
    void *cams;                   /* 0x04 */
    void *anim;                   /* 0x08 */
    StateAnim *stateanim;         /* 0x0C */
    unsigned char field10;        /* 0x10 */
};

struct NuGCutCamSys {
    NuGCutCamTgt *targets;        /* 0x00 */
    NuGCutCam *cams;              /* 0x04 */
    unsigned char field8;         /* 0x08 */
    unsigned char field9;         /* 0x09 */
    unsigned char fieldA;         /* 0x0A */
    unsigned char max;            /* 0x0B */
    unsigned char count;          /* 0x0C */
};

#define NuFixPtr(p, base) ((p) ? (void *)((char *)(p) + (base)) : 0)

extern void NuMtxSetIdentity(NuMtx *mtx);
extern void NuMtxSetTranslation(NuMtx *mtx, NuVec4 *pos);
extern void NuMtxTranslate(NuMtx *mtx, NuVec4 *offset);
extern void NuMtxRotateY(NuMtx *mtx, float angle);
extern void *NuAnimData2FixPtrs(void *anim, int base);
extern float NuAnimCurve2CalcVal(NuAnimCurve2 *curve, NuAnimTime *atime, int flags);
extern void NuVecMtxTransform(NuVec *dst, NuVec *src, NuMtx *mtx);
extern int strcasecmp(const char *s1, const char *s2);
extern void *memset(void *dst, int value, unsigned int size);

void instNuGCutSceneUpdate(NuGCutScene *scene, int paused);
void NuGCutRigidSysFixUp(NuGCutSceneDef *def, void *gscene);
void NuGCutCharSysFixUp(NuGCutSceneDef *def);
void NuGCutLocatorSysFixUp(NuGCutLocatorDef *def);
void NuGCutTriggerSysFixUp(NuGCutSceneDef *def, void *triggers);
void instNuGCutSceneEnd(NuGCutScene *scene);
void instNuGCutDebrisLocatorUpdate(NuGCutLocatorDef *def, NuGCutLocator *locator,
                                   NuGCutLocatorData *data, NuMtx *mtx, float time);
void instNuGCutSceneCalculateCentre(NuGCutScene *scene, NuMtx *mtx);
void instNuGCutCamSysStart(NuGCutCamSys *camsys, NuGCutCamDef *def);
void instNuGCutRigidSysEnd(NuGCutScene *scene, float time);
void instNuGCutRigidSysStart(NuGCutRigidSys *sys, NuGCutRigidDef *def);
void instNuGCutCharSysEnd(NuGCutScene *scene, float time);
void instNuGCutLocatorSysEnd(NuGCutLocatorSys *sys, NuGCutLocatorDef *def, float time);
void instNuGCutLocatorSysStart(NuGCutLocatorSys *sys, NuGCutLocatorDef *def);
void instNuGCutTriggerSysStart(NuGCutScene *scene);
void instNuGCutSceneRender(NuGCutScene *scene);
void instNuGCutRigidSysReset(NuGCutRigidSys *sys, NuGCutRigidDef *def);
StateAnim *StateAnimFixPtrs(StateAnim *anim, int base);
int StateAnimEvaluate(StateAnim *anim, unsigned char *key, unsigned char *value, float time);
void instNuGCutRigidSysRender(NuGCutScene *scene, float time);
void instNuGCutCharSysRender(NuGCutScene *scene, float time);
void instNuGCutLocatorSysUpdate(NuGCutScene *scene, float time);


void NuGCutSceneSysInit(NuGCutLocatorFn *fns) {
    locatorfns = fns;
    cutscenelist = 0;
}


void NuGCutSceneSysUpdate(int paused) {
    NuGCutScene *scene;

    for (scene = cutscenelist; scene; scene = scene->next) {
        if (!(scene->flags & NUGCUTSCENE_UPDATED)) {
            instNuGCutSceneUpdate(scene, paused);
            scene->flags |= NUGCUTSCENE_UPDATED;
        }
    }
}


void NuGCutSceneSysRender(void) {
    NuGCutScene *scene;

    for (scene = cutscenelist; scene; scene = scene->next) {
        instNuGCutSceneRender(scene);
        scene->flags &= ~NUGCUTSCENE_UPDATED;
    }
}


void NuSetCutSceneCharacterRenderFn(NuCutSceneCharacterRenderFn fn) {
    NuCutSceneCharacterRender = fn;
}


void NuSetCutSceneCharacterReleaseFn(NuCutSceneCharacterReleaseFn fn) {
    NuCutSceneCharacterRelease = fn;
}


void NuSetCutSceneFindCharactersFn(NuCutSceneFindCharactersFn fn) {
    NuCutSceneFindCharacters = fn;
}


void NuSetCutSceneCharacterCreateDataFn(NuCutSceneCharacterCreateDataFn fn) {
    NuCutSceneCharacterCreateData = fn;
}


void NuSetCutSceneRigidCollisionCheckFn(NuCutSceneRigidCollisionCheckFn fn) {
    NuCutSceneRigidCollisionCheck = fn;
}


int instNuGCutSceneCharGetStartMtx(NuGCutScene *scene, char *name, NuMtx *mtx) {
    NuGCutCharDef *chardef = scene->def->chardef;
    int i;

    for (i = 0; i < chardef->count; i++) {
        NuGCutCharDesc *desc = &chardef->chars[i];

        if (strcasecmp(name, desc->name) == 0) {
            *mtx = desc->mtx;
            return 1;
        }
    }
    return 0;
}

void NuGCutSceneFixUp(NuGCutSceneDef *def, void *gscene, void *triggers) {
    if (def) {
        if (def->rigiddef) {
            NuGCutRigidSysFixUp(def, gscene);
        }
        if (def->chardef) {
            NuGCutCharSysFixUp(def);
        }
        if (def->locatordef) {
            NuGCutLocatorSysFixUp(def->locatordef);
        }
        if (def->triggerdef && triggers) {
            NuGCutTriggerSysFixUp(def, triggers);
        }
    }
}


NuGCutScene *instNuGCutSceneFind(const char *name) {
    NuGCutScene *scene;

    for (scene = cutscenelist; scene; scene = scene->next) {
        if (strcasecmp(name, scene->name) == 0) {
            return scene;
        }
    }
    return 0;
}


void instNuGCutSceneDestroy(NuGCutScene *scene) {
    if (scene->flags & NUGCUTSCENE_RUNNING) {
        instNuGCutSceneEnd(scene);
    }
    if (scene->next) {
        scene->next->prev = scene->prev;
    }
    if (scene->prev) {
        scene->prev->next = scene->next;
    } else {
        cutscenelist = scene->next;
    }
}


int instNuGCutSceneIsFinished(NuGCutScene *scene) {
    return !(scene->flags & NUGCUTSCENE_RUNNING);
}


void instNuGCutSceneStart(NuGCutScene *scene) {
    scene->time = 1.0f;
    scene->flags |= NUGCUTSCENE_RUNNING;
    scene->flags &= ~NUGCUTSCENE_STARTED;
    if (scene->camsys) {
        instNuGCutCamSysStart(scene->camsys, scene->def->camdef);
    }
    if (scene->rigidsys) {
        instNuGCutRigidSysStart(scene->rigidsys, scene->def->rigiddef);
    }
    if (scene->locatorsys) {
        instNuGCutLocatorSysStart(scene->locatorsys, scene->def->locatordef);
    }
    if (scene->triggersys) {
        instNuGCutTriggerSysStart(scene);
    }
}


void instNuGCutSceneEnd(NuGCutScene *scene) {
    NuGCutSceneDef *def = scene->def;

    scene->time = def->duration + 1.0f;
    scene->flags &= ~NUGCUTSCENE_RUNNING;
    if (def->rigiddef) {
        instNuGCutRigidSysEnd(scene, scene->time);
    }
    if (scene->charsys) {
        instNuGCutCharSysEnd(scene, scene->time);
    }
    if (scene->locatorsys) {
        instNuGCutLocatorSysEnd(scene->locatorsys, def->locatordef, scene->time);
    }
}


void instNuGCutSceneReset(NuGCutScene *scene) {
    scene->flags &= ~NUGCUTSCENE_RUNNING;
    scene->flags &= ~NUGCUTSCENE_STARTED;
    scene->time = 1.0f;
    if (scene->rigidsys) {
        instNuGCutRigidSysReset(scene->rigidsys, scene->def->rigiddef);
    }
}


void instNuGCutSceneCreateCamTgtArray(NuGCutScene *scene, int count, char **heap) {
    NuGCutCamSys *camsys = scene->camsys;

    if (camsys && count) {
        camsys->max = count;
        *heap = (char *)(((unsigned int)*heap + 15) & ~15);
        camsys->targets = (NuGCutCamTgt *)*heap;
        *heap += count * sizeof(NuGCutCamTgt);
        memset(camsys->targets, 0, count * sizeof(NuGCutCamTgt));
    }
}


int instNuGCutSceneAddCamTgt(NuGCutScene *scene, void *object, char flags, float weight, float time) {
    NuGCutCamSys *camsys = scene->camsys;

    if (camsys && camsys->count < camsys->max) {
        NuGCutCamTgt *tgt = &camsys->targets[camsys->count++];
        tgt->object = object;
        tgt->weight = weight;
        tgt->time = time;
        tgt->flags = flags;
        return 1;
    }
    return 0;
}


void instNuGCutSceneSetPos(NuGCutScene *scene, NuVec4 *pos) {
    scene->flags |= NUGCUTSCENE_MATRIXSET;
    NuMtxSetTranslation(&scene->mtx, pos);
    instNuGCutSceneCalculateCentre(scene, &scene->mtx);
}


void instNuGCutSceneTranslate(NuGCutScene *scene, NuVec4 *offset) {
    if (!(scene->flags & NUGCUTSCENE_MATRIXSET)) {
        scene->flags |= NUGCUTSCENE_MATRIXSET;
        NuMtxSetIdentity(&scene->mtx);
    }
    NuMtxTranslate(&scene->mtx, offset);
    instNuGCutSceneCalculateCentre(scene, &scene->mtx);
}


void instNuGCutSceneRotateY(NuGCutScene *scene, float angle) {
    scene->flags |= NUGCUTSCENE_MATRIXSET;
    NuMtxRotateY(&scene->mtx, angle);
    instNuGCutSceneCalculateCentre(scene, &scene->mtx);
}


void instNuGCutSceneSetMtx(NuGCutScene *scene, NuMtx *mtx) {
    scene->flags |= NUGCUTSCENE_MATRIXSET;
    scene->mtx = *mtx;
    instNuGCutSceneCalculateCentre(scene, &scene->mtx);
}


void instNuGCutSceneChain(NuGCutScene *scene, NuGCutScene *chain) {
    scene->chain = chain;
}


void instNuGCutSceneSetEndCallback(NuGCutScene *scene, NuGCutSceneEndFn callback) {
    scene->endcallback = callback;
}


void instNuGCutSceneEnable(NuGCutScene *scene) {
    scene->flags &= ~NUGCUTSCENE_DISABLED;
}


void instNuGCutSceneDisable(NuGCutScene *scene) {
    scene->flags |= NUGCUTSCENE_DISABLED;
}


int NuGCutLocatorIsVisble(NuGCutLocatorData *data, float time, NuAnimTime *atime, float *value) {
    NuAnimData2 *anim = data->anim;
    int visible;

    if (anim) {
        NuAnimCurve2 *curves = anim->curves;
        signed char *curveflags = anim->curveflags;

        visible = NuAnimCurve2CalcVal(&curves[NUANIM_CURVE_VISIBLE], atime,
                                      curveflags[NUANIM_CURVE_VISIBLE]);
        if (visible && value) {
            *value = NuAnimCurve2CalcVal(&curves[NUANIM_CURVE_VALUE], atime,
                                         curveflags[NUANIM_CURVE_VALUE]);
        }
    } else {
        visible = data->flags & NUGCUTLOCATOR_VISIBLE;
        if (visible && value) {
            *value = data->value;
        }
    }
    return visible;
}


void instNuGCutLocatorUpdate(NuGCutScene *scene, NuGCutLocatorDef *def, NuGCutLocator *locator,
                             NuGCutLocatorData *data, float time, NuMtx *mtx) {
    NuGCutLocatorType *type = &def->types[data->type];

    if (type->flags & NUGCUTLOCATOR_DEBRIS) {
        instNuGCutDebrisLocatorUpdate(def, locator, data, mtx, time);
    } else if ((type->flags & NUGCUTLOCATOR_FUNC) && type->fn != NUGCUTLOCATOR_NOFN) {
        locatorfns[type->fn].func(scene, def, locator, data, mtx, time);
    }
}


void NuGCutCamsSysFixPtrs(NuGCutSceneDef *def, int base) {
    NuGCutCamDef *camdef;

    if (def->camdef) {
        camdef = (NuGCutCamDef *)((char *)def->camdef + base);
        def->camdef = camdef;
        camdef->cams = NuFixPtr(camdef->cams, base);
        if (camdef->anim) {
            camdef->anim = NuAnimData2FixPtrs(camdef->anim, base);
        }
        camdef->stateanim = StateAnimFixPtrs(camdef->stateanim, base);
    }
}


void instNuGCutSceneCalculateCentre(NuGCutScene *scene, NuMtx *mtx) {
    NuGCutSceneDef *def = scene->def;

    if (def->bounds) {
        scene->centre.x = (def->bounds->max.x + def->bounds->min.x) * 0.5f;
        scene->centre.y = (def->bounds->max.y + def->bounds->min.y) * 0.5f;
        scene->centre.z = (def->bounds->max.z + def->bounds->min.z) * 0.5f;
    } else {
        scene->centre.x = 0.0f;
        scene->centre.y = 0.0f;
        scene->centre.z = 0.0f;
    }
    if (mtx) {
        NuVecMtxTransform(&scene->centre, &scene->centre, mtx);
    }
}


NuGCutCamSys *instNuCGutCamSysCreate(NuGCutCamDef *def, char **heap) {
    NuGCutCamSys *camsys = 0;

    if (def && def->count && heap) {
        camsys = (NuGCutCamSys *)(((unsigned int)*heap + 15) & ~15);
        *heap = (char *)camsys + sizeof(NuGCutCamSys);
        memset(camsys, 0, sizeof(NuGCutCamSys));
        *heap = (char *)(((unsigned int)*heap + 15) & ~15);
        camsys->cams = (NuGCutCam *)*heap;
        *heap += def->count * sizeof(NuGCutCam);
        memset(camsys->cams, 0, def->count * sizeof(NuGCutCam));
    }
    return camsys;
}


void instNuGCutCamSysStart(NuGCutCamSys *camsys, NuGCutCamDef *def) {
    unsigned int i;

    camsys->field8 = 0;
    camsys->field9 = def->field10;
    camsys->fieldA = 0;
    for (i = 0; i < def->count; i++) {
        NuGCutCam *cam = &camsys->cams[i];
        cam->flags &= ~0x2;
        cam->state = 0;
    }
}


NuGCutLocatorSys *NuCGutLocatorSysCreateInst(NuGCutLocatorDef *def, char **heap) {
    NuGCutLocatorSys *sys = 0;

    if (def && def->count && heap) {
        sys = (NuGCutLocatorSys *)(((unsigned int)*heap + 15) & ~15);
        *heap = (char *)sys + sizeof(NuGCutLocatorSys);
        memset(sys, 0, sizeof(NuGCutLocatorSys));
        *heap = (char *)(((unsigned int)*heap + 15) & ~15);
        sys->locators = (NuGCutLocator *)*heap;
        *heap += def->count * sizeof(NuGCutLocator);
        memset(sys->locators, 0, def->count * sizeof(NuGCutLocator));
    }
    return sys;
}


void instNuGCutLocatorSysEnd(NuGCutLocatorSys *sys, NuGCutLocatorDef *def, float time) {
    unsigned int i;

    for (i = 0; i < def->count; i++) {
        NuGCutLocator *loc = &sys->locators[i];
        loc->object = 0;
    }
}


void instNuGCutLocatorSysStart(NuGCutLocatorSys *sys, NuGCutLocatorDef *def) {
    unsigned int i;

    for (i = 0; i < def->count; i++) {
        NuGCutLocator *loc = &sys->locators[i];
        loc->object = 0;
    }
}


void NuGCutRigidSysFixPtrs(NuGCutSceneDef *def, int base) {
    NuGCutRigidDef *rigiddef;
    int i;

    if (def->rigiddef) {
        rigiddef = (NuGCutRigidDef *)((char *)def->rigiddef + base);
        def->rigiddef = rigiddef;
        rigiddef->rigids = NuFixPtr(rigiddef->rigids, base);
        if (rigiddef->rigids) {
            for (i = 0; i < rigiddef->count; i++) {
                NuGCutRigidData *rigid = &rigiddef->rigids[i];

                if (rigid->name) {
                    rigid->name = def->names + (int)rigid->name - 1;
                }
                if (rigid->anim) {
                    rigid->anim = NuAnimData2FixPtrs(rigid->anim, base);
                }
                rigid->stateanim = StateAnimFixPtrs(rigid->stateanim, base);
            }
        }
    }
}


void NuGCutLocatorSysFixPtrs(NuGCutSceneDef *def, int base) {
    NuGCutLocatorDef *locatordef;
    int i;

    if (def->locatordef) {
        locatordef = (NuGCutLocatorDef *)((char *)def->locatordef + base);
        def->locatordef = locatordef;
        locatordef->locators = NuFixPtr(locatordef->locators, base);
        if (locatordef->locators) {
            for (i = 0; i < locatordef->count; i++) {
                NuGCutLocatorData *locator = &locatordef->locators[i];

                if (locator->anim) {
                    locator->anim = NuAnimData2FixPtrs(locator->anim, base);
                }
            }
        }
        locatordef->types = NuFixPtr(locatordef->types, base);
        if (locatordef->types) {
            for (i = 0; i < locatordef->ntypes; i++) {
                NuGCutLocatorType *type = &locatordef->types[i];

                type->name = def->names + (int)type->name - 1;
            }
        }
    }
}

typedef void (*nudebugmsg_fn)(char *fmt, ...);
extern nudebugmsg_fn NuDebugMsgProlog(char *file, int line);
extern int LookupDebrisEffect(char *name);

/* Retail-owned (no Data-from-C yet). */
extern char D_00619C50[];               /* source file name */
extern char D_00619D90[];               /* debug message */
#define gcutscn_srcfile D_00619C50
#define nolocatorfn_msg D_00619D90

/* Index of the locator function called `name`, or -1. */
inline int LookupLocatorFn(char *name) {
    int ix = 0;

    if (locatorfns != 0) {
        while (locatorfns[ix].name) {
            if (strcasecmp(name, locatorfns[ix].name) == 0) {
                return ix;
            }
            ix++;
        }
    }
    return -1;
}

/* Bind each locator type to its debris effect or update function. */
void NuGCutLocatorSysFixUp(NuGCutLocatorDef *def) {
    unsigned int i;

    for (i = 0; i < def->ntypes; i++) {
        NuGCutLocatorType *type = &def->types[i];

        if (type->flags & NUGCUTLOCATOR_DEBRIS) {
            type->fn = LookupDebrisEffect(type->name);
        } else if (type->flags & NUGCUTLOCATOR_FUNC) {
            type->fn = LookupLocatorFn(type->name);
        }
        if (type->fn == NUGCUTLOCATOR_NOFN) {
            NuDebugMsgProlog(gcutscn_srcfile, 0x4FE)(nolocatorfn_msg, type->name);
        }
    }
}

extern int NuSpecialFind(void *scene, NuHSpecial *special, char *name);

extern char D_00619DF8[];               /* debug message */
#define norigid_msg D_00619DF8

/* Find each rigid's object in the scene and point it at its locators. */
void NuGCutRigidSysFixUp(NuGCutSceneDef *def, void *scene) {
    NuGCutRigidDef *rigiddef;
    NuGCutLocatorDef *locatordef;
    int i;

    rigiddef = def->rigiddef;
    locatordef = def->locatordef;
    if (scene != 0) {
        for (i = 0; i < rigiddef->count; i++) {
            NuGCutRigidData *rigid = &rigiddef->rigids[i];

            if (NuSpecialFind(scene, &rigid->obj, rigid->name)) {
                rigid->flags |= NUGCUTRIGID_FOUND;
            } else {
                NuDebugMsgProlog(gcutscn_srcfile, 0x5D9)(norigid_msg, rigid->name);
            }
            if (rigid->nlocators != 0 && (int)rigid->locators < 0xFF) {
                rigid->first_locator = (int)rigid->locators;
                rigid->locators = &locatordef->locators[(int)rigid->locators];
            } else {
                rigid->first_locator = 0xFF;
            }
        }
    }
}

/* The scene's trigger definitions (full layout in gamelib/trigger.c). */
typedef struct NuTriggerDef {
    char *name;                   /* 0x00 */
    char pad04[0x34 - 0x04];      /* 0x04 */
} NuTriggerDef; /* 0x34 */

typedef struct NuTriggerDefHdr {
    int version;                  /* 0x00 */
    int reloc;                    /* 0x04 */
    int count;                    /* 0x08 */
    NuTriggerDef *defs;           /* 0x0C */
} NuTriggerDefHdr;

typedef void (*nuerror_fn)(char *fmt, ...);
extern nuerror_fn NuErrorProlog(char *file, int line);

extern char D_00619E90[];               /* error message */
#define notrigger_msg D_00619E90

/* Point each cutscene trigger at the scene trigger with the same name. */
void NuGCutTriggerSysFixUp(NuGCutSceneDef *def, void *triggers) {
    NuTriggerDefHdr *triggersys = triggers;
    NuGCutTriggerDef *triggerdef;
    int i;
    int j;

    triggerdef = def->triggerdef;
    for (i = 0; i < triggerdef->count; i++) {
        NuGCutTriggerData *trigger = &triggerdef->triggers[i];

        for (j = 0; j < triggersys->count; j++) {
            trigger->target = -1;
            if (strcasecmp(trigger->name, triggersys->defs[j].name) == 0) {
                trigger->target = j;
                break;
            }
        }
        if (trigger->target == -1) {
            NuErrorProlog(gcutscn_srcfile, 0x8A8)(notrigger_msg, trigger->name);
        }
    }
}

void NuGCutCharSysFixPtrs(NuGCutSceneDef *def, int base) {
    NuGCutCharDef *chardef;
    int i;

    if (def->chardef) {
        chardef = (NuGCutCharDef *)((char *)def->chardef + base);
        def->chardef = chardef;
        chardef->chars = NuFixPtr(chardef->chars, base);
        if (chardef->chars) {
            for (i = 0; i < chardef->count; i++) {
                NuGCutCharDesc *desc = &chardef->chars[i];

                if (desc->name) {
                    desc->name = def->names + (int)desc->name - 1;
                }
                if (desc->char_anim) {
                    desc->char_anim = NuAnimData2FixPtrs(desc->char_anim, base);
                }
                if (desc->joint_anim) {
                    desc->joint_anim = NuAnimData2FixPtrs(desc->joint_anim, base);
                }
                if (desc->face_anim) {
                    desc->face_anim = NuAnimData2FixPtrs(desc->face_anim, base);
                }
            }
        }
    }
}

void instNuGCutRigidSysReset(NuGCutRigidSys *sys, NuGCutRigidDef *def) {
    unsigned int i;

    for (i = 0; i < def->count; i++) {
        NuGCutRigidData *data = &def->rigids[i];
        NuGCutRigid *rigid = &sys->rigids[i];

        rigid->state = 0;
        if (!(data->flags & NUGCUTRIGID_FOUND)) {
            continue;
        }
        if (data->flags & NUGCUTRIGID_ANIMATED) {
            continue;
        }
        if (data->flags & NUGCUTRIGID_VISIBLE) {
            rigid->obj.special->instance->flags |= NUINSTANCE_VISIBLE;
        } else {
            rigid->obj.special->instance->flags &= ~NUINSTANCE_VISIBLE;
        }
        rigid->obj.special->instance->matrix = data->mtx;
    }
}


void instNuGCutRigidSysStart(NuGCutRigidSys *sys, NuGCutRigidDef *def) {
    unsigned int i;

    for (i = 0; i < def->count; i++) {
        NuGCutRigid *rigid = &sys->rigids[i];
        rigid->state = 0;
    }
}


void NuGCutCharSysFixUp(NuGCutSceneDef *def) {
    if (NuCutSceneFindCharacters) {
        NuCutSceneFindCharacters();
    }
}


extern void NuGCutRigidCalcMtx(NuGCutRigidData *rigid, float time, NuMtx *mtx);
extern void NuMtxMul(NuMtx *dst, NuMtx *a, NuMtx *b);

void instNuGCutRigidSysEnd(NuGCutScene *scene, float time) {
    NuGCutRigidDef *rigiddef = scene->def->rigiddef;
    NuGCutRigidSys *sys = scene->rigidsys;
    NuMtx mtx;
    signed char state;
    int i;

    for (i = 0; i < rigiddef->count; i++) {
        NuGCutRigidData *data = &rigiddef->rigids[i];
        NuGCutRigid *rigid = &sys->rigids[i];

        if (!(data->flags & 0x4)) {
            continue;
        }
        if (!(data->flags & 0x2)) {
            if (data->stateanim && StateAnimEvaluate(data->stateanim, &rigid->state, (unsigned char *)&state, time)) {
                if (state) {
                    rigid->obj.special->instance->flags |= NUINSTANCE_VISIBLE;
                } else {
                    rigid->obj.special->instance->flags &= ~NUINSTANCE_VISIBLE;
                }
            }
            if (rigid->obj.special->instance->flags & NUINSTANCE_VISIBLE) {
                NuGCutRigidCalcMtx(data, time, &mtx);
                if (scene->flags & NUGCUTSCENE_MATRIXSET) {
                    NuMtxMul(&mtx, &mtx, &scene->mtx);
                }
            }
        }
    }
}

void instNuGCutCharSysEnd(NuGCutScene *scene, float time) {
    NuGCutSceneDef *def = scene->def;
    NuGCutCharDef *chardef = def->chardef;
    NuGCutCharSys *sys = scene->charsys;
    int i;

    for (i = 0; i < chardef->count; i++) {
        NuGCutCharDesc *desc = &chardef->chars[i];
        NuGCutChar *character = &sys->chars[i];

        if (character->data && !(desc->flags & 0x2)) {
            if (NuCutSceneCharacterRender) {
                NuCutSceneCharacterRender(scene, def, character, desc, time);
            }
            if (NuCutSceneCharacterRelease) {
                NuCutSceneCharacterRelease(character);
            }
        }
    }
}


void NuGCutTriggerSysFixPtrs(NuGCutSceneDef *def, int base) {
    NuGCutTriggerDef *triggerdef;
    int i;

    if (def->triggerdef) {
        triggerdef = (NuGCutTriggerDef *)((char *)def->triggerdef + base);
        def->triggerdef = triggerdef;
        triggerdef->triggers = NuFixPtr(triggerdef->triggers, base);
        if (triggerdef->triggers) {
            for (i = 0; i < triggerdef->count; i++) {
                NuGCutTriggerData *trigger = &triggerdef->triggers[i];

                trigger->name = NuFixPtr(trigger->name, base);
                trigger->anim = StateAnimFixPtrs(trigger->anim, base);
            }
        }
    }
}


NuGCutTriggerSys *instNuCGutTriggerSysCreate(NuGCutSceneDef *def, NuGCutTriggerList *list, char **heap) {
    NuGCutTriggerSys *sys;
    int count = def->triggerdef->count;

    sys = (NuGCutTriggerSys *)(((unsigned int)*heap + 15) & ~15);
    *heap = (char *)sys + sizeof(NuGCutTriggerSys);
    memset(sys, 0, sizeof(NuGCutTriggerSys));
    sys->list = list;
    sys->triggers = (NuGCutTrigger *)*heap;
    *heap += count * sizeof(NuGCutTrigger);
    memset(sys->triggers, 0, count * sizeof(NuGCutTrigger));
    return sys;
}


extern int sprintf(char *buf, const char *fmt, ...);
extern NuGCutRigidSys *instNuCGutRigidSysCreate(NuGCutSceneDef *def, void *gscene, char **heap);

NuGCutCharSys *instNuCGutCharSysCreate(NuGCutSceneDef *def, char **heap) {
    NuGCutCharDef *chardef;
    NuGCutCharSys *charsys;
    NuGCutCharDesc *desc;
    NuGCutChar *character;
    int i;

    charsys = 0;
    chardef = def->chardef;
    if (chardef && chardef->count && heap) {
        charsys = (NuGCutCharSys *)(*heap = (char *)(((unsigned int)*heap + 15) & ~15));
        *heap = (char *)(charsys + 1);
        charsys->chars = 0;
        character = (NuGCutChar *)(*heap = (char *)(((unsigned int)*heap + 15) & ~15));
        charsys->chars = character;
        *heap += chardef->count * sizeof(NuGCutChar);
        memset(charsys->chars, 0, chardef->count * sizeof(NuGCutChar));
        for (i = 0; i < chardef->count; i++) {
            desc = &chardef->chars[i];
            character = &charsys->chars[i];
            character->current_animix = -1;
            character->prev_animix = -1;
            if (desc->flags & 0x2) {
                if (NuCutSceneCharacterCreateData) {
                    NuCutSceneCharacterCreateData(desc, character, heap);
                }
            } else {
                character->data = desc->character;
            }
        }
    }
    return charsys;
}

NuGCutScene *instNuGCutSceneCreate(NuGCutSceneDef *def, void *gscene, NuGCutTriggerList *triggers,
                                   char *name, char **heap) {
    NuGCutScene *scene;

    scene = 0;
    if (def) {
        scene = (NuGCutScene *)(*heap = (char *)(((unsigned int)*heap + 15) & ~15));
        *heap = (char *)(scene + 1);
        memset(scene, 0, sizeof(NuGCutScene));
        scene->next = cutscenelist;
        if (cutscenelist) {
            cutscenelist->prev = scene;
        }
        cutscenelist = scene;
        scene->def = def;
        if (name) {
            sprintf(scene->name, name);
        }
        if (def->camdef) {
            scene->camsys = instNuCGutCamSysCreate(def->camdef, heap);
        }
        if (def->rigiddef) {
            scene->rigidsys = instNuCGutRigidSysCreate(def, gscene, heap);
        }
        if (def->chardef) {
            scene->charsys = instNuCGutCharSysCreate(def, heap);
        }
        if (def->locatordef) {
            scene->locatorsys = NuCGutLocatorSysCreateInst(def->locatordef, heap);
        }
        if (def->triggerdef && triggers) {
            scene->triggersys = instNuCGutTriggerSysCreate(def, triggers, heap);
        }
        instNuGCutSceneCalculateCentre(scene, 0);
        scene->speed = 1.0f;
    }
    return scene;
}


void instNuGCutTriggerSysUpdate(NuGCutScene *scene, float time) {
    NuGCutTriggerDef *def = scene->def->triggerdef;
    NuGCutTriggerSys *sys = scene->triggersys;
    unsigned char state;
    int i;

    for (i = 0; i < def->count; i++) {
        NuGCutTriggerData *trigger = &def->triggers[i];

        if (trigger->anim && StateAnimEvaluate(trigger->anim, &sys->triggers[i].state, &state, time)) {
            if (state) {
                sys->list->targets[trigger->target].flags |= 0x1;
            } else {
                sys->list->targets[trigger->target].flags &= ~0x1;
            }
        }
    }
}


void instNuGCutTriggerSysStart(NuGCutScene *scene) {
    NuGCutSceneDef *def = scene->def;
    NuGCutTriggerSys *sys = scene->triggersys;
    NuGCutTriggerDef *tdef = def->triggerdef;
    int i;

    for (i = 0; i < tdef->count; i++) {
        NuGCutTrigger *trigger = &sys->triggers[i];
        trigger->state = 0;
    }
}


void instNuGCutSceneRender(NuGCutScene *scene) {
    if (scene->flags & NUGCUTSCENE_DISABLED) {
        return;
    }
    if (!(scene->flags & NUGCUTSCENE_RUNNING)) {
        return;
    }
    if (!(scene->flags & NUGCUTSCENE_VISIBLE)) {
        return;
    }
    if (cutscene_rndr_rigids && scene->rigidsys) {
        instNuGCutRigidSysRender(scene, scene->time);
    }
    if (cutscene_rndr_chars && scene->charsys) {
        instNuGCutCharSysRender(scene, scene->time);
    }
    if (cutscene_rndr_locators && scene->locatorsys) {
        instNuGCutLocatorSysUpdate(scene, scene->time);
    }
}


int StateAnimEvaluate(StateAnim *anim, unsigned char *key, unsigned char *value, float time) {
    int changed = 0;

    if (*key < anim->count) {
        do {
            if (!(anim->times[*key] <= time)) {
                break;
            }
            *value = anim->values[*key];
            (*key)++;
            changed = 1;
        } while (*key < anim->count);
    }
    if (!changed) {
        if (*key != 0) {
            do {
                if (!(time < anim->times[*key - 1])) {
                    break;
                }
                (*key)--;
                *value = anim->values[*key];
                changed = 1;
            } while (*key != 0);
        }
    }
    return changed;
}


StateAnim *StateAnimFixPtrs(StateAnim *anim, int base) {
    anim = NuFixPtr(anim, base);
    if (anim) {
        anim->times = NuFixPtr(anim->times, base);
        anim->values = NuFixPtr(anim->values, base);
    }
    return anim;
}


void instNuGCutLocatorSysUpdate(NuGCutScene *scene, float time) {
    NuGCutLocatorDef *def = scene->def->locatordef;
    NuGCutLocatorSys *sys = scene->locatorsys;
    unsigned int i;

    for (i = 0; i < def->count; i++) {
        NuGCutLocatorData *data = &def->locators[i];

        if (data->flags & 0x1) {
            continue;
        }
        if (data->flags & 0x2) {
            continue;
        }
        if (scene->flags & NUGCUTSCENE_MATRIXSET) {
            instNuGCutLocatorUpdate(scene, def, &sys->locators[i], data, time, &scene->mtx);
        } else {
            instNuGCutLocatorUpdate(scene, def, &sys->locators[i], data, time, 0);
        }
    }
}


void instNuGCutCharSysRender(NuGCutScene *scene, float time) {
    NuGCutSceneDef *def = scene->def;
    NuGCutCharSys *sys = scene->charsys;
    NuGCutCharDef *chardef = def->chardef;
    int i;

    if (NuCutSceneCharacterRender) {
        for (i = 0; i < chardef->count; i++) {
            NuGCutCharDesc *desc = &chardef->chars[i];
            NuGCutChar *character = &sys->chars[i];

            if (character->data) {
                NuCutSceneCharacterRender(scene, def, character, desc, time);
            }
        }
    }
}


extern char D_00619C78[];               /* error message */
extern char D_00619CD8[];               /* error message */
#define loadfail_msg D_00619C78
#define badversion_msg D_00619CD8
extern int NuFileLoadBuffer(char *name, void *buf, int size);

NuGCutSceneDef *NuGCutSceneLoad(char *name, char **heap, char **heapend) {
    NuGCutSceneDef *def;
    int size;

    *heap = (char *)(((unsigned int)*heap + 15) & ~15);
    size = NuFileLoadBuffer(name, *heap, *heapend - *heap);
    if (size == 0) {
        NuErrorProlog(gcutscn_srcfile, 0x14C)(loadfail_msg);
    }
    def = (NuGCutSceneDef *)*heap;
    *heap = (char *)def + size;
    if (def->version != 1) {
        NuErrorProlog(gcutscn_srcfile, 0x151)(badversion_msg, name, def->version);
    }
    def->base = (int)def - def->base;
    def->names = def->names ? def->names + def->base : 0;
    if (def->camdef) {
        NuGCutCamsSysFixPtrs(def, def->base);
    }
    if (def->locatordef) {
        NuGCutLocatorSysFixPtrs(def, def->base);
    }
    if (def->rigiddef) {
        NuGCutRigidSysFixPtrs(def, def->base);
    }
    if (def->chardef) {
        NuGCutCharSysFixPtrs(def, def->base);
    }
    if (def->triggerdef) {
        NuGCutTriggerSysFixPtrs(def, def->base);
    }
    def->bounds = def->bounds ? (NuGCutBounds *)((char *)def->bounds + def->base) : 0;
    return def;
}
