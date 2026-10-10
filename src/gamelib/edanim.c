/*
 * Unit: gamelib/edanim
 *
 * Functions:
 *   0x001a1198 edanimUpdateObjects
 *   0x001a1ce8 edanimDetermineNearestAnim
 *   0x001a1e90 edanimDetermineNearestParticle
 *   0x001a2088 edanimDetermineNearestSound
 *   0x001a2280 edanimParamCreate
 *   0x001a2398 edanimSoundCreate
 *   0x001a24c8 edanimSoundDestroy
 *   0x001a2610 edanimParticleCreate
 *   0x001a2750 edanimParticleDestroy
 *   0x001a28a0 edanimDoInput
 *   0x001a2fa0 edanimFileSave
 *   0x001a32a8 edanimFileLoad
 *   0x001a3810 edanimDrawCursor
 *   0x001a3c90 edanimcbSwitchTypeMenu
 *   0x001a41b0 edanimcbSwitchMenu
 *   0x001a4398 edanimcbBouncyMenu
 *   0x001a4548 edanimcbParticleTypeMenu
 *   0x001a46c8 edanimcbLocalParticleMenu
 *   0x001a4858 edanimcbLocalSoundMenu
 *   0x001a4ab0 edanimcbMCTBMenu
 *   0x001a4dd8 edanimInit
 *   0x001a50c8 edanimRenderParticleEmitters
 *   0x001a5260 edanimRenderSoundEmitters
 *   0x001a5390 edanimRender
 *   0x001a5490 edanimLookupInstanceIndex
 *   0x001a54f0 edanimRegisterBaseScene
 *   0x001a54f8 edanimLookupSpecial
 *   0x001a5580 edanimParamReset
 *   0x001a55b0 edanimPlayerAnimDistance
 *   0x001a5628 edanimParamDestroy
 *   0x001a5658 edanimSoundPlace
 *   0x001a56b0 edanimParticlePlace
 *   0x001a5750 edanimcbSetSwitchType
 *   0x001a57b0 edanimcbSetSwitchId
 *   0x001a57f0 edanimcbSetSwitchDelay
 *   0x001a5828 edanimcbSetSwitchVar
 *   0x001a5860 edanimcbCancelSwitchTypeMenu
 *   0x001a5888 edanimcbCancelSwitchMenu
 *   0x001a58b0 edanimcbSetBouncyPlayerGrav
 *   0x001a5910 edanimcbSetBouncyTension
 *   0x001a5970 edanimcbSetBouncyDamping
 *   0x001a59d0 edanimcbCancelBouncyMenu
 *   0x001a59f8 edanimcbToggleSoundType
 *   0x001a5ab0 edanimcbSetSoundTiming
 *   0x001a5ae0 edanimcbToggleParticleSwitch
 *   0x001a5b20 edanimcbSetParticleType
 *   0x001a5b70 edanimcbSetParticleRate
 *   0x001a5ba8 edanimcbCancelParticleTypeMenu
 *   0x001a5bd0 edanimcbLocalParticleTypeMenu
 *   0x001a5d48 edanimcbCancelLocalParticleMenu
 *   0x001a5d70 edanimcbParticleMenu
 *   0x001a5e90 edanimcbLocalSoundTypeMenu
 *   0x001a6008 edanimcbCancelLocalSoundMenu
 *   0x001a6030 edanimcbSoundMenu
 *   0x001a6150 edanimcbCancelOptMenu
 *   0x001a6158 edanimcbFileSave
 *   0x001a6230 edanimcbFileLoad
 *   0x001a62d8 edanimcbMCTBCardPresent
 *   0x001a6330 edanimcbMCTBCardType
 *   0x001a6388 edanimcbMCTBCardCheckFormat
 *   0x001a63e0 edanimcbMCTBCardFreeSpace
 *   0x001a6430 edanimcbMCTBCardSlotsUsed
 *   0x001a6478 edanimcbMCTBCardSaveSlot
 *   0x001a6500 edanimcbMCTBCardLoadSlot
 *   0x001a6560 edanimcbMCTBCardDeleteSlot
 *   0x001a65c0 edanimcbMCTBCardUnFormat
 *   0x001a6618 edanimcbMCTBCardFormat
 *   0x001a6670 edanimcbCancelMCTBMenu
 *   0x001a6698 edanimClose
 *   0x001a66b8 edanimEnter
 *   0x001a66d8 edanimProc
 *   0x001a6790 edanimcbSetLocalSoundType
 *   0x001a6828 edanimcbSetLocalParticleType
 *   0x001a68c0 edanimcbCancelLocalParticleTypeMenu
 *   0x001a68e8 edanimcbCancelParticleMenu
 *   0x001a6910 edanimcbSoundTypeMenu
 *   0x001a6a90 edanimcbCancelLocalSoundTypeMenu
 *   0x001a6ab8 edanimcbCancelSoundMenu
 *   0x001a6ae0 edanimcbSetSoundType
 *   0x001a6b40 edanimcbCancelSoundTypeMenu
 */

