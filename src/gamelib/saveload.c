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

typedef int s32;

extern s32 NuFileSize(char *name);
extern s32 NuFileLoadBuffer(char *name, void *buf, s32 max);
extern s32 sprintf(char *buf, const char *fmt, ...);
extern s32 sceMcSync(s32 mode, s32 *cmd, s32 *result);
extern s32 sceMcGetInfo(s32 port, s32 slot, s32 *type, s32 *free, s32 *format);
extern s32 sceMcChdir(s32 port, s32 slot, const char *path, char *pwd);
extern s32 sceMcFormat(s32 port, s32 slot);
extern s32 sceMcUnformat(s32 port, s32 slot);
extern s32 sceMcGetDir(s32 port, s32 slot, const char *name, unsigned int mode, s32 maxent,
                       void *table);
extern s32 sceMcOpen(s32 port, s32 slot, const char *name, s32 mode);
extern s32 sceMcRead(s32 fd, void *buf, s32 size);
extern s32 sceMcClose(s32 fd);

s32 saveloadGetDirectory(void);

extern s32 D_0062F770;
extern void *iconbuff;
extern s32 saveload_status;
extern s32 saveload_slotid;
extern void *saveload_data;
extern s32 saveload_datasize;
extern char saveload_slotname[];

extern s32 D_0063321C;
extern s32 D_00633220;
extern s32 D_00633224;
extern s32 D_00633228;

extern char D_00619B70[];
extern char D_00619BB0[];
extern char D_00619BC0[];

#define iconsize D_0062F770
#define mc_type D_0063321C
#define mc_free D_00633220
#define mc_format D_00633224
#define mc_result D_00633228
#define saveload_gamecode D_00619B70
#define saveload_slotfmt D_00619BB0
#define saveload_savedir D_00619BC0

#define SCE_RDONLY 1

extern char fname[];
extern char D_006B3B40[];               /* sceMcGetDir entry table */
#define mc_dirtable D_006B3B40


extern char dname[];
extern char D_0062F790[];
extern char D_0062F798[];
extern char D_0062F7A0[];
extern char D_0062F7A8[];
#define saveload_rootdir D_0062F790
#define saveload_fnamefmt D_0062F798
#define saveload_dnamefmt D_0062F7A0
#define saveload_dirfmt D_0062F7A8

s32 saveloadCheckCardPresent(void) {
    s32 nocard;

    sceMcGetInfo(0, 0, 0, 0, 0);
    sceMcSync(0, 0, &mc_result);
    nocard = -10;
    return mc_result > nocard;
}


/* Fills mc_dirtable with the save directory listing. Returns 0 when the
   directory does not exist yet, otherwise the entry count or -1 on error. */
s32 saveloadGetDirectory(void) {
    char path[32];
    s32 present;

    sceMcGetInfo(0, 0, 0, 0, 0);
    sceMcSync(0, 0, &mc_result);
    present = mc_result > -10;
    if (!present) {
        return -1;
    }
    sceMcChdir(0, 0, saveload_rootdir, 0);
    sceMcSync(0, 0, &mc_result);
    if (mc_result != 0) {
        return -1;
    }
    sprintf(fname, saveload_fnamefmt, saveload_gamecode);
    sprintf(dname, saveload_dnamefmt, fname);
    sprintf(path, saveload_dirfmt, fname);
    sceMcGetDir(0, 0, fname, 0, 0x12, mc_dirtable);
    sceMcSync(0, 0, &mc_result);
    if (mc_result == 0) {
        return 0;
    }
    sceMcChdir(0, 0, dname, 0);
    sceMcSync(0, 0, &mc_result);
    if (mc_result != 0) {
        return -1;
    }
    sceMcGetDir(0, 0, path, 0, 0x12, mc_dirtable);
    sceMcSync(0, 0, &mc_result);
    if (mc_result == 0) {
        return -1;
    }
    return mc_result;
}

s32 saveloadLoadIcon(char *name, void *buf) {
    iconsize = NuFileSize(name);
    iconbuff = buf;
    NuFileLoadBuffer(name, buf, iconsize);
    return iconsize;
}



