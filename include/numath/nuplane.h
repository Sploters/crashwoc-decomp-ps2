#ifndef NUMATH_NUPLANE_H
#define NUMATH_NUPLANE_H

#include "nu_types.h"
#include "numath/numathtypes.h"

s32 NuPlnPlnIntersect(struct nuplane_s *p0, struct nuplane_s *p1, struct nuvec_s *point, struct nuvec_s *direction);
void NuPlnEqn(struct nuplane_s *plane, struct nuvec_s *p0, struct nuvec_s *p1, struct nuvec_s *p2);
void NuPlnEqnPn(struct nuplane_s *plane, struct nuvec_s *point, struct nuvec_s *normal);
f32 NuPlnDist(struct nuplane_s *plane, struct nuvec_s *point);
f32 NuPlnDist2(struct nuplane_s *plane, struct nuvec_s *p0, struct nuvec_s *p1);
s32 NuPlnLine(struct nuplane_s *plane, struct nuvec_s *p0, struct nuvec_s *p1, struct nuvec_s *intersection);
s32 NuPointRelToBoundingBox(struct nuvec_s *point, struct nuvec_s *upper, struct nuvec_s *lower);
s32 NuPtInPolyXY(struct nuvec_s *point, struct nuvec_s *v0, struct nuvec_s *v1, struct nuvec_s *v2);
s32 NuPtInPolyYX(struct nuvec_s *point, struct nuvec_s *v0, struct nuvec_s *v1, struct nuvec_s *v2);
s32 NuPtInPolyXZ(struct nuvec_s *point, struct nuvec_s *v0, struct nuvec_s *v1, struct nuvec_s *v2);
s32 NuPtInPolyZX(struct nuvec_s *point, struct nuvec_s *v0, struct nuvec_s *v1, struct nuvec_s *v2);
s32 NuPtInPolyYZ(struct nuvec_s *point, struct nuvec_s *v0, struct nuvec_s *v1, struct nuvec_s *v2);
s32 NuPtInPolyZY(struct nuvec_s *point, struct nuvec_s *v0, struct nuvec_s *v1, struct nuvec_s *v2);
f32 NuLineToPointDistSqr(struct nuvec_s *p0, struct nuvec_s *p1, struct nuvec_s *point);
s32 NuPlnLine2(struct nuplane_s *plane, struct nuvec_s *v0, struct nuvec_s *v1, struct nuvec_s *v2, struct nuvec_s *p0, struct nuvec_s *p1, struct nuvec_s *intersection, f32 *distance, f32 *along);
s32 NuPtInPoly(struct nuvec_s *point, struct nuvec_s *v0, struct nuvec_s *v1, struct nuvec_s *v2, struct nuplane_s *plane);
void NuClipXPlane(struct nuvec_s *out, struct nuvec_s *point, struct nuvec_s *direction, f32 *x);
void NuClipYPlane(struct nuvec_s *out, struct nuvec_s *point, struct nuvec_s *direction, f32 *y);
void NuClipZPlane(struct nuvec_s *out, struct nuvec_s *point, struct nuvec_s *direction, f32 *z);

#endif /* NUMATH_NUPLANE_H */
