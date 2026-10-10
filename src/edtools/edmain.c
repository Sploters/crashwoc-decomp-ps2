/*
 * Unit: edtools/edmain
 *
 * Functions:
 *   0x001b7840 edmainCreateMainMenu
 *   0x001b79f8 edmainSetReturn
 *   0x001b7a00 edmainRegister
 *   0x001b7a50 edmainProcess
 *   0x001b7ac0 edmainRender
 *   0x001b7b10 edmainSetCamera
 *   0x001b7bb0 edmainInit
 *   0x001b7c78 edmainClose
 *   0x001b7cf8 edmainRegisterLocVec
 *   0x001b7d00 edmainQueryLocVec
 *   0x001b7d08 edmainMainMenuProc
 *   0x001b7d30 edmainMainMenuRender
 *   0x001b7d58 edmainReadLevelFile
 *   0x001b7e08 cbMMRegSel
 *   0x001b7e40 cbMMCancel
 *   0x001b7e48 cbMMEditorConfig
 *   0x001b7e88 cbMMReturnToApp
 *   0x001b7e98 cbmcfgCancel
 *   0x001b7eb8 cbmcfgCameraSpeed
 */

typedef signed int s32;
typedef float f32;

struct pad_s;
struct nuvec_s;

typedef struct edmainreg_s {
    struct edmainreg_s *next;
    struct edmainreg_s *prev;
    char *name;
    void (*init)(void);
    void (*close)(void);
    void (*select)(void);
    char pad_0x18[0xC];
    void (*readblock)(s32 fh);
    s32 blockid;
    s32 (*process)(struct pad_s *pad, f32 dt);
    void (*render)(void);
} EDMAINREG;

typedef struct eduiitem_s {
    char pad_0x00[0xC];
    void *data;
    char pad_0x10[0x8];
    s32 x;
    s32 y;
} EDUIITEM;

typedef struct eduimenu_s {
    char pad_0x00[0x10];
    s32 x;
    s32 y;
} EDUIMENU;

extern s32 D_0062F8C8;
extern EDMAINREG *D_0062F8CC;
extern EDMAINREG *D_0062F8D0;
extern s32 D_0062F8D4;
extern EDUIMENU *D_0062F8D8;
extern EDUIMENU *D_0062F8DC;
extern struct nuvec_s *D_0062F8E8;
extern s32 D_00633234;

#define edmain_initialised D_0062F8C8
#define edmain_registry    D_0062F8CC
#define edmain_current     D_0062F8D0
#define edmain_active      D_0062F8D4
#define edmain_mainmenu    D_0062F8D8
#define edmain_cfgmenu     D_0062F8DC
#define edmain_locvec      D_0062F8E8
#define edmain_return      D_00633234

extern void edmainClose(void);
extern void eduiMenuProcess(EDUIMENU *menu, struct pad_s *pad);
extern void eduiMenuRender(EDUIMENU *menu);
extern s32 eduiMenuAttach(EDUIMENU *menu, EDUIMENU *child);
extern void eduiMenuDetach(EDUIMENU *menu);

void edmainMainMenuProc(struct pad_s *pad);


void edmainSetReturn(s32 value) {
    edmain_return = value;
}


s32 edmainRegister(EDMAINREG *reg) {
    if (edmain_initialised) {
        edmainClose();
    }
    if (edmain_registry) {
        edmain_registry->prev = reg;
    }
    reg->prev = 0;
    reg->next = edmain_registry;
    edmain_registry = reg;
    return 1;
}


s32 edmainProcess(struct pad_s *pad, f32 dt) {
    edmain_return = 0;
    if (edmain_active && edmain_current) {
        if (!edmain_current->process || edmain_current->process(pad, dt)) {
            edmain_active = 0;
        }
    } else {
        if (!edmain_current) {
            edmain_current = edmain_registry;
        }
        edmainMainMenuProc(pad);
    }
    return edmain_return;
}


void edmainRegisterLocVec(struct nuvec_s *vec) {
    edmain_locvec = vec;
}


struct nuvec_s *edmainQueryLocVec(void) {
    return edmain_locvec;
}


void edmainMainMenuProc(struct pad_s *pad) {
    if (edmain_mainmenu) {
        eduiMenuProcess(edmain_mainmenu, pad);
    }
}


void edmainMainMenuRender(void) {
    if (edmain_mainmenu) {
        eduiMenuRender(edmain_mainmenu);
    }
}


void cbMMRegSel(EDUIMENU *menu, EDUIITEM *item) {
    EDMAINREG *reg;

    reg = item->data;
    edmain_active = 1;
    edmain_current = reg;
    if (reg->select) {
        reg->select();
    }
}


void cbMMCancel(EDUIMENU *menu, EDUIITEM *item) {
}


void cbMMEditorConfig(EDUIMENU *menu, EDUIITEM *item) {
    edmain_cfgmenu->x = item->x + 10;
    edmain_cfgmenu->y = item->y + 10;
    eduiMenuAttach(menu, edmain_cfgmenu);
}


void cbMMReturnToApp(EDUIMENU *menu, EDUIITEM *item) {
    edmain_return = 1;
}


void cbmcfgCancel(EDUIMENU *menu, EDUIITEM *item) {
    eduiMenuDetach(menu);
}
