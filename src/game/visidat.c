/*
 * Unit: game/visidat
 *
 * Functions:
 *   0x0024d0b0 visiSetSplineKnot
 *   0x0024d238 visiLoadData
 *   0x0024dc20 visiReset
 */

#include "creature.h"

struct nuinstflags_s {
    unsigned int visible : 1;
    unsigned int onscreen : 1;
    unsigned int visitest : 1;
    unsigned int pad : 29;
};

struct nuinstance_s {
    char pad00[0x44];
    struct nuinstflags_s flags;     /* 0x44 */
    char pad48[0x08];
}; /* 0x50 */

struct nugspline_s {
    short len;                      /* 0x00 */
    short ptsize;                   /* 0x02 */
    char *name;                     /* 0x04 */
    struct nuvec_s *pts;            /* 0x08 */
}; /* 0x0C */

struct nugscn_s {
    char pad00[0x18];
    int numinstance;                /* 0x18 */
    struct nuinstance_s *instances; /* 0x1C */
    char pad20[0x2C - 0x20];
    int numsplines;                 /* 0x2C */
    struct nugspline_s *splines;    /* 0x30 */
};

struct visidata_s {
    struct nugscn_s *sc;  /* 0x0 */
    unsigned long long ***vspline; /* 0x4 per spline, per knot: instance bit masks */
    int binfosize;        /* 0x8 */
    int curspline;        /* 0xC */
    int curknot;          /* 0x10 */
};

/* Applies the visibility masks stored for a spline knot (or the nearest
   earlier knot that has one) to the scene's instances. */
void visiSetSplineKnot(struct visidata_s *vd, int spix, int knotix)
{
    unsigned long long mask;
    int n;
    int i;
    int ix;

    if (vd == 0) {
        return;
    }
    if (spix == vd->curspline && knotix == vd->curknot) {
        return;
    }
    if (spix < 0 || spix >= vd->sc->numsplines) {
        return;
    }
    if (vd->vspline[spix] == 0) {
        return;
    }
    if (knotix < 0 || knotix >= vd->sc->splines[spix].len) {
        return;
    }
    vd->curspline = spix;
    vd->curknot = knotix;

    for (; knotix >= 0; knotix--) {
        if (vd->vspline[spix][knotix] != 0) {
            i = 0;
            for (ix = 0; ix < vd->binfosize; ix++) {
                mask = vd->vspline[spix][knotix][ix];
                for (n = 0; n < 64; n++) {
                    vd->sc->instances[i++].flags.visitest = mask & 1;
                    mask >>= 1;
                    if (i >= vd->sc->numinstance) {
                        break;
                    }
                }
                if (i >= vd->sc->numinstance) {
                    return;
                }
            }
            return;
        }
    }
}

void visiReset(struct visidata_s *vd) {
    vd->curknot = vd->curspline = -1;
}
