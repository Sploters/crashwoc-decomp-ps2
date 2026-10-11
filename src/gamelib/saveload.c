/*
 * Unit: gamelib/saveload
 *
 * Functions:
 *   0x001a9b08 saveloadGetDirectory
 *   0x001a9c98 saveloadSaveSlot
 *   0x001aa020 saveloadDeleteSlot
 *   0x001aa1c0 saveloadASCallEachFrame
 *   0x001aa918 saveloadLoadIcon
 *   0x001aa968 saveloadCheckCardPresent
 *   0x001aa9b0 saveloadCheckCardType
 *   0x001aaa08 saveloadCheckCardFormatted
 *   0x001aaa70 saveloadCheckCardFreeSpace
 *   0x001aab08 saveloadFormatCard
 *   0x001aab40 saveloadUnFormatCard
 *   0x001aab78 saveloadCheckSlotsUsed
 *   0x001aabb0 saveloadLoadSlot
 *   0x001aacf8 saveloadASFormat
 *   0x001aad30 saveloadASLoad
 *   0x001aada8 saveloadASSave
 *   0x001aae20 saveloadASDelete
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
s32 sceMcChdir(s32, s32, s32 *, s32);
s32 sprintf(s32 *, s32 *, s32 *, s32);
extern s32 D_00619B70[];
extern s32 D_00619BB0[];
extern s32 D_00619BC0[];
extern s32 saveload_slotid;
extern s32 saveload_slotname[];
extern s32 saveload_status;

void saveloadASDelete(s32 arg0) {
    if (arg0 < 4) {
        saveload_slotid = arg0;
        sprintf(&saveload_slotname[0], &D_00619BB0[0], &D_00619B70[0], arg0);
        sceMcChdir(0, 0, &D_00619BC0[0], 0);
        saveload_status = 0x19;
    }
}



extern s32 saveload_data;
extern s32 saveload_datasize;

void saveloadASLoad(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 < 4) {
        saveload_slotid = arg0;
        saveload_data = arg1;
        saveload_datasize = arg2;
        sprintf(&saveload_slotname, &D_00619BB0, &D_00619B70, arg0);
        sceMcChdir(0, 0, &D_00619BC0, 0);
        saveload_status = 5;
    }
}



s32 sceMcSync(s32, s32, s32 *);
s32 sceMcUnformat(s32, s32);
extern s32 D_00633228;

s32 saveloadUnFormatCard(void) {
    sceMcUnformat(0, 0);
    sceMcSync(0, 0, &D_00633228);
    return D_00633228 == 0;
}





void saveloadASFormat(void) {
    if (saveload_status == 1) {
        sceMcFormat(0, 0);
        saveload_status = 4;
    }
}





void saveloadASSave(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 < 4) {
        saveload_slotid = arg0;
        saveload_data = arg1;
        saveload_datasize = arg2;
        sprintf(&saveload_slotname, &D_00619BB0, &D_00619B70, arg0);
        sceMcChdir(0, 0, &D_00619BC0, 0);
        saveload_status = 9;
    }
}