struct nuvec_s {
    float x;
    float y;
    float z;
};

struct nuinstance_s {
    float mtx[4][4];
    char pad40[0x50 - 0x40];
};

struct nuspecial_s {
    float rot[3][4];
    struct nuvec_s pos;
    float pos_w;
    struct nuinstance_s *instance;
    char *name;
    char pad48[0x50 - 0x48];
};

struct nugscn_s {
    char pad00[0x18];
    int numinstance;
    struct nuinstance_s *instances;
    int numspecial;
    struct nuspecial_s *specials;
};

struct edanimitem_s {
    char pad00[0x0C];
    int value;
    unsigned char toggle;
    char pad11[0x4C - 0x11];
    float fvalue;
};

struct pad_s {
    char pad000[0x564];
    unsigned int buttons;
};

struct edanimparam_s {
    int id;
    char pad004[0x0C - 0x04];
    int switch_type;
    int switch_id;
    float switch_var;
    float switch_delay;
    char pad01c[0xBC - 0x1C];
    int particle_rate[8];
    int particle_switch[8];
    struct nuvec_s particle_pos[8];
    unsigned short particle_rotz[8];
    unsigned short particle_roty[8];
    char pad17c[0x180 - 0x17C];
    char sound_name[8][16];
    int sound_type[8];
    int sound_switch[8];
    float sound_timing[8];
    struct nuvec_s sound_pos[8];
    int bouncy_instance;
    float bouncy_player_grav;
    float bouncy_tension;
    float bouncy_damping;
};

extern struct edanimparam_s AnimParams[];

extern struct nugscn_s *edanim_base_scene;
extern void *edanim_active_menu;
extern void *edanim_bouncy_menu;
extern void *edanim_localparticle_menu;
extern void *edanim_localparticletype_menu;
extern void *edanim_localsound_menu;
extern void *edanim_localsoundtype_menu;
extern void *edanim_mctb_menu;
extern void *edanim_options_menu;
extern void *edanim_particle_menu;
extern void *edanim_particletype_menu;
extern void *edanim_sound_menu;
extern void *edanim_switch_menu;
extern void *edanim_switchtype_menu;
extern int edanim_nearest;
extern int edanim_nearest_param_id;
extern int edanim_nearest_particle;
extern int edanim_nearest_sound;
extern int edanim_next_param;
extern int edanim_particle_mode;
extern int edanim_particle_type;
extern int edanim_sound_type;
extern int edanim_sound_mode;
extern unsigned short edanim_emitrotz;
extern unsigned short edanim_emitroty;
extern void *edanim_mtl_zoff;
extern void *edanim_soundtype_menu;

extern char D_0062F6E0[];
extern char D_006196B0[];
extern char D_006196C0[];
extern char D_006196D0[];
extern char D_006196E0[];
extern char D_006196F0[];
extern char D_00619700[];

#define mctb_text_present D_0062F6E0
#define mctb_text_not_present D_006196B0
#define mctb_text_ps2_card D_006196C0
#define mctb_text_not_ps2_card D_006196D0
#define mctb_text_formatted D_006196E0
#define mctb_text_not_formatted D_006196F0
#define mctb_text_free_space D_00619700

