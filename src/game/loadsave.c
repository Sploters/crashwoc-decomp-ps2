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
extern s32 saveloadCheckCardType(void);
extern s32 saveloadLoadSlot(s32 slot, void *buf, s32 size);
extern s32 memcard_in;
extern s32 memcard_formatted;
extern s32 memcard_gamesavailable;

/* Saved game (full layout in game/game.c). */
struct game_s {
    u8 pad000[0x407];
    u8 empty;                /* 0x407 */
    u32 cutbits;             /* 0x408 */
}; /* 0x40C */

/* Four slots: two pages of two (game.c indexes them as x * 2 + y). */
extern struct game_s SaveSlot[2][2];

void loadsaveCallEachFrame(void) {
    saveloadASCallEachFrame();
    if (saveload_cardtype != 2 || saveload_cardformatted == 0) {
        memcard_loadattempted = 0;
    }
}

void LoadGameSlots(void) {
    s32 x;
    s32 y;

    for (x = 0; x < 2; x++) {
        for (y = 0; y < 2; y++) {
            SaveSlot[x][y].empty = 1;
        }
    }
    memcard_in = saveloadCheckCardType();
    memcard_formatted = 0;
    memcard_gamesavailable = 0;
    if (memcard_in != 0) {
        memcard_formatted = saveloadCheckCardFormatted();
        if (memcard_formatted != 0) {
            memcard_gamesavailable = saveloadLoadSlot(0, SaveSlot, sizeof(SaveSlot));
        }
    }
}

void SaveGameSlots(void) {
    if (saveloadCheckCardFormatted() == 0) {
        saveloadFormatCard();
    }
    saveloadSaveSlot(0, SaveSlot, sizeof(SaveSlot));
}
