/*
 * Unit: game/visidat
 *
 * Functions:
 *   0x0024d0b0 visiSetSplineKnot
 *   0x0024d238 visiLoadData
 *   0x0024dc20 visiReset
 */

#include "creature.h"

struct visidata_s {
    struct nugscn_s *sc;  /* 0x0 */
    void ***vspline;      /* 0x4 */
    int binfosize;        /* 0x8 */
    int curspline;        /* 0xC */
    int curknot;          /* 0x10 */
};

void visiReset(struct visidata_s *vd) {
    vd->curknot = vd->curspline = -1;
}
