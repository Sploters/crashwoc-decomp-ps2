/*
 * Unit: numath/nupatch
 *
 * Functions:
 *   0x0010e708 NuBezierCubicPatchEvaluatePartials
 *   0x0010eab8 NuBezierCubicPatchPartialsUUVV
 *   0x0010ede8 NuBezierCubicPatchPartialsUUV
 *   0x0010f048 NuBezierCubicPatchPartialsUVV
 *   0x0010f2a8 NuBezierCubicPatchPartialsUUVVV
 *   0x0010f578 NuBezierCubicPatchEvaluate
 *   0x0010f6f0 NuBezierCubicPatchPartialsU
 *   0x0010f7e0 NuBezierCubicPatchPartialsV
 *   0x0010f8d0 NuBezierCubicPatchPartialsVV
 *   0x0010fa28 NuBezierCubicPatchPartialsVVV
 *   0x0010fb68 NuBezierCubicPatchPartialsUU
 *   0x0010fcc0 NuBezierCubicPatchPartialsUUU
 *   0x0010fe00 NuBezierQuadraticTriangleEvaluateBarycentric
 *   0x0010fed0 NuBezierQuadraticTriangleEvaluateParametric
 *   0x0010ffb0 NuBezierQuadraticTrianglePartialsUU
 *   0x00110040 NuBezierQuadraticTrianglePartialsVV
 *   0x001100c8 NuBezierQuadraticTrianglePartialsUeV
 *   0x00110150 NuCubicToBez3
 *   0x001101a8 NuBez3ToCubic
 *   0x00110210 NuBez3Subdiv
 *   0x001102b0 NuBez3EvaluateX
 */


#ifndef NULL
#define NULL ((void *)0)
#endif
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;


void NuCubicToBez3(void *arg0, void *arg1) {
    (*( f32 *)((s8 *)(arg1) + ( 0))) = (f32) (*( f32 *)((s8 *)(arg0) + ( 0xC)));
    (*( f32 *)((s8 *)(arg1) + ( 4))) = (f32) ((*( f32 *)((s8 *)(arg0) + ( 8))) / 3.0f);
    (*( f32 *)((s8 *)(arg1) + ( 8))) = (f32) (((*( f32 *)((s8 *)(arg0) + ( 4))) + (2.0f * (*( f32 *)((s8 *)(arg0) + ( 8))))) / 3.0f);
    (*( f32 *)((s8 *)(arg1) + ( 0x10))) = (f32) (((*( f32 *)((s8 *)(arg0) + ( 0))) + (*( f32 *)((s8 *)(arg0) + ( 4))) + (*( f32 *)((s8 *)(arg0) + ( 8)))) - (*( f32 *)((s8 *)(arg0) + ( 0xC))));
}





void NuBez3ToCubic(void *arg0, void *arg1) {
    (*( f32 *)((s8 *)(arg1) + ( 0))) = (f32) ((((*( f32 *)((s8 *)(arg0) + ( 0))) + ((*( f32 *)((s8 *)(arg0) + ( 4))) * 3.0f)) - ((*( f32 *)((s8 *)(arg0) + ( 8))) * 3.0f)) + (*( f32 *)((s8 *)(arg0) + ( 0xC))));
    (*( f32 *)((s8 *)(arg1) + ( 4))) = (f32) (((*( f32 *)((s8 *)(arg0) + ( 8))) * 3.0f) - ((*( f32 *)((s8 *)(arg0) + ( 4))) * 6.0f));
    (*( f32 *)((s8 *)(arg1) + ( 0))) = (f32) ((*( f32 *)((s8 *)(arg0) + ( 4))) * 3.0f);
    (*( f32 *)((s8 *)(arg1) + ( 0))) = (f32) (*( f32 *)((s8 *)(arg0) + ( 0)));
}
