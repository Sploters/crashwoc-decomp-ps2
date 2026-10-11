/*
 * Unit: nu3d/nulight
 *
 * Functions:
 *   0x00132090 NuLightMatInit
 *   0x00135160 NuLightInit
 *   0x001357e0 NuLightUpdate
 *   0x00135948 NuLightFxSetup
 *   0x00135cb8 NuLightProcessSpot
 *   0x00139788 NuLightHaze
 *   0x0013af40 NuLightPolyHaze
 *   0x0013cbe8 NuLightFogDraw
 *   0x0013f268 NuLigthSetModeMask
 *   0x0013f408 NuLgtArcLaserDraw
 *   0x0013ff70 NuLightClose
 *   0x0013ff78 NuLightCreate
 *   0x0013ffc0 NuLightDestroy
 *   0x00140018 NuLightGetLight
 *   0x00140030 NuLigthSetPolyHazeMat
 *   0x00140088 NuLightAddSpot
 *   0x00140108 NuLightAddSpotXSpanFade
 *   0x00140190 NuLightFog
 *   0x001401b0 NuLightFogClear
 *   0x001401b8 NuLgtArcLaser
 *   0x001402c8 NuLgtSetArcMat
 *   0x001402e0 NuLightAddHazePoly
 *   0x00140340 NuLightSprDmaOpen
 *   0x001403d0 NuLightSprDmaClose
 *   0x00140400 NuLightSprMake
 *   0x001404e8 NuLgtRand
 */


extern int NuRndrFogClear;
extern unsigned char D_006AD100[];
extern float NuRndrFogNear;
extern float NuRndrFogFar;
extern int NuRndrFogCol;
extern int NuRndrFogBlur;
extern int HazeColour;
extern void *NuLgtArcMtl;
extern float NuLgtArcU0;
extern float NuLgtArcV0;
extern float NuLgtArcU1;
extern float NuLgtArcV1;
extern int NuLgtSeed;


void NuLightClose(void) {
}


void *NuLightGetLight(int index) {
    return &D_006AD100[index * 0x90];
}


void NuLightFog(float near, float far, int colour, int blur, int haze_colour) {
    NuRndrFogNear = near;
    NuRndrFogFar = far;
    NuRndrFogCol = colour;
    NuRndrFogBlur = blur << 24;
    HazeColour = haze_colour;
}


void NuLightFogClear(int clear) {
    NuRndrFogClear = clear;
}


void NuLgtSetArcMat(void *material, float u0, float v0, float u1, float v1) {
    NuLgtArcMtl = material;
    NuLgtArcU0 = u0;
    NuLgtArcV0 = v0;
    NuLgtArcU1 = u1;
    NuLgtArcV1 = v1;
}


int NuLgtRand(void) {
    int seed = (NuLgtSeed * 0x24CD + 1) & 0xFFFF;

    NuLgtSeed = seed;
    return seed;
}


#ifndef NULL
#define NULL ((void *)0)
#endif
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
s32 vpDmaTag_Close(s32);
s32 vtCloseDirect(s32);
extern s32 NuLgtBuff;

void NuLightSprDmaClose(void) {
    s32 temp_v0;

    temp_v0 = vtCloseDirect(NuLgtBuff);
    NuLgtBuff = temp_v0;
    NuLgtBuff = vpDmaTag_Close(temp_v0);
}



extern s32 NuLightHazePolymtl;
extern s32 NuLightHazePolyus[];
extern s32 NuLightHazePolyvs[];

void NuLigthSetPolyHazeMat(struct numtl_s * a_0, struct hazecol_s * a_1, struct hazecol_s * a_2)
{
    s32 arg0 = a_0;
    void *arg1 = a_1;
    void *arg2 = a_2;

    NuLightHazePolymtl = arg0;
    (*( f32 *)((s8 *)(&NuLightHazePolyus[0]) + ( 0))) = (f32) (*( f32 *)((s8 *)(arg1) + ( 0)));
    (*( f32 *)((s8 *)(&NuLightHazePolyus[0]) + ( 4))) = (f32) (*( f32 *)((s8 *)(arg1) + ( 4)));
    (*( f32 *)((s8 *)(&NuLightHazePolyus[0]) + ( 8))) = (f32) (*( f32 *)((s8 *)(arg1) + ( 0)));
    (*( f32 *)((s8 *)(&NuLightHazePolyus[0]) + ( 0xC))) = (f32) (*( f32 *)((s8 *)(arg1) + ( 4)));
    (*( f32 *)((s8 *)(&NuLightHazePolyvs[0]) + ( 0))) = (f32) (*( f32 *)((s8 *)(arg2) + ( 0)));
    (*( f32 *)((s8 *)(&NuLightHazePolyvs[0]) + ( 4))) = (f32) (*( f32 *)((s8 *)(arg2) + ( 0)));
    (*( f32 *)((s8 *)(&NuLightHazePolyvs[0]) + ( 8))) = (f32) (*( f32 *)((s8 *)(arg2) + ( 4)));
    (*( f32 *)((s8 *)(&NuLightHazePolyvs[0]) + ( 0xC))) = (f32) (*( f32 *)((s8 *)(arg2) + ( 4)));
}
