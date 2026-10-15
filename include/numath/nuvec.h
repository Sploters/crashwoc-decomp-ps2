#ifndef NUMATH_NUVEC_H
#define NUMATH_NUVEC_H

#include "nu_types.h"
#include "numath/numathtypes.h"

void NuVecRotateAxis(struct nuvec_s *dst, struct nuvec_s *src, void *unused, struct nuvec_s *rot);
void NuVecMtxTranslate(struct nuvec_s *dst, struct nuvec_s *src, struct numtx_s *mtx);
void NuVecMtxRotate(struct nuvec_s *dst, struct nuvec_s *src, struct numtx_s *mtx);
void NuVecMtxTransform(struct nuvec_s *dst, struct nuvec_s *src, struct numtx_s *mtx);
void NuVecMtxTransformH(struct nuvec_s *dst, struct nuvec_s *src, struct numtx_s *mtx);
void NuVecMtxRotateValX(struct nuvec_s *dst, struct numtx_s *mtx, f32 val);
void NuVecMtxRotateValY(struct nuvec_s *dst, struct numtx_s *mtx, f32 val);
void NuVecMtxRotateValZ(struct nuvec_s *dst, struct numtx_s *mtx, f32 val);
void NuVecMtxRotateH(struct nuvec_s *dst, struct nuvec_s *src, struct numtx_s *mtx);
void NuVecMtxScale(struct nuvec_s *dst, struct nuvec_s *src, struct numtx_s *mtx);
void NuVecInvMtxTranslate(struct nuvec_s *dst, struct nuvec_s *src, struct numtx_s *mtx);
void NuVecInvMtxRotate(struct nuvec_s *dst, struct nuvec_s *src, struct numtx_s *mtx);
void NuVecInvMtxTransform(struct nuvec_s *dst, struct nuvec_s *src, struct numtx_s *mtx);
void NuVecInvMtxRotateValX(struct nuvec_s *v, struct numtx_s *mtx);
void NuVecInvMtxRotateValY(struct nuvec_s *v, struct numtx_s *mtx);
void NuVecInvMtxRotateValZ(struct nuvec_s *v, struct numtx_s *mtx);
void NuVecInvMtxScale(struct nuvec_s *dst, struct nuvec_s *src, struct numtx_s *mtx);
void NuVecRotateX(struct nuvec_s *dst, struct nuvec_s *src, int angle);
void NuVecRotateY(struct nuvec_s *dst, struct nuvec_s *src, int angle);
void NuVecRotateZ(struct nuvec_s *dst, struct nuvec_s *src, int angle);
void NuVecAdd(struct nuvec_s *dst, struct nuvec_s *a, struct nuvec_s *b);
void NuVecSub(struct nuvec_s *dst, struct nuvec_s *a, struct nuvec_s *b);
void NuVecScale(struct nuvec_s *dst, struct nuvec_s *src, f32 scale);
void NuVecScaleAccum(struct nuvec_s *dst, struct nuvec_s *src, f32 scale);
void NuVecInvScale(struct nuvec_s *dst, struct nuvec_s *src, f32 scale);
void NuVecCross(struct nuvec_s *dst, struct nuvec_s *a, struct nuvec_s *b);
void NuVecCrossRel(struct nuvec_s *dst, struct nuvec_s *origin, struct nuvec_s *p1, struct nuvec_s *p2);
f32 NuVecMagSqr(struct nuvec_s *v);
f32 NuVecMag(struct nuvec_s *v);
f32 NuVecNorm(struct nuvec_s *dst, struct nuvec_s *src);
void NuVecSurfaceNormal(struct nuvec_s *dst, struct nuvec_s *p0, struct nuvec_s *p1, struct nuvec_s *p2);
f32 NuVecDot(struct nuvec_s *a, struct nuvec_s *b);
void NuVecLerp(struct nuvec_s *dst, struct nuvec_s *a, struct nuvec_s *b, f32 t);
f32 NuVecDist(struct nuvec_s *a, struct nuvec_s *b, struct nuvec_s *diff);
f32 NuVecDistSqr(struct nuvec_s *a, struct nuvec_s *b, struct nuvec_s *diff);
f32 NuVecXZDist(struct nuvec_s *a, struct nuvec_s *b, struct nuvec_s *diff);
f32 NuVecXZDistSqr(struct nuvec_s *a, struct nuvec_s *b, struct nuvec_s *diff);
void NuLineLineIntersect(struct nuvec_s *p0, struct nuvec_s *d0, struct nuvec_s *p1, struct nuvec_s *d1, f32 *t0, f32 *t1);

#endif /* NUMATH_NUVEC_H */
