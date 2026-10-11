/*
 * Unit: gamelib/nuwind
 *
 * Functions:
 *   0x001b51e0 NuWindCreate
 *   0x001b5498 NuWindUpdate
 *   0x001b5c80 NuWindInit
 *   0x001b5c98 NuWindDraw
 *   0x001b5db0 NuWindRand
 *   0x001b5dd0 NuWindAllocateGrp
 *   0x001b5e08 NuWindFreeGrp
 *   0x001b5e20 NuWindAllocMtxs
 *   0x001b5e58 NuWindFreeMtxs
 */


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
extern s32 NuWindDir;
extern s32 NuWindDir2;
extern s32 NuWindGCount;
extern s32 NuWindMtxIndex;
extern s32 NuWindWave;

void NuWindInit(void) {
    NuWindDir = 0;
    NuWindDir2 = 0;
    NuWindWave = 0;
    NuWindGCount = 0;
    NuWindMtxIndex = 0;
}



extern s32 NuWindQS;

s32 NuWindRand(void) {
    s32 temp_v1;

    temp_v1 = ((NuWindQS * 0x24CD) + 1) & 0xFFFF;
    NuWindQS = temp_v1;
    return temp_v1;
}





void NuWindFreeGrp(s32 arg0) {
    if (arg0 != 0) {
        NuWindGCount -= 1;
    }
}





void NuWindFreeMtxs(s32 arg0, s32 arg1) {
    if (arg0 != 0) {
        NuWindMtxIndex -= arg1;
    }
}
