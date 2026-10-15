#ifndef GAME_VEHSUPP_H
#define GAME_VEHSUPP_H

#include "nu_types.h"
#include "numath/numathtypes.h"

struct spline_s;

/* A position along a spline, advanced a step at a time (GC MYSPLINE). */
struct MYSPLINE {
    struct spline_s *Spline;   /* 0x00 */
    f32 Cur;                   /* 0x04  ratio along the spline */
    f32 Nex;                   /* 0x08  ratio the next step reaches */
    f32 Act;                   /* 0x0C */
    f32 Inc;                   /* 0x10  ratio step */
    struct nuvec_s CurPos;     /* 0x14 */
    struct nuvec_s NexPos;     /* 0x20 */
    f32 LookaheadDist;         /* 0x2C */
}; /* 0x30 */

f32 FindSplineClosestPointAndDist(struct MYSPLINE *Spline, s32 Control,
                                  struct nuvec_s *Point,
                                  struct nuvec_s *TargetPoint, s32 Wrap,
                                  s32 BigLook);

#endif /* GAME_VEHSUPP_H */
