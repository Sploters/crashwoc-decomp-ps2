/*
 * Unit: nu3d/numtl
 *
 * Functions:
 *   0x0011a900 CreateStateBlock
 *   0x0011ace8 NuMtlAnimate
 *   0x0011aff0 NuMtlBuildRenderList
 *   0x0011b1a0 NuMtlCreateDefault
 *   0x0011b2c8 NuMtlAlloc
 *   0x0011b3c8 NuMtlCreate
 *   0x0011b550 NuMtlRead
 *   0x0011bae8 NuMtlUpdateEx
 *   0x0011be38 NuMtlValid
 *   0x0011be50 NuMtlInit
 *   0x0011be98 NuMtlGetSysBuffSize
 *   0x0011bea8 NuMtlInitEx
 *   0x0011bf80 NuMtlClose
 *   0x0011c008 NuMtlRelease
 *   0x0011c080 NuMtlDestroy
 *   0x0011c130 NuMtlUpdate
 *   0x0011c150 NuMtlWrite
 *   0x0011c170 NuMtlReadEventSetHandler
 *   0x0011c198 NuMtlGetStateBlockDma
 *   0x0011c1a0 NuMtlGetZupd
 *   0x0011c1b8 NuMtlAdd
 *   0x0011c2c0 NuMtlIterate
 *   0x0011c300 NuMtlRemove
 *   0x0011c358 NuMtlReadEventDefault
 *   0x0011c360 NuMtlInsert
 *   0x0011c430 NuMtlUpdateAll
 *   0x0011c4b0 NuMtlDisplayMtl
 */

typedef unsigned char u8;
typedef unsigned long long u64;

typedef struct NuMtl {
    u8 unk000[0x120];
    u64 flags;
    u8 unk128[0x38];
    struct NuMtl *next;
    u8 unk164[0x6C];
} NuMtl;

typedef int (*NuMtlReadEventFunc)(NuMtl *mtl);

#define NUMTL_VALID 0x01

extern int NuMtlReadEventDefault(NuMtl *mtl);

extern NuMtlReadEventFunc NuMtlReadEvent;

extern NuMtl *D_0062EBA4;
#define mtl_list D_0062EBA4

extern NuMtl *D_0062EBDC;
#define mtl_iter D_0062EBDC


int NuMtlValid(NuMtl *mtl) {
    u64 flags;

    flags = mtl->flags;
    return -((u8)flags & NUMTL_VALID);
}


int NuMtlGetSysBuffSize(int count) {
    return count * sizeof(NuMtl);
}


NuMtlReadEventFunc NuMtlReadEventSetHandler(NuMtlReadEventFunc handler) {
    NuMtlReadEventFunc old;

    old = NuMtlReadEvent;
    if (handler) {
        NuMtlReadEvent = handler;
    } else {
        NuMtlReadEvent = NuMtlReadEventDefault;
    }
    return old;
}


void *NuMtlGetStateBlockDma(void *dma) {
    return dma;
}


NuMtl *NuMtlIterate(int restart) {
    NuMtl *mtl;

    if (restart) {
        mtl = mtl_list;
        if (mtl) {
            mtl_iter = mtl->next;
        }
        return mtl;
    }
    mtl = mtl_iter;
    if (mtl) {
        mtl_iter = mtl->next;
    }
    return mtl;
}


int NuMtlReadEventDefault(NuMtl *mtl) {
    return 0;
}


int NuMtlGetZupd(NuMtl *mtl) {
    return -(((int)(mtl->flags & 0xFF) << 28) < 0);
}


#ifndef NULL
#define NULL ((void *)0)
#endif
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef float f32;
typedef double f64;
extern s32 D_0062EBA0;
extern s32 D_0062EBA8;
extern s32 D_0062EBB0;