extern char D_00619710[];
extern char D_00619748[];
extern char D_00619758[];
extern char D_00619768[];
extern char D_00619778[];
extern char D_00619788[];
extern char D_00619798[];
extern char D_006197A8[];

#define mctb_text_slots_used D_00619710
#define mctb_text_load_failed D_00619748
#define mctb_text_deleted D_00619758
#define mctb_text_delete_failed D_00619768
#define mctb_text_unformatted D_00619778
#define mctb_text_unformat_failed D_00619788
#define mctb_text_formatted_ok D_00619798
#define mctb_text_format_failed D_006197A8

extern void eduiMenuDestroy(void *menu);
extern void eduiMenuDetach(void *menu);
extern void eduiCreateMessageMenu(void *menu, char *text, int flag);
extern struct nuvec_s *edmainQueryLocVec(void);
extern float NuVecDist(struct nuvec_s *a, struct nuvec_s *b, void *c);
extern void NuVecSub(struct nuvec_s *dst, struct nuvec_s *a, struct nuvec_s *b);
extern void PlatInstBounce(int instance, float player_grav, float tension, float damping);
extern int saveloadCheckCardPresent(void);
extern int saveloadCheckCardType(void);
extern int saveloadCheckCardFormatted(void);
extern int saveloadCheckCardFreeSpace(int port);
extern int sprintf(char *buf, const char *fmt, ...);
extern int saveloadCheckSlotsUsed(void);
extern int saveloadLoadSlot(int slot, char *text, int size);
extern int saveloadDeleteSlot(int slot);
extern int saveloadUnFormatCard(void);
extern int saveloadFormatCard(void);
extern int strncmp(const char *a, const char *b, int n);
extern char *strcpy(char *dst, const char *src);
extern char *edbitsGetSoundName(int type);
extern void edbitsDrawCube(float x, float y, float z, float sizex, float sizey, float sizez,
                           int rotx, int roty, int rotz, int flags, int colour, void *mtl);
extern void edcamSet(void);
extern void NuFntScale(int x, int y);
extern void eduiMenuRender(void *menu);
extern void eduiMenuProcess(void *menu, struct pad_s *pad);
extern void edanimDrawCursor(void);
extern void edanimRenderParticleEmitters(int param);
extern void edanimRenderSoundEmitters(int param);
extern void edanimDoInput(struct pad_s *pad);
extern void edanimDetermineNearestAnim(float range);
extern void edanimDetermineNearestParticle(float range);
extern void edanimDetermineNearestSound(float range);


void edanimRender(void)
{
    int colour;
    struct nuinstance_s *instance;

    edcamSet();
    edanimDrawCursor();
    if (edanim_nearest != -1) {
        if (edanim_particle_mode) {
            colour = 0xFF00FF00;
        } else if (edanim_sound_mode) {
            colour = 0xFFFF0000;
        } else if (edanim_nearest_param_id != -1) {
            colour = 0xFFFFFFFF;
        } else {
            colour = 0xFF808080;
        }
        instance = edanim_base_scene->specials[edanim_nearest].instance;
        edbitsDrawCube(instance->mtx[3][0], instance->mtx[3][1], instance->mtx[3][2],
                       0.5f, 0.5f, 0.5f, 0, 0, 0, 0, colour, edanim_mtl_zoff);
        if (edanim_particle_mode) {
            edanimRenderParticleEmitters(edanim_nearest_param_id);
        }
        if (edanim_sound_mode) {
            edanimRenderSoundEmitters(edanim_nearest_param_id);
        }
    }
    if (edanim_active_menu != 0) {
        NuFntScale(12, 12);
        eduiMenuRender(edanim_active_menu);
    }
}


int edanimLookupInstanceIndex(int special)
{
    int i;

    if (edanim_base_scene == 0) {
        return -1;
    }
    for (i = 0; i < edanim_base_scene->numinstance; i++) {
        if (&edanim_base_scene->instances[i]
            == edanim_base_scene->specials[special].instance) {
            return i;
        }
    }
    return -1;
}


void edanimRegisterBaseScene(struct nugscn_s *scene)
{
    edanim_base_scene = scene;
}


