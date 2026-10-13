/*
 * Unit: game/cut
 *
 * Functions:
 *   0x0025b850 AppCutSceneCharacterRender
 *   0x0025ba88 LoadCutComponents
 *   0x0025bd98 LoadCutMovie
 *   0x0025c050 SetCutMovieRate
 *   0x0025c0e8 CloseCutMovie
 *   0x0025c210 StartCutMovie
 *   0x0025c368 PlayCutMovie
 *   0x0025c8c0 InitGameCut
 *   0x0025ca10 UpdateGameCut
 *   0x0025ce20 DrawGameCut
 *   0x0025d0d0 GetSpaceCut
 *   0x0025d4e0 SetCutSceneLights
 *   0x0025e598 InitCutScenes
 *   0x0025e5d0 UpdateCutMovie
 *   0x0025e650 DrawCutMovie
 *   0x0025e670 NewCut
 *   0x0025e698 LoadCutSceneCharacters
 *   0x0025e798 FindCutChar
 *   0x0025e808 AppCutSceneFindCharacters
 *   0x0025e948 locatorfn_fadeout
 *   0x0025ea08 CutSceneEndFunction
 *   0x0025ea60 CutSceneNextFunction
 *   0x0025eb48 CutSceneNextLogoFunction
 *   0x0025eb98 UpdateCutMovieCamera
 *   0x0025ecd0 NewGameCutAnim
 *   0x0025ed88 MakeDirLightColour
 *   0x0025edb8 MakeLightDirection
 */

#include "creature.h"

struct game_s {
    u8 unk_0x00[0xF];
    u8 language;          /* 0xF */
    u8 unk_0x10[0x408 - 0x10];
    s32 cutbits;          /* 0x408 */
};

extern struct game_s Game;
extern s32 gamecut;
extern struct nucolour3_s cutdircol[];
extern struct nuvec_s cutdir[];

extern void NuGCutSceneSysRender(void);
extern void NuVecNorm(struct nuvec_s *out, struct nuvec_s *in);

extern s32 cutmovie;
extern s32 cut_on;
extern s32 logos_played;
extern char Cursor[];
extern s32 D_00633388;                 /* cutworldix */

struct csc_s {
    void *obj;            /* 0x0 */
    char *path;           /* 0x4 */
    char *name;           /* 0x8 */
};                        /* 0xC */

extern struct csc_s *CutChar;

struct csccache_s {
    char *path;          /* 0x0 */
    void *obj;           /* 0x4 */
};
extern struct csccache_s D_006FF880[];     /* cut char cache */
extern s32 D_0063338C;                     /* cut char cache count */
extern void *superbuffer_ptr;
extern void *NuGHGRead(void *buf, char *path);

extern void NewMenu(void *cursor, s32 a, s32 b, s32 c);
extern void GameMusic(s32 sfx, s32 i);
extern s32 strcasecmp(char *a, char *b);

extern s32 cutmovie_hold;
extern s32 Paused;
extern u8 D_0060B398[];                 /* cutscene_locatorfns */

struct cutinst_s {
    u8 unk_0x00[0x6C];
    u32 flags;            /* 0x6C (4 = loop) */
    f32 cframe;           /* 0x70 */
    f32 rate;             /* 0x74 */
    u8 unk_0x78[0x8C - 0x78];
    struct cutinst_s *next; /* 0x8C next to play */
};
extern struct nugcutscene_s *D_006FF680[]; /* CutScene */
extern struct cutinst_s *D_006FF700[];  /* CutInst */
extern s32 D_006FF780[];                /* CutAudio */
#define CutScene D_006FF680
#define CutInst D_006FF700
#define CutAudio D_006FF780
#define cutworldix D_00633388

extern u32 texanimbits;
extern f32 TITLESTARTTIME;
extern f32 TITLEFPS;
extern void instNuGCutSceneStart(struct cutinst_s *inst);

static void AppCutSceneFindCharacters();
extern void NuGCutSceneSysInit(void *fns);
extern void NuSetCutSceneFindCharactersFn(void *fn);
extern void NuSetCutSceneCharacterRenderFn(void *fn);
extern s32 NuSoundKeyStatus(s32 chan);
extern void SetCutMovieRate(void);
extern void NuGCutSceneSysUpdate(s32 paused);

extern f32 D_0062E888;
extern f32 D_0062E88C;
extern f32 D_00632BD8;

