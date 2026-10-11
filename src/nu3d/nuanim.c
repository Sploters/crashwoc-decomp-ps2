/*
 * Unit: nu3d/nuanim
 *
 * Functions:
 *   0x00140508 NuAnimDataWrite
 *   0x00140850 NuAnimDataRead
 *   0x00140c00 NuAnimDataLoad
 *   0x00140d40 NuAnimDataFixPtrs
 *   0x00140f38 NuAnimData2FixPtrs
 *   0x00141080 NuAnimCurveCalcVal2
 *   0x00141220 NuAnimCurve2CalcVal
 *   0x00141500 NuAnimCurveSetApplyToJoint
 *   0x00141898 NuAnimCurveSetApplyBlendToJoint
 *   0x00141e88 NuAnimCurveSetApplyBlendToJoint2
 *   0x001426f8 NuAnimCurve2SetApplyBlendToJoint2
 *   0x00142f08 NuAnimCurveSetApplyToJoint2
 *   0x00143510 NuAnimCurve2SetApplyToJoint
 *   0x00143a70 NuAnimCurveSetApplyToJointBasic
 *   0x00143dc8 NuAnimCurve2SetApplyToJointBasic
 *   0x001440e8 NuAnimCurveSetApplyToMatrix
 *   0x00144350 NuAnimCurve2SetApplyToMatrix
 *   0x00144518 NuAnimDataLoadBuff
 *   0x001445f8 NuAnimDataDestroy
 *   0x00144688 NuAnimDataCalcTime
 *   0x001447c8 NuAnimDataCreate
 *   0x00144838 NuAnimCurveSetCreate
 *   0x001448f8 NuAnimCurveSetDestroy
 *   0x001449e0 NuAnimCurveSetWrite
 *   0x00144b68 NuAnimCurveCreate
 *   0x00144c18 NuAnimCurveDestroy
 *   0x00144c68 NuAnimCurveWrite
 *   0x00144d50 NuAnimCurveRead
 *   0x00144e28 NuAnimKeyWrite
 *   0x00144e88 NuAnimKeyRead
 *   0x00144ee8 NuAnimDataFindVersion
 *   0x00144f50 NuAnimData2LoadBuff
 *   0x00145030 NuAnimData2CalcTime
 *   0x00145170 NuAnimDataChunkCreate
 *   0x00145220 NuAnimDataChunkDestroy
 *   0x001452f0 buildBitCountTable
 *   0x00145358 NuAnimCurveScale
 */


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
void *NuMemAllocFn(s32, s32 *, s32);
s32 memset(void *, s32, s32);
extern s32 D_00614A60[];

void *NuAnimDataCreate(s32 arg0) {
    s32 temp_s1;
    void *temp_v0;

    temp_s1 = (arg0 * 4) + 0x10;
    temp_v0 = NuMemAllocFn(temp_s1, &D_00614A60[0], 0x291);
    memset(temp_v0, 0, temp_s1);
    (*( s32 *)((s8 *)(temp_v0) + ( 8))) = arg0;
    (*( void **)((s8 *)(temp_v0) + ( 0xC))) = (void *) (temp_v0 + 0x10);
    return temp_v0;
}



s32 (*NuErrorProlog(s32 *, s32))(s32 *);
extern s32 D_0062EE20[];

void *NuAnimCurveCreate(s32 arg0) {
    s32 temp_s2;
    void *temp_v0;
    void *temp_v0_2;

    if (arg0 <= 0) {
        NuErrorProlog(&D_00614A60, 0x469)(&D_0062EE20[0]);
    }
    temp_v0 = NuMemAllocFn(0x10, &D_00614A60, 0x46B);
    temp_s2 = arg0 * 0x10;
    memset(temp_v0, 0, 0x10);
    (*( s32 *)((s8 *)(temp_v0) + ( 8))) = arg0;
    temp_v0_2 = NuMemAllocFn(temp_s2, &D_00614A60, 0x46F);
    (*( void **)((s8 *)(temp_v0) + ( 4))) = temp_v0_2;
    memset(temp_v0_2, 0, temp_s2);
    return temp_v0;
}



s32 NuMemFreeFn(void *, s32 *, s32);

void NuAnimCurveDestroy(void *arg0) {
    void *temp_a0;

    temp_a0 = (*( void **)((s8 *)(arg0) + ( 4)));
    if (temp_a0 != ((void *)0)) {
        NuMemFreeFn(temp_a0, &D_00614A60, 0x479);
    }
    NuMemFreeFn(arg0, &D_00614A60, 0x47A);
}



s32 (*NuErrorProlog(s32 *, s32))(s32 *);

void *NuAnimDataChunkCreate(s32 arg0) {
    s32 temp_s2;
    void *temp_v0;
    void *temp_v0_2;

    if (arg0 <= 0) {
        NuErrorProlog(&D_00614A60, 0x27C)(&D_0062EE20);
    }
    temp_v0 = NuMemAllocFn(0x14, &D_00614A60, 0x27D);
    temp_s2 = arg0 * 4;
    memset(temp_v0, 0, 0x14);
    (*( s32 *)((s8 *)(temp_v0) + ( 0))) = arg0;
    temp_v0_2 = NuMemAllocFn(temp_s2, &D_00614A60, 0x281);
    (*( void **)((s8 *)(temp_v0) + ( 8))) = temp_v0_2;
    memset(temp_v0_2, 0, temp_s2);
    return temp_v0;
}
