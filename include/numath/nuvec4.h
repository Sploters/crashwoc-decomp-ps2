#ifndef NUMATH_NUVEC4_H
#define NUMATH_NUVEC4_H

#include "numath/numathtypes.h"

void NuVec4Add(struct nuvec4_s *dst, struct nuvec4_s *a, struct nuvec4_s *b);
float NuVec4Dot(struct nuvec4_s *a, struct nuvec4_s *b);
void NuVec4Sub(struct nuvec4_s *dst, struct nuvec4_s *a, struct nuvec4_s *b);
float NuVec4Mag(struct nuvec4_s *v);
float NuVec4MagSqr(struct nuvec4_s *v);
void NuVec4Scale(struct nuvec4_s *dst, struct nuvec4_s *src, float scale);
void NuVec4ScaleAccum(struct nuvec4_s *dst, struct nuvec4_s *src, float scale);
void NuVec4MtxTransform(struct nuvec4_s *dst, struct nuvec4_s *src, struct numtx_s *m);
void NuVec4MtxTransformH(struct nuvec4_s *dst, struct nuvec4_s *src, struct numtx_s *m);
void NuVec4Lerp(struct nuvec4_s *dst, struct nuvec4_s *a, struct nuvec4_s *b, float t);

#endif /* NUMATH_NUVEC4_H */