struct pcutanm_s {
    s16 character;    /* 0x0 */
    s16 action;       /* 0x2 */
    s16 sfx;          /* 0x4 */
    s8 i;             /* 0x6 */
    s8 pad1;          /* 0x7 */
};
extern struct pcutanm_s *pCutAnim;
extern s32 gamecut_hold;
extern s32 gamecut_sfx;
extern s32 gamesfx_channel;
extern s32 Level;
extern u8 D_0058B117[];
extern void NuSoundStopStream(s32 chan);
extern void GameSfx(s32 sfx, s32 a);

struct nugcutlocator_s {
    u8 pad0[0x50];
    void *anim;                      /* 0x50 */
    u8 pad1[0x5A - 0x54];
    u8 joint_ix;                     /* 0x5A (0xFF = not on a joint) */
    u8 pad2[0x64 - 0x5B];
};                                   /* 0x64 */
struct nugcutlocatorsys_s {
    struct nugcutlocator_s *locators; /* 0x0 */
};
struct nugcutchar_s {
    u8 mtx[0x40];                    /* 0x0 */
    char *name;                      /* 0x40 */
    u8 pad1[0x48 - 0x44];
    void *joint_anim;                /* 0x48 */
    void *face_anim;                 /* 0x4C */
    void *character;                 /* 0x50 */
    struct nugcutlocator_s *locators; /* 0x54 */
    u8 pad2[0x5E - 0x58];
    u8 nlocators;                    /* 0x5E */
    u8 pad3;                         /* 0x5F */
    u8 first_locator;                /* 0x60 */
    u8 pad4[0x64 - 0x61];
};                                   /* 0x64 */
struct nugcutcharsys_s {
    struct nugcutchar_s *chars;      /* 0x0 */
    u16 nchars;                      /* 0x4 */
};
struct nugcutscene_s {
    u8 pad0[0x8];
    f32 nframes;                     /* 0x8 */
    u8 pad1[0x18 - 0xC];
    struct nugcutcharsys_s *chars;   /* 0x18 */
    struct nugcutlocatorsys_s *locators; /* 0x1C */
};
struct instnugcutscene_s {
    u8 pad0[0x18];
    struct numtx_s mtx;              /* 0x18 */
    struct nugcutscene_s *cutscene;  /* 0x58 */
    u8 pad1[0x6C - 0x5C];
    u32 flags;                       /* 0x6C (0x10 = has mtx) */
    f32 cframe;                      /* 0x70 */
    u8 pad2[0x84 - 0x74];
    struct instnugcutlocatorsys_s *ilocators; /* 0x84 */
};
struct instnugcutlocator_s {
    f32 timer;                       /* 0x0 */
    void *data;                      /* 0x4 */
};
struct nuanimtime_s {
    f32 time;                        /* 0x0 */
    f32 time_offset;                 /* 0x4 */
    s32 chunk;                       /* 0x8 */
    u32 time_mask;                   /* 0xC */
    u32 time_byte;                   /* 0x10 */
};

extern s32 fade_rate;
extern s32 fadeval;
extern void NuSoundUpdate(void);
extern void sceGsSyncV(s32 mode);
extern void *world_scene[];
extern void instNuGCutSceneDestroy(void *inst);
extern void NuHGobjDestroy(void *obj);
extern void NuSoundFlushLoops(void);
extern void NuTexInit(void);
extern void NuMtlRelease(void);
extern void NuGobjInit(void);
extern s32 set_cutscenecammtx;
extern struct numtx_s cutscenecammtx;
extern struct numtx_s *pNuCam;
extern void *memcpy(void *dst, void *src, s32 n);
extern void NuCameraSet(void *cam);

extern struct nuvec_s cutpos_CRASH;
extern struct nuvec_s cutpos_FRONTEND[];
extern struct nuvec_s cutpos_SPACE[];
extern struct nuvec_s campos_SPACE;
extern u16 cutang_FRONTEND[2];
extern u16 cutang_SPACE[3];
extern void *SpaceGameCutTab[][2];
extern u8 CutAnim[];
extern u8 CutVortexAnim[];
extern s32 gamecut_finished;
extern s32 NuAtan2D(f32 x, f32 z);
extern void NuAnimData2CalcTime(void *anim, f32 currf, struct nuanimtime_s *atime);
extern s32 NuGCutLocatorIsVisble(void *locator, f32 currf, struct nuanimtime_s *atime,
                                 void *null);


