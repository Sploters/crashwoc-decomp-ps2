#ifndef NUMATH_NUPATCH_H
#define NUMATH_NUPATCH_H

#include "numath/numathtypes.h"

struct nuvec_s *NuBezierCubicPatchEvaluatePartials(struct nuvec_s *out, struct nuvec_s *cp, int uorder, int vorder, float u, float v);
void NuBezierCubicPatchPartialsUUVV(struct nuvec_s *cp, struct nuvec_s *d00, struct nuvec_s *d01, struct nuvec_s *d10, struct nuvec_s *d11);
void NuBezierCubicPatchPartialsUUVVV(struct nuvec_s *cp, struct nuvec_s *d00, struct nuvec_s *d01, struct nuvec_s *d10, struct nuvec_s *d11);
void NuBezierCubicPatchPartialsUUV(struct nuvec_s *cp, struct nuvec_s *d00, struct nuvec_s *d01, struct nuvec_s *d10, struct nuvec_s *d11);
void NuBezierCubicPatchPartialsUVV(struct nuvec_s *cp, struct nuvec_s *d00, struct nuvec_s *d01, struct nuvec_s *d10, struct nuvec_s *d11);
struct nuvec_s *NuBezierCubicPatchEvaluate(struct nuvec_s *out, struct nuvec_s *cp, float u, float v);
void NuBezierCubicPatchPartialsU(struct nuvec_s *cp, struct nuvec_s *du00, struct nuvec_s *du01, struct nuvec_s *du10, struct nuvec_s *du11);
void NuBezierCubicPatchPartialsV(struct nuvec_s *cp, struct nuvec_s *dv00, struct nuvec_s *dv01, struct nuvec_s *dv10, struct nuvec_s *dv11);
void NuBezierCubicPatchPartialsUUU(struct nuvec_s *cp, struct nuvec_s *d00, struct nuvec_s *d01, struct nuvec_s *d10, struct nuvec_s *d11);
void NuBezierCubicPatchPartialsVVV(struct nuvec_s *cp, struct nuvec_s *d00, struct nuvec_s *d01, struct nuvec_s *d10, struct nuvec_s *d11);
void NuBezierCubicPatchPartialsUU(struct nuvec_s *cp, struct nuvec_s *d00, struct nuvec_s *d01, struct nuvec_s *d10, struct nuvec_s *d11);
void NuBezierCubicPatchPartialsVV(struct nuvec_s *cp, struct nuvec_s *d00, struct nuvec_s *d01, struct nuvec_s *d10, struct nuvec_s *d11);
struct nuvec_s *NuBezierQuadraticTriangleEvaluateBarycentric(struct nuvec_s *out, struct nuvec_s *cp, float u, float v, float w);
struct nuvec_s *NuBezierQuadraticTriangleEvaluateParametric(struct nuvec_s *out, struct nuvec_s *cp, float u, float v);
struct nuvec_s *NuBezierQuadraticTrianglePartialsUU(struct nuvec_s *out, struct nuvec_s *cp);
struct nuvec_s *NuBezierQuadraticTrianglePartialsVV(struct nuvec_s *out, struct nuvec_s *cp);
struct nuvec_s *NuBezierQuadraticTrianglePartialsUeV(struct nuvec_s *out, struct nuvec_s *cp);
void NuCubicToBez3(float *cubic, float *bez);
void NuBez3ToCubic(float *bez, float *cubic);
void NuBez3Subdiv(float *bez, float *left, float *right, float t);
float NuBez3EvaluateX(struct nuvec4_s *bez, float t);

#endif /* NUMATH_NUPATCH_H */
