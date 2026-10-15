#ifndef NUMATH_NUQUAT_H
#define NUMATH_NUQUAT_H

#include "numath/numathtypes.h"

void NuMtxToQuat(struct numtx_s *m, struct nuquat_s *q);
void NuQuatSlerp(struct nuquat_s *dst, struct nuquat_s *a, struct nuquat_s *b, float t);
void NuQuatToMtx(struct nuquat_s *q, struct numtx_s *m);
void NuQuatAdd(struct nuquat_s *dst, struct nuquat_s *a, struct nuquat_s *b);
void NuQuatSub(struct nuquat_s *dst, struct nuquat_s *a, struct nuquat_s *b);
void NuQuatMul(struct nuquat_s *dst, struct nuquat_s *a, struct nuquat_s *b);
void NuQuatInv(struct nuquat_s *dst, struct nuquat_s *q);
void NuQuatNormalise(struct nuquat_s *dst, struct nuquat_s *q);
float NuQuatMagnitude(struct nuquat_s *q);
void NuQuatLerp(struct nuquat_s *dst, struct nuquat_s *a, struct nuquat_s *b, float t);

#endif /* NUMATH_NUQUAT_H */
