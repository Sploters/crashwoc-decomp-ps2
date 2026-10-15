#ifndef NUMATH_NUMTX_H
#define NUMATH_NUMTX_H

#include "numath/numathtypes.h"

void NuMtxSetRotateXYZ(struct numtx_s *m, struct nuangvec_s *a);
void NuMtxMul(struct numtx_s *dest, struct numtx_s *a, struct numtx_s *b);
void NuMtxMulH(struct numtx_s *dest, struct numtx_s *a, struct numtx_s *b);
void NuMtxMulR(struct numtx_s *dest, struct numtx_s *a, struct numtx_s *b);
void NuMtxInvRSS(struct numtx_s *dest, struct numtx_s *m);
void NuMtxInvH(struct numtx_s *mi, struct numtx_s *m0);
void NuMtxAlignZ(struct numtx_s *m, struct nuvec_s *v);
void NuMtxAlignX(struct numtx_s *m, struct nuvec_s *v);
void NuMtxAlignY(struct numtx_s *m, struct nuvec_s *v);
void NuMtxLookAtX(struct numtx_s *dest, struct nuvec_s *pnt);
void NuMtxLookAtY(struct numtx_s *dest, struct nuvec_s *pnt);
void NuMtxLookAtZ(struct numtx_s *dest, struct nuvec_s *pnt);
void NuMtxAddR(struct numtx_s *m, struct numtx_s *m0, struct numtx_s *m1);
void NuMtxSubR(struct numtx_s *m, struct numtx_s *m0, struct numtx_s *m1);
void NuMtxOrth(struct numtx_s *m);
void NuMtxSetZero(struct numtx_s *m);
void NuMtxSetIdentity(struct numtx_s *m);
void NuMtxTranslate(struct numtx_s *m, struct nuvec_s *v);
void NuMtxPreTranslate(struct numtx_s *m, struct nuvec_s *t);
void NuMtxScale(struct numtx_s *m, struct nuvec_s *s);
struct nuvec_s NuMtxGetScale(struct numtx_s *m);
void NuMtxPreScale(struct numtx_s *m, struct nuvec_s *s);
void NuMtxRotateX(struct numtx_s *m, int a);
void NuMtxRotateXR(float angle, struct numtx_s *m);
void NuMtxPreRotateX(struct numtx_s *m, int a);
void NuMtxRotateY(struct numtx_s *m, int a);
void NuMtxRotateYR(float angle, struct numtx_s *m);
void NuMtxPreRotateY(struct numtx_s *m, int a);
void NuMtxRotateZ(struct numtx_s *m, int a);
void NuMtxRotateZR(float angle, struct numtx_s *m);
void NuMtxPreRotateZ(struct numtx_s *m, int a);
void NuMtxSetTranslation(struct numtx_s *m, struct nuvec_s *v);
void NuMtxSetScale(struct numtx_s *m, struct nuvec_s *v);
void NuMtxSetRotationX(struct numtx_s *m, int r);
void NuMtxSetRotationY(struct numtx_s *m, int r);
void NuMtxSetRotationZ(struct numtx_s *m, int a);
void NuMtxSetRotationAxis(struct numtx_s *m, int angle, struct nuvec_s *axis);
float NuMtxDet3(struct numtx_s *m);
void NuMtxTransposeR(struct numtx_s *m, struct numtx_s *m0);
void NuMtxTranspose(struct numtx_s *m, struct numtx_s *m0);
void NuMtxInv(struct numtx_s *dest, struct numtx_s *m);
void NuMtxInvR(struct numtx_s *dest, struct numtx_s *m);
void NuMtxSkewSymmetric(struct numtx_s *m, struct nuvec_s *v);
void NuMtxGetXAxis(struct numtx_s *m, struct nuvec_s *v);
void NuMtxGetYAxis(struct numtx_s *m, struct nuvec_s *v);
void NuMtxGetZAxis(struct numtx_s *m, struct nuvec_s *v);
void NuMtxGetTranslation(struct numtx_s *m, struct nuvec_s *v);

#endif /* NUMATH_NUMTX_H */