struct instnugcutlocator_s;
struct instnugcutlocatorsys_s {
    struct instnugcutlocator_s *ilocators; /* 0x0 */
};
struct instnugcutchar_s {
    struct NUHGOBJ_s *character; /* 0x0 */
};
struct NUHGOBJ_s {
    u8 pad0[0x7E];
    u8 num_layers;                   /* 0x7E */
};

extern s32 NuHGobjRndrMtxDwa(struct NUHGOBJ_s *hobj, struct numtx_s *m,
                             s32 nlayers, short *layer, struct numtx_s *tmtx,
                             float **dwa);

extern void NuGCutCharAnimProcess(struct nugcutchar_s *cutchar, f32 frame,
                                  struct numtx_s *mtx, s32 *is_visible,
                                  void *a, void *b, void *c);
extern void NuHGobjEvalAnim2(struct NUHGOBJ_s *obj, void *joint_anim, f32 frame,
                             s32 njoints, void *joints, struct numtx_s *tmtx);
extern void NuHGobjEval(struct NUHGOBJ_s *obj, s32 njoints, void *joints,
                        struct numtx_s *tmtx);
extern float **NuHGobjEvalDwa2(s32 nlayers, short *layers, void *face_anim, f32 frame);
extern void NuMtxMul(struct numtx_s *dst, struct numtx_s *a, struct numtx_s *b);
extern void NuMtxMulVU0(struct numtx_s *dst, struct numtx_s *a, struct numtx_s *b);
extern void instNuGCutLocatorUpdate(struct instnugcutscene_s *icutscene,
                                    struct nugcutlocatorsys_s *locatorsys,
                                    struct instnugcutlocator_s *ilocator,
                                    struct nugcutlocator_s *locator, f32 frame,
                                    struct numtx_s *mtx);

/* Cutscene character render callback: animate and draw one character, then
 * update the locators attached to it (on a joint, or to the character). */
