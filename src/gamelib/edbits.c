/*
 * Unit: gamelib/edbits
 *
 * Functions:
 *   0x001a6b68 edbitsSoundPlay
 *   0x001a6cd8 edqrand
 *   0x001a6cf8 edbitsRegisterLevel
 *   0x001a6d48 edbitsRegisterSfx
 *   0x001a6d60 edbitsDrawCube
 *   0x001a70c0 edbitsDrawDiagonalCross
 *   0x001a7228 edbitsLookupSoundFX
 *   0x001a7308 edbitsGetSoundName
 *   0x001a7340 edbitsDrawCircleXY
 *   0x001a7468 edbitsLookupInstance
 *   0x001a74f0 edbitsSetSoundFxVolume
 *   0x001a74f8 edbitsRegisterBaseScene
 */


extern int edqseed;
extern void *edSfxGlobalTab;
extern void *edSfxLevelTab;
extern int edSfxGlobalCount;
extern int edSfxAllCount;
extern int edbitsSfxVol;
extern void *edbits_base_scene;

int edqrand(void) {
    edqseed = (edqseed * 0x24CD + 1) & 0xFFFF;
    return edqseed;
}


void edbitsRegisterSfx(void *global_tab, void *level_tab, int global_count, int all_count) {
    edSfxGlobalTab = global_tab;
    edSfxLevelTab = level_tab;
    edSfxGlobalCount = global_count;
    edSfxAllCount = all_count;
}


void edbitsSetSoundFxVolume(int vol) {
    edbitsSfxVol = vol;
}


void edbitsRegisterBaseScene(void *scene) {
    edbits_base_scene = scene;
}
