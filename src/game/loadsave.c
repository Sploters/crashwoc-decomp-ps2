/*
 * Unit: game/loadsave
 *
 * Functions:
 *   0x0025ad80 UpdateSaveSlots
 *   0x0025b0f8 DrawGameSlot
 *   0x0025b3e0 loadsaveCallEachFrame
 *   0x0025b420 LoadGameSlots
 *   0x0025b4c8 SaveGameSlots
 *   0x0025b508 DrawGameSlots
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
s32 saveloadASCallEachFrame();
extern s32 memcard_loadattempted;
extern s32 saveload_cardformatted;
extern s32 saveload_cardtype;

void loadsaveCallEachFrame(void) {
    saveloadASCallEachFrame();
    if (saveload_cardtype == 2) {
        if (saveload_cardformatted == 0) {
            goto block_2;
        }
    } else {
block_2:
        memcard_loadattempted = 0;
    }
}



int saveloadCheckCardFormatted(void);
s32 saveloadFormatCard(void);
s32 saveloadSaveSlot(s32, s32 *, s32);
extern s32 SaveSlot[];

void SaveGameSlots(void) {
    if (saveloadCheckCardFormatted() == 0) {
        saveloadFormatCard();
    }
    saveloadSaveSlot(0, &SaveSlot[0], 0x1030);
}
