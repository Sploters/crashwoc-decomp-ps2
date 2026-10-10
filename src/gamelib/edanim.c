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

struct nuinstance_s {
    char pad00[0x50];
};

struct nuspecial_s {
    char pad00[0x40];
    struct nuinstance_s *instance;
    char pad44[0x50 - 0x44];
};

struct nugscn_s {
    char pad00[0x18];
    int numinstance;
    struct nuinstance_s *instances;
    char pad20[0x24 - 0x20];
    struct nuspecial_s *specials;
};

struct edanimitem_s {
    char pad00[0x0C];
    int value;
    unsigned char toggle;
    char pad11[0x4C - 0x11];
    float fvalue;
};

struct edanimparam_s {
    int id;
    char pad004[0x10 - 0x04];
    int switch_id;
    float switch_var;
    float switch_delay;
    char pad01c[0xBC - 0x1C];
    int particle_rate[8];
    int particle_switch[8];
    char pad0fc[0x240 - 0xFC];
    float sound_timing[8];
    char pad260[0x2D0 - 0x260];
};

extern struct edanimparam_s AnimParams[];

extern struct nugscn_s *edanim_base_scene;
extern void *edanim_active_menu;
extern int edanim_nearest;
extern int edanim_nearest_param_id;
extern int edanim_nearest_particle;
extern int edanim_nearest_sound;
extern int edanim_next_param;
extern int edanim_particle_mode;
extern int edanim_particle_type;
extern int edanim_sound_type;


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


void edanimParamReset(void)
{
    int i;

    for (i = 0; i < 64; i++) {
        AnimParams[i].id = -1;
    }
    edanim_next_param = 0;
}


void edanimParamDestroy(int param)
{
    if (AnimParams[param].id != -1) {
        AnimParams[param].id = -1;
    }
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


void edanimcbSetParticleRate(void *menu, struct edanimitem_s *item)
{
    AnimParams[edanim_nearest_param_id].particle_rate[edanim_nearest_particle] =
        item->fvalue;
}


void edanimcbCancelOptMenu(void)
{
    edanim_active_menu = 0;
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
