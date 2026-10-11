/*
 * Unit: sdk/newlib/libc/stdlib/mprec
 *
 * Functions:
 *   0x002773d8 _Balloc
 *   0x00277480 _Bfree
 *   0x002774b0 _multadd
 *   0x002775c8 _s2b
 *   0x00277710 _hi0bits
 *   0x00277798 _lo0bits
 *   0x00277858 _i2b
 *   0x00277890 _multiply
 *   0x00277ac0 _pow5mult
 *   0x00277bc0 _lshift
 *   0x00277d38 __mcmp
 *   0x00277da0 __mdiff
 *   0x00277f30 _ulp
 *   0x00277fc8 _b2d
 *   0x00278148 _d2b
 *   0x002782c8 _ratio
 *   0x00278388 _mprec_log10
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


void _Bfree(void *arg0, void *arg1) {
    void **temp_v0;

    if (arg1 != ((void *)0)) {
        temp_v0 = ((*( s32 *)((s8 *)(arg1) + ( 4))) * 4) + (*( s32 *)((s8 *)(arg0) + ( 0x4C)));
        (*( void **)((s8 *)(arg1) + ( 0))) = (void *) *temp_v0;
        *temp_v0 = arg1;
    }
}