s32 saveloadCheckCardType(void) {
    sceMcGetInfo(0, 0, &mc_type, 0, 0);
    sceMcSync(0, 0, &mc_result);
    if (mc_result < -9) {
        return 0;
    }
    return mc_type == 2;
}


s32 saveloadCheckCardFormatted(void) {
    sceMcGetInfo(0, 0, &mc_type, 0, &mc_format);
    sceMcSync(0, 0, &mc_result);
    if (mc_result < -9) {
        return 0;
    }
    if (mc_type != 2) {
        return 0;
    }
    return mc_format == 1;
}


s32 saveloadCheckCardFreeSpace(s32 size) {
    sceMcGetInfo(0, 0, &mc_type, &mc_free, &mc_format);
    sceMcSync(0, 0, &mc_result);
    if (mc_result < -9) {
        return 0;
    }
    if (mc_type != 2) {
        return 0;
    }
    if (mc_format != 1) {
        return 0;
    }
    if (size <= 0) {
        return mc_free;
    }
    if (mc_free >= size) {
        return 1;
    }
    return 0;
}


s32 saveloadFormatCard(void) {
    sceMcFormat(0, 0);
    sceMcSync(0, 0, &mc_result);
    return mc_result == 0;
}


s32 saveloadUnFormatCard(void) {
    sceMcUnformat(0, 0);
    sceMcSync(0, 0, &mc_result);
    return mc_result == 0;
}


s32 saveloadCheckSlotsUsed(void) {
    s32 slots;

    slots = saveloadGetDirectory();
    if (slots == -1 || slots == 0) {
        return 0;
    }
    return slots - 1;
}


/* Read slot `slot` of the save into `buf`; returns 1 on success, 0 if the
 * card, the save directory or the slot file is missing or a step fails.
 * Near-match (state=asm): retail picks the final result with movn on `found`
 * where this build uses movz on 0; everything else is exact. */
s32 saveloadLoadSlot(s32 slot, void *buf, s32 size) {
    char path[32];
    s32 dir;
    s32 found;
    s32 fd;

    if (slot >= 4) {
        return 0;
    }
    dir = saveloadGetDirectory();
    if (dir == 0 || dir == -1) {
        return 0;
    }
    sprintf(path, saveload_slotfmt, fname, slot);
    sceMcGetDir(0, 0, path, 0, 0x12, mc_dirtable);
    sceMcSync(0, 0, &mc_result);
    found = mc_result;
    if (found != 1) {
        return 0;
    }
    sceMcOpen(0, 0, path, SCE_RDONLY);
    sceMcSync(0, 0, &mc_result);
    fd = mc_result;
    if (fd < 0) {
        return 0;
    }
    sceMcRead(fd, buf, size);
    sceMcSync(0, 0, &mc_result);
    if (mc_result < 0) {
        return 0;
    }
    sceMcClose(fd);
    sceMcSync(0, 0, &mc_result);
    if (mc_result < 0) {
        return 0;
    }
    return found;
}

void saveloadASFormat(void) {
    if (saveload_status == 1) {
        sceMcFormat(0, 0);
        saveload_status = 4;
    }
}


void saveloadASLoad(s32 slot, void *data, s32 size) {
    if (slot < 4) {
        saveload_slotid = slot;
        saveload_data = data;
        saveload_datasize = size;
        sprintf(saveload_slotname, saveload_slotfmt, saveload_gamecode, slot);
        sceMcChdir(0, 0, saveload_savedir, 0);
        saveload_status = 5;
    }
}


void saveloadASSave(s32 slot, void *data, s32 size) {
    if (slot < 4) {
        saveload_slotid = slot;
        saveload_data = data;
        saveload_datasize = size;
        sprintf(saveload_slotname, saveload_slotfmt, saveload_gamecode, slot);
        sceMcChdir(0, 0, saveload_savedir, 0);
        saveload_status = 9;
    }
}


void saveloadASDelete(s32 slot) {
    if (slot < 4) {
        saveload_slotid = slot;
        sprintf(saveload_slotname, saveload_slotfmt, saveload_gamecode, slot);
        sceMcChdir(0, 0, saveload_savedir, 0);
        saveload_status = 0x19;
    }
}
