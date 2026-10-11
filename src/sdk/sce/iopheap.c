/*
 * Unit: sdk/sce/iopheap
 *
 * Functions:
 *   0x0027f840 sceSifInitIopHeap
 *   0x0027f8c8 sceSifAllocIopHeap
 *   0x0027f938 sceSifAllocSysMemory
 *   0x0027f9b8 sceSifFreeSysMemory
 *   0x0027fa30 sceSifFreeIopHeap
 *   0x0027fa50 sceSifLoadIopHeap
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


void sceSifFreeIopHeap(void)
{
 do { sceSifFreeSysMemory(); } while (0);
}