void NuMtlInit(void) {
    if (D_0062EBA0 != 0) {
        NuMtlClose();
    }
    D_0062EBA4 = 0;
    D_0062EBA0 = 1;
    D_0062EBB0 = 0;
    D_0062EBA8 = 0;
    NuMtlCreateDefault();
    NuMtlReadEventSetHandler(0);
}



extern s32 D_0062EBAC;

void NuMtlInitEx(s32 *arg0, s32 arg1) {
    s32 temp_a2;
    s32 temp_v1;
    s32 var_a0;
    s32 var_v1;

    if (D_0062EBA0 != 0) {
        NuMtlClose();
    }
    D_0062EBA4 = 0;
    D_0062EBA0 = 1;
    D_0062EBB0 = 0;
    D_0062EBA8 = 0;
    NuMtlCreateDefault();
    NuMtlReadEventSetHandler(0);
    temp_a2 = arg1 * 0x1D0;
    temp_v1 = (*arg0 + 0xF) & 0xFFFFFFF0;
    D_0062EBB0 = arg1;
    D_0062EBAC = 0;
    *arg0 = temp_v1;
    D_0062EBA8 = temp_v1;
    *arg0 = temp_v1 + temp_a2;
    memset(temp_v1, 0, temp_a2);
    var_v1 = D_0062EBA8;
    if (D_0062EBB0 > 0) {
        var_a0 = D_0062EBB0;
        do {
            var_a0 -= 1;
            (*( s64 *)((s8 *)(var_v1) + ( 0x120))) = (s64) ((*( s64 *)((s8 *)(var_v1) + ( 0x120))) | 2);
            var_v1 += 0x1D0;
        } while (var_a0 != 0);
    }
    NuMtlCreateDefault();
}



void NuDisableVBlank(void);
void NuEnableVBlank(void);

void NuMtlRemove(void *arg0) {
    void *temp_v0;
    void *temp_v1;
    void *temp_v1_2;

    NuDisableVBlank();
    temp_v1 = (*( void **)((s8 *)(arg0) + ( 0x164)));
    temp_v0 = (*( void **)((s8 *)(arg0) + ( 0x160)));
    if (temp_v1 != ((void *)0)) {
        (*( void **)((s8 *)(temp_v1) + ( 0x160))) = temp_v0;
    } else {
        D_0062EBA4 = temp_v0;
    }
    temp_v1_2 = (*( void **)((s8 *)(arg0) + ( 0x160)));
    if (temp_v1_2 != ((void *)0)) {
        (*( void **)((s8 *)(temp_v1_2) + ( 0x164))) = (void *) (*( void **)((s8 *)(arg0) + ( 0x164)));
    }
    NuEnableVBlank();
}



s32 (*NuDebugMsgProlog(s32 *, s32))(s32 *);
void NuMemFreeFn(void *ptr, void *src, s32 id);
short NuTexUnRef(int tex);
extern s32 D_00614470[];
extern s32 D_00614508[];

void NuMtlDestroy(void * a_0)
{
    void *arg0 = a_0;

    s32 temp_a0;
    s64 temp_a0_2;

    temp_a0 = (*( s32 *)((s8 *)(arg0) + ( 0x1A4)));
    if (temp_a0 != 0) {
        NuTexUnRef(temp_a0);
    }
    NuMtlRemove(arg0);
    temp_a0_2 = (*( s64 *)((s8 *)(arg0) + ( 0x120)));
    if (temp_a0_2 & 2) {
        if (!(temp_a0_2 & 1)) {
            NuDebugMsgProlog(&D_00614470[0], 0x3EC)(&D_00614508[0]);
        }
        (*( s64 *)((s8 *)(arg0) + ( 0x120))) = (s64) ((*( s64 *)((s8 *)(arg0) + ( 0x120))) & ~1);
        return;
    }
    (*( s64 *)((s8 *)(arg0) + ( 0x120))) = (s64) (temp_a0_2 & ~1);
    if (temp_a0_2 & 4) {
        NuMemFreeFn(arg0, &D_00614470[0], 0x3F2);
    }
}
