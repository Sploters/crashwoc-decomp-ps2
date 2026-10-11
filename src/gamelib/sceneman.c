/*
 * Unit: gamelib/sceneman
 *
 * Functions:
 *   0x001b0fe8 NuSceneDataLoad
 *   0x001b1410 NuSceneInstanceCreate
 *   0x001b1790 NuSceneInstanceRunScript
 *   0x001b1940 LoadCutSceneInstanceData
 *   0x001b1ca8 LoadCutsceneData
 *   0x001b1f60 LoadCutsceneScript
 *   0x001b2448 NuSceneManagerSysInit
 *   0x001b2450 NuSceneManagerSysUpdate
 *   0x001b24a0 NuSceneInstanceDestroy
 *   0x001b2558 NuSceneInstanceTranslate
 *   0x001b25d0 NuSceneInstanceRotateY
 *   0x001b2648 NuSceneInstanceEnable
 *   0x001b26e0 NuSceneInstanceDisable
 *   0x001b2778 NuSceneInstanceSetRate
 *   0x001b2780 SceneInstanceFindCutsceneInst
 *   0x001b2820 NuSceneManagerSysRender
 *   0x001b2868 NuSceneManagerSetPtrToPlayerMtx
 */


extern int D_0062F808;
extern void *D_0062F80C;

typedef struct NuSceneInstanceRate {
    char unk000[0x18];
    float rate;
} NuSceneInstanceRate;

void NuSceneManagerSysInit(void) {
    D_0062F808 = 0;
}


void NuSceneInstanceSetRate(NuSceneInstanceRate *inst, float rate) {
    inst->rate = rate;
}


void NuSceneManagerSetPtrToPlayerMtx(void *mtx) {
    D_0062F80C = mtx;
}
