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

/* Only the fields DrawGameSlots reads (full layouts in game/game.c, game/camera.c). */
struct cursor_s {
    u8 pad00[0x68];
    char x;                  /* 0x68 */
    char y;                  /* 0x69 */
};

struct gametimer_s {
    u32 frame;               /* 0x00 */
    u8 pad04[0x18 - 0x04];
};                           /* 0x18 */

extern struct cursor_s Cursor;
extern struct gametimer_s GlobalTimer;
extern f32 SLOTPANELDX;
extern f32 SLOTPANELDY;
extern void DrawGameSlot(struct game_s *slot, f32 x, f32 y, s32 colour);

/* The 2x2 grid of save slots; the one under the cursor flashes. */
void DrawGameSlots(void) {
    f32 dx;
    f32 dy;
    f32 x;
    f32 y;
    s32 i;
    s32 j;
    s32 colour;

    dx = SLOTPANELDX;
    dy = SLOTPANELDY;
    y = dy * 0.5f + 0.0f;
    for (i = 0; i < 2; i++) {
        x = 0.0f - dx * 0.5f;
        for (j = 0; j < 2; j++, x += dx) {
            colour = 0;
            if (Cursor.y == i && Cursor.x == j) {
                colour = 3;
                if (GlobalTimer.frame % 10 <= 4) {
                    colour = 4;
                }
            }
            DrawGameSlot(&SaveSlot[i][j], x, y, colour);
        }
        y -= dy;
    }
}
