/*
 * Unit: game/visi
 *
 * Functions:
 *   0x00201450 ApplyVisiTable
 *   0x00201668 VSAddObjs
 *   0x00201868 BuildVisiTable
 *   0x00201a00 ReleaseVisiTable
 */

#include "creature.h"

extern void NuMemFreeFn(void *ptr, char *file, int line);

/* Retail-owned (no Data-from-C yet). */
extern char D_0061F2E8[];                /* source file name */
extern int D_0063326C;
extern char *D_00633270;
#define visi_srcfile D_0061F2E8
#define vscnt D_0063326C
#define vsdata D_00633270

void ReleaseVisiTable(void) {
    if (vsdata != 0) {
        NuMemFreeFn(vsdata, visi_srcfile, 0xC0);
    }
    vscnt = 0;
    vsdata = 0;
}
