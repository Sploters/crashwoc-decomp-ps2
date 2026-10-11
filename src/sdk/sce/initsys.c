/*
 * Unit: sdk/sce/initsys
 *
 * Functions:
 *   0x00280bb0 supplement_crt0
 *   0x00280bf8 _InitSys
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
s32 InitAlarm();
s32 InitThread();

void _InitSys(void) {
    supplement_crt0();
    InitAlarm();
    InitThread();
}
