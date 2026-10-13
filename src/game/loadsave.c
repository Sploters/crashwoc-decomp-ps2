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

#include "creature.h"

extern void saveloadASCallEachFrame(void);
extern s32 saveloadCheckCardFormatted(void);
extern s32 saveloadFormatCard(void);
extern s32 saveloadSaveSlot(s32 slot, void *buf, s32 size);
extern s32 saveload_cardtype;
extern s32 saveload_cardformatted;
extern s32 memcard_loadattempted;
extern u8 SaveSlot[];

void loadsaveCallEachFrame(void) {
    saveloadASCallEachFrame();
    if (saveload_cardtype != 2 || saveload_cardformatted == 0) {
        memcard_loadattempted = 0;
    }
}

void SaveGameSlots(void) {
    if (saveloadCheckCardFormatted() == 0) {
        saveloadFormatCard();
    }
    saveloadSaveSlot(0, SaveSlot, 0x1030);
}