void AppCutSceneCharacterRender(struct instnugcutscene_s *icutscene,
                                struct nugcutscene_s *cutscene,
                                struct instnugcutchar_s *icutchar,
                                struct nugcutchar_s *cutchar, f32 current_frame) {
    struct numtx_s mtx;
    struct numtx_s tmtx[256];
    short layertab[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    struct numtx_s locatormtx;
    s32 is_visible;
    struct NUHGOBJ_s *obj;
    float **dwa;
    struct nugcutlocatorsys_s *locatorsys;
    struct instnugcutlocatorsys_s *ilocatorsys;
    struct nugcutlocator_s *locator;
    s32 locix;
    s32 j;

    obj = icutchar->character;
    NuGCutCharAnimProcess(cutchar, current_frame, &mtx, &is_visible, 0, 0, 0);
    if (icutscene->flags & 0x10) {
        NuMtxMul(&mtx, &mtx, &icutscene->mtx);
    }
    if (is_visible == 0) {
        return;
    }

    if (cutchar->joint_anim != 0) {
        NuHGobjEvalAnim2(obj, cutchar->joint_anim, current_frame, 0, 0, tmtx);
    } else {
        NuHGobjEval(obj, 0, 0, tmtx);
    }
    if (cutchar->face_anim != 0) {
        dwa = NuHGobjEvalDwa2(obj->num_layers, layertab, cutchar->face_anim, current_frame);
    } else {
        dwa = 0;
    }
    NuHGobjRndrMtxDwa(obj, &mtx, obj->num_layers, layertab, tmtx, dwa);

    if (cutchar->first_locator == 0xFF) {
        return;
    }
    locatorsys = cutscene->locators;
    ilocatorsys = icutscene->ilocators;
    for (j = 0; j < cutchar->nlocators; j++) {
        locix = cutchar->first_locator + j;
        locator = &locatorsys->locators[locix];
        if (locator->joint_ix == 0xFF) {
            instNuGCutLocatorUpdate(icutscene, locatorsys, &ilocatorsys->ilocators[locix],
                                    locator, current_frame, &mtx);
        } else {
            NuMtxMulVU0(&locatormtx, &tmtx[locator->joint_ix], &mtx);
            instNuGCutLocatorUpdate(icutscene, locatorsys, &ilocatorsys->ilocators[locix],
                                    &locatorsys->locators[locix], current_frame, &locatormtx);
        }
    }
}

struct cutscenedesc_s {
    char *gscname;        /* 0x0 scene (.gsc) */
    char *cutname;        /* 0x4 cutscene (.cut) */
    s32 sfxid;            /* 0x8 audio stream */
    s32 debgroup;         /* 0xC */
};                        /* 0x10 */

/* Cutscene tables (scenes/cuts, characters); unnamed in the symbol map, named
 * here after the files they load. */
extern struct cutscenedesc_s D_0060AFE0[]; /* levels\\b\\logos\\logos.gsc, ... */
extern struct csc_s D_0060AFB8[];        /* levels\\b\\logos\\ttchar.ghg, ... */
extern struct cutscenedesc_s D_0060B160[]; /* levels\\b\\intro\\space.gsc, ... */
extern struct csc_s D_0060B010[];        /* levels\\b\\intro\\cortex.ghg, ... */
extern struct cutscenedesc_s D_0060B280[]; /* levels\\b\\outro\\station.gsc, ... */
extern struct csc_s D_0060B220[];        /* levels\\b\\outro\\crunch.ghg, ... */
extern struct cutscenedesc_s D_0060B358[]; /* levels\\b\\outro\\station.gsc, outro2 */
extern struct csc_s D_0060B2A0[];        /* levels\\b\\outro\\Crunch.ghg, ... */
#define csd_logos D_0060AFE0
#define csc_logos D_0060AFB8
#define csd_intro D_0060B160
#define csc_intro D_0060B010
#define csd_outro D_0060B280
#define csc_outro D_0060B220
#define csd_outro2 D_0060B358
#define csc_outro2 D_0060B2A0

/* Retail-owned strings (no Data-from-C yet). */
extern char D_00632BD0[];                /* "ats.dat" */
extern char D_006293A0[];                /* "ats\\levels\\pal\\title.ats" */
extern char D_006293C0[];                /* "ats\\splash\\ripples.ats" */
extern char D_006293D8[];                /* "ats\\elecy\\elecy2.ats" */
#define ats_dat D_00632BD0
#define title_ats D_006293A0
#define ripples_ats D_006293C0
#define elecy2_ats D_006293D8

extern s32 D_00633390;                   /* scene instance count */
extern u8 texanimbuff[];
extern void LoadFont3D(void);
extern void *NuDatOpen(char *name, char **mem, s32 *used);
extern void instNuGCutSceneSetEndCallback(struct cutinst_s *inst, void *fn);
void CutSceneNextLogoFunction(void *icutscene);

struct scenecache_s {
    char *path;           /* 0x0 */
    void *scene;          /* 0x4 */
};
extern struct scenecache_s D_006FF980[];   /* loaded scenes, by path */
extern s32 D_006FF800[];                   /* cutdebgroups */
extern void *superbuffer_end;
extern void InitSpecular(void);
extern void InitEnviro(void);
extern void *NuGScnRead(void **buf, void *end, char *path);
extern struct nugcutscene_s *NuGCutSceneLoad(char *path, void **buf, void **end);
extern void NuGCutSceneFixUp(struct nugcutscene_s *cutscene, void *scene, void *triggers);
extern struct cutinst_s *instNuGCutSceneCreate(struct nugcutscene_s *cutscene,
                                               void *a, void *b, char *name,
                                               void **buf);
extern u32 strlen(char *s);
void CutSceneEndFunction(void *icutscene);
void CutSceneNextFunction(void *icutscene);

/* Load each character model once, sharing already-loaded ones by path.
 * Inline: LoadCutComponents carries a copy of it in retail. */
inline void LoadCutSceneCharacters(struct csc_s *name) {
    s32 i;
    void *obj;
    void *tmp;
    char *path;

    while (name->path != 0) {
        path = name->path;
        for (i = 0; i < D_0063338C; i++) {
            if (strcasecmp(path, D_006FF880[i].path) == 0) {
                obj = D_006FF880[i].obj;
                goto found;
            }
        }
        D_006FF880[i].path = path;
        tmp = NuGHGRead(&superbuffer_ptr, path);
        D_006FF880[i].obj = tmp;
        obj = tmp;
        D_0063338C++;
    found:
        name->obj = obj;
        name++;
    }
}

/* Load the characters and every scene/cutscene of a movie, sharing models and
 * scenes already loaded, and chain the cutscenes so each plays the next.
 * Returns the number of cutscenes. */
/* Near-match (state=asm): control flow, frame size (0xB0) and the inlined
 * character cache match retail; the remaining diff is how gcc keeps the two
 * counters (retail: one in $fp plus `ix + 1` spilled at 0($sp)). Tried: for
 * with/without init, while, a single counter, ret/ix declaration order. */
s32 LoadCutComponents(struct cutscenedesc_s *csd, struct csc_s *chars) {
    s32 ret = 0;
    s32 ix = 0;
    s32 i;
    char *path;
    void *scene;
    void *end;

    InitSpecular();
    InitEnviro();
    CutChar = chars;
    LoadCutSceneCharacters(chars);

    for (; csd[ix].gscname != 0; ix++, ret++) {
        D_006FF800[ix] = csd[ix].debgroup;

        path = csd[ix].gscname;
        end = superbuffer_end;
        for (i = 0; i < D_00633390; i++) {
            if (strcasecmp(path, D_006FF980[i].path) == 0) {
                scene = D_006FF980[i].scene;
                goto found;
            }
        }
        D_006FF980[i].path = path;
        scene = NuGScnRead(&superbuffer_ptr, end, path);
        D_006FF980[i].scene = scene;
        D_00633390++;
    found:
        world_scene[ix] = scene;

        CutScene[ix] = NuGCutSceneLoad(csd[ix].cutname, &superbuffer_ptr, &superbuffer_end);
        if (CutScene[ix] != 0) {
            NuGCutSceneFixUp(CutScene[ix], world_scene[ix], 0);
            /* The instance is named after the last 16 characters of the path. */
            path = csd[ix].cutname;
            if (strlen(path) != 0x10) {
                path -= 0x10;
                path += strlen(csd[ix].cutname);
            }
            CutInst[ix] = instNuGCutSceneCreate(CutScene[ix], 0, 0, path, &superbuffer_ptr);
            if (CutInst[ix] != 0) {
                CutInst[ix]->rate = 0.6f;
                CutAudio[ix] = csd[ix].sfxid;
                instNuGCutSceneSetEndCallback(CutInst[ix], CutSceneEndFunction);
                if (ret != 0 && CutInst[ix - 1] != 0) {
                    CutInst[ix - 1]->next = CutInst[ix];
                    instNuGCutSceneSetEndCallback(CutInst[ix - 1], CutSceneNextFunction);
                }
            }
        }
    }
    return ret;
}

/* Reset the cutscene state and load movie `movie`: 0 = logos/title,
 * 1 = intro, 3 = outro, 4 = outro2. Returns whether a cutscene was loaded. */
/* Near-match (state=asm), 652/696 bytes: same instructions; retail allocates
 * $v0 where this build picks $v1 for the switch's `1` and for &texanimbuff in
 * every case. No `case 2` (adding one emits a jump table retail lacks). */
s32 LoadCutMovie(s32 movie) {
    void *buf;
    void *dat;
    s32 count;
    s32 i;

    LoadFont3D();
    D_00633390 = 0;
    for (i = 0x1F; i >= 0; i--) {
        world_scene[i] = 0;
    }
    D_0063338C = 0;
    for (i = 0; i < 0x20; i++) {
        CutScene[i] = 0;
        CutInst[i] = 0;
        CutAudio[i] = -1;
    }
    CutChar = 0;
    cutworldix = 0;
    NuTexAnimProgSysInit();

    switch (movie) {
    case 0:
        dat = NuDatOpen(ats_dat, 0, 0);
        NuDatSet(dat);
        buf = texanimbuff;
        NuTexAnimProgReadScript(&buf, title_ats);
        NuDatSet(0);
        if (dat != 0) {
            NuDatClose(dat);
        }
        count = LoadCutComponents(csd_logos, csc_logos);
        instNuGCutSceneSetEndCallback(CutInst[0], CutSceneNextLogoFunction);
        CutInst[count - 1]->flags |= 4;
        instNuGCutSceneSetEndCallback(CutInst[count - 1], 0);
        break;
    case 1:
        dat = NuDatOpen(ats_dat, 0, 0);
        NuDatSet(dat);
        buf = texanimbuff;
        NuTexAnimProgReadScript(&buf, ripples_ats);
        NuDatSet(0);
        if (dat != 0) {
            NuDatClose(dat);
        }
        LoadCutComponents(csd_intro, csc_intro);
        break;
    case 3:
    case 4:
        dat = NuDatOpen(ats_dat, 0, 0);
        NuDatSet(dat);
        buf = texanimbuff;
        NuTexAnimProgReadScript(&buf, elecy2_ats);
        if (movie == 4) {
            NuTexAnimProgReadScript(&buf, ripples_ats);
        }
        NuDatSet(0);
        if (dat != 0) {
            NuDatClose(dat);
        }
        if (movie == 3) {
            LoadCutComponents(csd_outro, csc_outro);
        } else {
            LoadCutComponents(csd_outro2, csc_outro2);
        }
        break;
    }
    return CutInst[0] != 0;
}

void SetCutMovieRate(void) {
    CutInst[cutworldix]->rate = 0.5f;
    if (cutmovie == 3 || cutmovie == 4) {
        if (cutworldix == 0) {
            CutInst[cutworldix]->rate *= D_0062E888;
        }
    }
    CutInst[cutworldix]->rate *= D_0062E88C;
    if (D_00632BD8 != 0.0f) {
        CutInst[cutworldix]->rate = D_00632BD8;
    }
}


void CloseCutMovie(s32 all) {
    s32 i;

    for (i = 0; i < 0x20; i++) {
        if (CutInst[i] != 0) {
            instNuGCutSceneDestroy(CutInst[i]);
            CutInst[i] = 0;
        }
    }
    if (D_0063338C != 0) {
        for (i = 0; i < D_0063338C; i++) {
            NuHGobjDestroy(D_006FF880[i].obj);
        }
    }
    D_0063338C = 0;
    for (i = 0; i < 0x20; i++) {
        world_scene[i] = 0;
    }
    NuSoundUpdate();
    NuSoundStopStream(0);
    NuSoundStopStream(1);
    NuSoundStopStream(2);
    NuSoundStopStream(3);
    NuSoundStopStream(4);
    NuSoundUpdate();
    NuSoundFlushLoops();
    if (all != 0) {
        NuTexInit();
        NuMtlRelease();
        NuGobjInit();
    }
}

/* Start the loaded cutscene. On the title screen (logos already played) it
 * loops from TITLESTARTTIME seconds before the end with title music; otherwise
 * its audio stream is started, offset by the selected language for movies. */
/* Near-match (state=asm), 336/344 bytes: the only difference is the register
 * the post-NuSoundStopStream reload of `cutmovie` lands in ($v0 vs retail
 * $v1). Tried: operand order, ternary, local, statement order, condition
 * forms, Game.language vs D_0058B117[0] -- all identical output. */
void StartCutMovie(void) {
    s32 sfx;

    cutmovie_hold = 0;
    if (CutScene[0] != 0 && CutInst[0] != 0) {
        instNuGCutSceneStart(CutInst[0]);
        if (cutmovie == 0 && logos_played != 0) {
            GameMusic(0xaa, 0);
            sfx = -1;
            CutInst[cutworldix]->cframe =
                (CutScene[cutworldix]->nframes - TITLESTARTTIME * TITLEFPS) + 0.0f;
            texanimbits = (texanimbits & ~2) | 4;
        } else {
            sfx = CutAudio[cutworldix];
            texanimbits = (texanimbits | 2) & ~4;
        }
        if (sfx != -1 && (cutmovie != 0 || logos_played == 0)) {
            NuSoundStopStream(4);
            if (cutmovie != 0) {
                sfx += Game.language * 4;
            }
            gamesfx_channel = 4;
            GameSfx(sfx, 0);
            cutmovie_hold = 1;
        }
    }
}

void InitGameCut(void) {
    cutang_FRONTEND[0] = (u16)NuAtan2D(cutpos_CRASH.x - cutpos_FRONTEND[0].x,
                                       cutpos_CRASH.z - cutpos_FRONTEND[0].z);
    cutang_FRONTEND[1] = (u16)NuAtan2D(cutpos_CRASH.x - cutpos_FRONTEND[1].x,
                                       cutpos_CRASH.z - cutpos_FRONTEND[1].z);
    cutang_SPACE[0] = (u16)NuAtan2D(campos_SPACE.x - cutpos_SPACE[0].x,
                                    campos_SPACE.z - cutpos_SPACE[0].z);
    cutang_SPACE[1] = (u16)NuAtan2D(campos_SPACE.x - cutpos_SPACE[1].x,
                                    campos_SPACE.z - cutpos_SPACE[1].z);
    cutang_SPACE[2] = (u16)NuAtan2D(campos_SPACE.x - cutpos_SPACE[2].x,
                                    campos_SPACE.z - cutpos_SPACE[2].z);
    pCutAnim = (struct pcutanm_s *)SpaceGameCutTab[gamecut][1];
    ResetAnimPacket(CutAnim, pCutAnim->action);
    gamecut_hold = 1;
    gamecut_sfx = -1;
    gamecut_finished = 0;
    ResetAnimPacket(CutVortexAnim, 0x22);
}

extern s32 GameMode;
extern f32 HUBHOLOSCALE;
extern f32 SPACEMASKADJUSTY;
extern f32 FRONTENDCORTEXADJUSTY;
extern void NuMtxSetTranslation(struct numtx_s *m, struct nuvec_s *v);
extern void NuMtxSetScale(struct numtx_s *m, struct nuvec_s *v);
extern void NuMtxRotateY(struct numtx_s *m, s32 a);
extern void NuMtxTranslate(struct numtx_s *m, struct nuvec_s *v);

/* Draw the character of the current hub/space cutscene, plus the vortex in
 * space (Level 0x28). Unlike the GameCube build, the bone matrices live in a
 * stack array here rather than in a global. */
/* Near-match (state=asm): instruction-for-instruction except register choice
 * inside the Level 0x28 / frontend branches (retail reloads pCutAnim per arm,
 * into $a1/$a0; here gcc loads it once). Tried: statement order in both arms,
 * direct pCutAnim->i indexing, single-expression yrot, local decl order. */
void DrawGameCut(void) {
    struct numtx_s m;
    struct numtx_s tmtx[256];
    struct nuvec_s scale;
    struct nuvec_s pos;
    float **dwa;
    struct CharacterModel *model;
    u16 yrot;
    s32 i;

    if (Level == 0x28 && CRemap[183] != -1) {
        model = &CModel[CRemap[183]];
        NuMtxSetTranslation(&m, &campos_SPACE);
        EvalModelAnim(model, (struct anim_s *)CutVortexAnim, &m, tmtx, &dwa, 0);
        NuHGobjRndrMtxDwa(model->hobj, &m, 1, 0, tmtx, dwa);
    }
    if (GameMode != 1 || gamecut_finished != 0) {
        return;
    }
    if (CRemap[pCutAnim->character] != -1) {
        model = &CModel[CRemap[pCutAnim->character]];
    } else {
        model = 0;
    }
    if (model == 0) {
        return;
    }

    scale.x = (model->character == 0xB3 || model->character == 0xB4) ? 0.77f : 1.0f;
    if (Level == 0x25) {
        scale.x *= HUBHOLOSCALE;
    }
    scale.y = scale.z = scale.x;
    NuMtxSetScale(&m, &scale);

    if (Level == 0x28) {
        i = pCutAnim->i;
        pos = cutpos_SPACE[i];
        yrot = cutang_SPACE[i];
        yrot -= 0x8000;
        if (model->character == 0x55 || model->character == 0x57 ||
            model->character == 0x56 || model->character == 0x58 ||
            model->character == 2) {
            pos.y += SPACEMASKADJUSTY * scale.y;
        }
    } else {
        i = pCutAnim->i;
        yrot = cutang_FRONTEND[i];
        yrot -= 0x8000;
        pos = cutpos_FRONTEND[i];
        if (model->character == 0xB5) {
            pos.y += FRONTENDCORTEXADJUSTY * scale.y;
        }
    }
    NuMtxRotateY(&m, yrot);
    NuMtxTranslate(&m, &pos);
    EvalModelAnim(model, (struct anim_s *)CutAnim, &m, tmtx, &dwa, 0);
    NuHGobjRndrMtxDwa(model->hobj, &m, 1, 0, tmtx, dwa);
}

void InitCutScenes(void) {
    NuGCutSceneSysInit(D_0060B398);
    NuSetCutSceneFindCharactersFn((void *)AppCutSceneFindCharacters);
    NuSetCutSceneCharacterRenderFn((void *)AppCutSceneCharacterRender);
}

void UpdateCutMovie(void) {
    if (cutmovie_hold != 0 && CutAudio[cutworldix] != -1 &&
        NuSoundKeyStatus(4) != 1) {
        CutInst[cutworldix]->rate = 0.0f;
    } else {
        cutmovie_hold = 0;
        SetCutMovieRate();
    }
    NuGCutSceneSysUpdate(Paused);
}


void locatorfn_fadeout(struct instnugcutscene_s *icutscene, void *locatorsys,
                       struct instnugcutlocator_s *ilocator,
                       struct nugcutlocator_s *locator, f32 currf, void *wm) {
    struct nuanimtime_s atime;
    f32 remaining;

    if (locator->anim != 0) {
        NuAnimData2CalcTime(locator->anim, currf, &atime);
        if (NuGCutLocatorIsVisble(locator, currf, &atime, 0) != 0 &&
            ilocator->data == 0) {
            ilocator->data = (void *)0x1;
            remaining = 255.0f / (icutscene->cutscene->nframes - icutscene->cframe);
            fade_rate = remaining < 1.0f ? 1 : (s32)remaining;
        }
    }
}

void CutSceneEndFunction(void *icutscene) {
    if (cutmovie == 0) {
        NewMenu(Cursor, 0, 0, -1);
        if (logos_played == 0) {
            GameMusic(0xaa, 0);
        }
    } else {
        cut_on = 0;
    }
}

void CutSceneNextFunction(void *icutscene) {
    s32 sfx;

    cutworldix++;
    if (CutAudio[cutworldix] != -1) {
        NuSoundStopStream(4);
        do {
            NuSoundUpdate();
            if (NuSoundKeyStatus(4) == 1) {
                sceGsSyncV(0);
            }
        } while (NuSoundKeyStatus(4) == 1);
        sfx = CutAudio[cutworldix];
        if (cutmovie != 0) {
            sfx += D_0058B117[0] * 4;
        }
        gamesfx_channel = 4;
        GameSfx(sfx, 0);
        cutmovie_hold = 1;
    }
    if (fadeval != 0) {
        fade_rate = -10;
    }
}

void CutSceneNextLogoFunction(void *icutscene) {
    D_00633388++;
    NewMenu(Cursor, 0, 0, -1);
    if (logos_played == 0) {
        GameMusic(0xaa, 0);
    }
}


inline void *FindCutChar(char *name) {
    struct csc_s *chr;

    chr = CutChar;
    if (CutChar != 0) {
        for (; chr->path != 0; chr++) {
            if (strcasecmp(name, chr->name) == 0) {
                return chr->obj;
            }
        }
    }
    return 0;
}

typedef void (*NuDebugMsgFn)();
extern NuDebugMsgFn NuDebugMsgProlog(char *file, s32 line);
extern char D_00632BC0[];
extern char D_00629350[];

static void AppCutSceneFindCharacters(struct nugcutscene_s *cutscene) {
    struct nugcutcharsys_s *charSys;
    struct nugcutchar_s *cutchar;
    struct nugcutlocatorsys_s *locator;
    s32 i;

    charSys = cutscene->chars;
    locator = cutscene->locators;
    for (i = 0; i < charSys->nchars; i++) {
        cutchar = &charSys->chars[i];
        cutchar->character = FindCutChar(cutchar->name);
        if (cutchar->character == 0) {
            NuDebugMsgProlog(D_00632BC0, 0x2a1)(D_00629350, cutchar->name);
        }
        if (cutchar->nlocators != 0 && (s32)cutchar->locators < 0xff) {
            cutchar->first_locator = (u8)(s32)cutchar->locators;
            cutchar->locators = &locator->locators[(s32)cutchar->locators];
        } else {
            cutchar->first_locator = -1;
        }
    }
}


void DrawCutMovie(void) {
    NuGCutSceneSysRender();
}

void NewCut(s32 i) {
    gamecut = i;
    Game.cutbits |= 1 << i;
}

void UpdateCutMovieCamera(struct numtx_s *cam) {
    if (set_cutscenecammtx != 0) {
        *cam = cutscenecammtx;
        set_cutscenecammtx = 0;
        *pNuCam = *cam;
        NuCameraSet(pNuCam);
    }
}

void NewGameCutAnim(void) {
    s32 sfx;

    gamecut_hold = 1;
    gamecut_sfx = pCutAnim->sfx;
    if (gamecut_sfx != -1) {
        NuSoundStopStream(4);
        gamesfx_channel = 4;
        if (Level == 0x25) {
            GameSfx(gamecut_sfx + D_0058B117[0] * 12, 0);
        } else {
            GameSfx(gamecut_sfx, 0);
        }
    }
    if (pCutAnim->action == 0xc) {
        sfx = 0x1e;
    } else if (pCutAnim->action == 0xd) {
        sfx = 0x21;
    } else {
        sfx = -1;
    }
    if (sfx != -1) {
        GameSfx(sfx, 0);
    }
}

void MakeDirLightColour(s32 ix, f32 r, f32 g, f32 b) {
    cutdircol[ix].r = r;
    cutdircol[ix].g = g;
    cutdircol[ix].b = b;
}

void MakeLightDirection(s32 ix, f32 x, f32 y, f32 z) {
    cutdir[ix].x = x;
    cutdir[ix].y = y;
    cutdir[ix].z = z;
    NuVecNorm(&cutdir[ix], &cutdir[ix]);
}
