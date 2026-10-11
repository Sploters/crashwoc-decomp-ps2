/*
 * Unit: game/visidat
 *
 * Functions:
 *   0x0024d0b0 visiSetSplineKnot
 *   0x0024d238 visiLoadData
 *   0x0024dc20 visiReset
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


void visiReset(void *arg0)
{
  *((s32 *) (((s8 *) arg0) + 0x10)) = (*((s32 *) (((s8 *) arg0) + 0xC)) = -1);
}
