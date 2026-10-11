/*
 * Unit: sdk/sce/kprintf
 *
 * Functions:
 *   0x0027a2a8 kputchar
 *   0x0027a2e0 deci2Putchar
 *   0x0027a390 serialPutchar
 *   0x0027a3c8 ftoi
 *   0x0027a458 printfloat
 *   0x0027a5c0 _printf
 *   0x0027ab88 kprintf
 *   0x0027abc0 scePrintf
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


void serialPutchar(s32 arg0) {
    if (arg0 == 0xA) {
        kputchar(0xD);
        kputchar(0xA);
        return;
    }
    kputchar();
}