int edanimLookupSpecial(char *name)
{
    int i;

    if (edanim_base_scene != 0) {
        for (i = 0; i < edanim_base_scene->numspecial; i++) {
            if (strncmp(edanim_base_scene->specials[i].name, name, 19) == 0) {
                return i;
            }
        }
    }
    return -1;
}


void edanimParamReset(void)
{
    int i;

    for (i = 0; i < 64; i++) {
        AnimParams[i].id = -1;
    }
    edanim_next_param = 0;
}


float edanimPlayerAnimDistance(int param)
{
    if (edmainQueryLocVec() != 0) {
        return NuVecDist(&edanim_base_scene->specials[AnimParams[param].id].pos,
                         edmainQueryLocVec(), 0);
    }
    return 0.0f;
}


void edanimParamDestroy(int param)
{
    if (AnimParams[param].id != -1) {
        AnimParams[param].id = -1;
    }
}


void edanimSoundPlace(int slot, struct nuvec_s *wpos)
{
    struct nuvec_s *origin = &edanim_base_scene->specials[edanim_nearest].pos;

    NuVecSub(&AnimParams[edanim_nearest_param_id].sound_pos[slot], wpos, origin);
}


void edanimParticlePlace(int slot, struct nuvec_s *wpos)
{
    struct nuvec_s *origin = &edanim_base_scene->specials[edanim_nearest].pos;

    NuVecSub(&AnimParams[edanim_nearest_param_id].particle_pos[slot], wpos, origin);
    AnimParams[edanim_nearest_param_id].particle_rotz[slot] = edanim_emitrotz;
    AnimParams[edanim_nearest_param_id].particle_roty[slot] = edanim_emitroty;
}


void edanimcbSetSwitchType(void *menu, struct edanimitem_s *item)
{
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
    edanim_switchtype_menu = 0;
    AnimParams[edanim_nearest_param_id].switch_type = item->value;
}


void edanimcbSetSwitchId(void *menu, struct edanimitem_s *item)
{
    if (edanim_nearest != -1 && edanim_nearest_param_id != -1) {
        AnimParams[edanim_nearest_param_id].switch_id = item->fvalue;
    }
}


void edanimcbSetSwitchDelay(void *menu, struct edanimitem_s *item)
{
    if (edanim_nearest != -1 && edanim_nearest_param_id != -1) {
        AnimParams[edanim_nearest_param_id].switch_delay = item->fvalue;
    }
}


void edanimcbSetSwitchVar(void *menu, struct edanimitem_s *item)
{
    if (edanim_nearest != -1 && edanim_nearest_param_id != -1) {
        AnimParams[edanim_nearest_param_id].switch_var = item->fvalue;
    }
}


void edanimcbCancelSwitchTypeMenu(void)
{
    eduiMenuDestroy(edanim_switchtype_menu);
    edanim_switchtype_menu = 0;
}


void edanimcbCancelSwitchMenu(void)
{
    eduiMenuDestroy(edanim_switch_menu);
    edanim_switch_menu = 0;
}


void edanimcbSetBouncyPlayerGrav(void *menu, struct edanimitem_s *item)
{
    int id;

    if (edanim_nearest != -1) {
        id = edanim_nearest_param_id;
        AnimParams[id].bouncy_player_grav = item->fvalue;
        if (AnimParams[id].bouncy_instance != -1) {
            PlatInstBounce(AnimParams[id].bouncy_instance,
                           AnimParams[id].bouncy_player_grav,
                           AnimParams[id].bouncy_tension,
                           AnimParams[id].bouncy_damping);
        }
    }
}


void edanimcbSetBouncyTension(void *menu, struct edanimitem_s *item)
{
    int id;

    if (edanim_nearest != -1) {
        id = edanim_nearest_param_id;
        AnimParams[id].bouncy_tension = item->fvalue;
        if (AnimParams[id].bouncy_instance != -1) {
            PlatInstBounce(AnimParams[id].bouncy_instance,
                           AnimParams[id].bouncy_player_grav,
                           AnimParams[id].bouncy_tension,
                           AnimParams[id].bouncy_damping);
        }
    }
}


