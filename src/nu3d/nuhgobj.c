/*
 * Unit: nu3d/nuhgobj
 *
 * Functions:
 *   0x0014c828 NuHGobjDestroyDynamic
 *   0x0014ccc0 NuHGobjLoad
 *   0x0014ce70 ReadNuIFFHGobj
 *   0x0014d0a0 ReadNuIFFHGobjSet
 *   0x0014d9e8 NuGHGWriteGobj
 *   0x0014dc50 NuGHGWrite
 *   0x0014ed60 NuGHGRead
 *   0x0014f7e8 NuHGobjBoundingBoxToLine
 *   0x0014fae0 NuHGobjCollideSphere
 *   0x0014fce8 NuHGobjDestroy
 *   0x0014fdb8 NuHGobjPOIMtx
 *   0x0014fe28 NuHGobjPOIMtxWithCheck
 *   0x0014feb8 NuHGobjGetPOI
 *   0x0014fef8 NuHGobjJointMtx
 *   0x0014ff30 NuHGobjJointMtxWithCheck
 *   0x0014ff88 NuHGobjBoundingSphereToSphere
 *   0x001500e0 NuHGobjBoundingBoxToSphere
 *   0x001501c8 NuHGobjBoundingCylinderToSphere
 *   0x00150278 NuHGobjBoundingSphereToLine
 *   0x00150338 NuHGobjReadForCutScene
 *   0x001503f0 ReadNuIFFCollideData
 *   0x001504c8 NuGHGFixupGobj
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
s32 NuFileBeginBlkRead(s32, s32);
s32 NuFileEndBlkRead(s32);
void *NuMemAllocFn(s32, s32 *, s32);
s32 memset(void *, s32, s32);
extern s32 D_00615290[];

void *NuHGobjReadForCutScene(s32 arg0, void *arg1) {
    void *temp_v0;

    temp_v0 = NuMemAllocFn(0x84, &D_00615290[0], 0xE5);
    memset(temp_v0, 0, 0x84);
    (*( s32 *)((s8 *)(temp_v0) + ( 0x10))) = 0;
    (*( f32 *)((s8 *)(temp_v0) + ( 0x38))) = 1.0f;
    (*( s32 *)((s8 *)(temp_v0) + ( 0xC))) = (s32) (*( s32 *)((s8 *)(arg1) + ( 0xC)));
    (*( s32 *)((s8 *)(temp_v0) + ( 8))) = (s32) (*( s32 *)((s8 *)(arg1) + ( 8)));
    (*( s32 *)((s8 *)(temp_v0) + ( 4))) = (s32) (*( s32 *)((s8 *)(arg1) + ( 4)));
    (*( s32 *)((s8 *)(temp_v0) + ( 0))) = (s32) (*( s32 *)((s8 *)(arg1) + ( 0)));
    NuFileBeginBlkRead(arg0, 0x304F4748);
    ReadNuIFFHGobjSet(arg0, temp_v0);
    NuFileEndBlkRead(arg0);
    return temp_v0;
}
