/*
 * Unit: gamelib/nubridge
 *
 * Functions:
 *   0x001b2870 NuBridgeCreate
 *   0x001b2ee8 NuBridgeUpdate
 *   0x001b4660 NuBridgeStreamRope
 *   0x001b4c58 NuBridgeDraw
 *   0x001b5028 NuBridgeInit
 *   0x001b5030 NuBridgeOn
 *   0x001b5038 NuBridgeRegisterBaseScene
 *   0x001b5040 NuBridgeAlloc
 *   0x001b5078 NuBridgeLookupInstanceIndex
 *   0x001b50c8 NuBrdigeDrawRope
 */


extern int BridgeFree;
extern void *NuBridgeProc;
extern void *NuBridge_base_scene;

void NuBridgeInit(void) {
    BridgeFree = 0;
}


void NuBridgeOn(void *proc) {
    NuBridgeProc = proc;
}


void NuBridgeRegisterBaseScene(void *scene) {
    NuBridge_base_scene = scene;
}