void edanimcbSetBouncyDamping(void *menu, struct edanimitem_s *item)
{
    int id;

    if (edanim_nearest != -1) {
        id = edanim_nearest_param_id;
        AnimParams[id].bouncy_damping = item->fvalue;
        if (AnimParams[id].bouncy_instance != -1) {
            PlatInstBounce(AnimParams[id].bouncy_instance,
                           AnimParams[id].bouncy_player_grav,
                           AnimParams[id].bouncy_tension,
                           AnimParams[id].bouncy_damping);
        }
    }
}


void edanimcbCancelBouncyMenu(void)
{
    eduiMenuDestroy(edanim_bouncy_menu);
    edanim_bouncy_menu = 0;
}


void edanimcbToggleSoundType(void *menu, struct edanimitem_s *item)
{
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
    edanim_soundtype_menu = 0;
    if (item->toggle == 0) {
        AnimParams[edanim_nearest_param_id].sound_switch[edanim_nearest_sound] = 0;
        AnimParams[edanim_nearest_param_id].sound_timing[edanim_nearest_sound] = 1.0f;
    } else {
        AnimParams[edanim_nearest_param_id].sound_switch[edanim_nearest_sound] = 1;
        AnimParams[edanim_nearest_param_id].sound_timing[edanim_nearest_sound] = 50.0f;
    }
}


void edanimcbSetSoundTiming(void *menu, struct edanimitem_s *item)
{
    AnimParams[edanim_nearest_param_id].sound_timing[edanim_nearest_sound] = item->fvalue;
}


void edanimcbToggleParticleSwitch(void *menu, struct edanimitem_s *item)
{
    if (edanim_nearest_param_id != -1 && edanim_nearest_particle != -1) {
        AnimParams[edanim_nearest_param_id].particle_switch[edanim_nearest_particle] =
            item->toggle;
    }
}


void edanimcbSetParticleType(void *menu, struct edanimitem_s *item)
{
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
    edanim_particletype_menu = 0;
    if (item->value == 0) {
        edanim_particle_type = -1;
    } else {
        edanim_particle_type = item->value;
    }
}


void edanimcbSetParticleRate(void *menu, struct edanimitem_s *item)
{
    AnimParams[edanim_nearest_param_id].particle_rate[edanim_nearest_particle] =
        item->fvalue;
}


void edanimcbCancelParticleTypeMenu(void)
{
    eduiMenuDestroy(edanim_particletype_menu);
    edanim_particletype_menu = 0;
}


void edanimcbCancelLocalParticleMenu(void)
{
    eduiMenuDestroy(edanim_localparticle_menu);
    edanim_localparticle_menu = 0;
}


void edanimcbCancelLocalSoundMenu(void)
{
    eduiMenuDestroy(edanim_localsound_menu);
    edanim_localsound_menu = 0;
}


void edanimcbCancelOptMenu(void)
{
    edanim_active_menu = 0;
}


void edanimcbMCTBCardPresent(void *menu)
{
    if (saveloadCheckCardPresent()) {
        eduiCreateMessageMenu(menu, mctb_text_present, 1);
    } else {
        eduiCreateMessageMenu(menu, mctb_text_not_present, 0);
    }
}


void edanimcbMCTBCardType(void *menu)
{
    if (saveloadCheckCardType()) {
        eduiCreateMessageMenu(menu, mctb_text_ps2_card, 1);
    } else {
        eduiCreateMessageMenu(menu, mctb_text_not_ps2_card, 0);
    }
}


void edanimcbMCTBCardCheckFormat(void *menu)
{
    if (saveloadCheckCardFormatted()) {
        eduiCreateMessageMenu(menu, mctb_text_formatted, 1);
    } else {
        eduiCreateMessageMenu(menu, mctb_text_not_formatted, 0);
    }
}


void edanimcbMCTBCardFreeSpace(void *menu)
{
    char text[32];
    int space;

    space = saveloadCheckCardFreeSpace(0);
    sprintf(text, mctb_text_free_space, space);
    eduiCreateMessageMenu(menu, text, 1);
}


void edanimcbMCTBCardSlotsUsed(void *menu)
{
    char text[32];
    int used;

    used = saveloadCheckSlotsUsed();
    sprintf(text, mctb_text_slots_used, used);
    eduiCreateMessageMenu(menu, text, 1);
}


void edanimcbMCTBCardLoadSlot(void *menu, struct edanimitem_s *item)
{
    char text[32];

    if (saveloadLoadSlot(item->value, text, 32)) {
        eduiCreateMessageMenu(menu, text, 1);
    } else {
        eduiCreateMessageMenu(menu, mctb_text_load_failed, 0);
    }
}


void edanimcbMCTBCardDeleteSlot(void *menu, struct edanimitem_s *item)
{
    if (saveloadDeleteSlot(item->value)) {
        eduiCreateMessageMenu(menu, mctb_text_deleted, 1);
    } else {
        eduiCreateMessageMenu(menu, mctb_text_delete_failed, 0);
    }
}


void edanimcbMCTBCardUnFormat(void *menu)
{
    if (saveloadUnFormatCard()) {
        eduiCreateMessageMenu(menu, mctb_text_unformatted, 1);
    } else {
        eduiCreateMessageMenu(menu, mctb_text_unformat_failed, 0);
    }
}


void edanimcbMCTBCardFormat(void *menu)
{
    if (saveloadFormatCard()) {
        eduiCreateMessageMenu(menu, mctb_text_formatted_ok, 1);
    } else {
        eduiCreateMessageMenu(menu, mctb_text_format_failed, 0);
    }
}


void edanimcbCancelMCTBMenu(void)
{
    eduiMenuDestroy(edanim_mctb_menu);
    edanim_mctb_menu = 0;
}


void edanimClose(void)
{
    eduiMenuDestroy(edanim_options_menu);
}


void edanimEnter(void)
{
    edanim_nearest = -1;
    edanim_nearest_param_id = -1;
    edanim_nearest_particle = -1;
    edanim_particle_mode = 0;
    edanim_particle_type = -1;
    edanim_sound_type = -1;
}


int edanimProc(struct pad_s *pad)
{
    if (edanim_active_menu != 0) {
        eduiMenuProcess(edanim_active_menu, pad);
        return 0;
    }
    edanimDoInput(pad);
    if (edanim_particle_mode) {
        edanimDetermineNearestParticle(1.0f);
    } else if (edanim_sound_mode) {
        edanimDetermineNearestSound(1.0f);
    } else {
        edanimDetermineNearestAnim(1.0f);
    }
    if (pad->buttons & 0x800) {
        edanim_particle_mode = 0;
        return 1;
    }
    return 0;
}


void edanimcbSetLocalSoundType(void *menu, struct edanimitem_s *item)
{
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
    edanim_localsoundtype_menu = 0;
    AnimParams[edanim_nearest_param_id].sound_type[edanim_nearest_sound] = item->value;
    strcpy(AnimParams[edanim_nearest_param_id].sound_name[edanim_nearest_sound],
           edbitsGetSoundName(item->value));
    edanim_sound_type = item->value;
}


void edanimcbCancelLocalParticleTypeMenu(void)
{
    eduiMenuDestroy(edanim_localparticletype_menu);
    edanim_localparticletype_menu = 0;
}


void edanimcbCancelParticleMenu(void)
{
    eduiMenuDestroy(edanim_particle_menu);
    edanim_particle_menu = 0;
}


void edanimcbCancelLocalSoundTypeMenu(void)
{
    eduiMenuDestroy(edanim_localsoundtype_menu);
    edanim_localsoundtype_menu = 0;
}


void edanimcbCancelSoundMenu(void)
{
    eduiMenuDestroy(edanim_sound_menu);
    edanim_sound_menu = 0;
}


void edanimcbSetSoundType(void *menu, struct edanimitem_s *item)
{
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
    edanim_soundtype_menu = 0;
    if (item->value == 99999) {
        edanim_sound_type = -1;
    } else {
        edanim_sound_type = item->value;
    }
}


void edanimcbCancelSoundTypeMenu(void)
{
    eduiMenuDestroy(edanim_soundtype_menu);
    edanim_soundtype_menu = 0;
}
