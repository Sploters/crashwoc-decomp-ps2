/*
 * Unit: sdk/sce/glue
 *
 * Functions:
 *   0x00279110 sceResetttyinit
 *   0x00279120 VSync
 *   0x00279160 VSync2
 *   0x002791c0 write
 *   0x00279240 read
 *   0x002792b8 open
 *   0x002792e0 close
 *   0x002792e8 ioctl
 *   0x002792f0 lseek
 *   0x002792f8 sbrk
 *   0x002793a8 isatty
 *   0x002793b0 fstat
 *   0x002793c8 getpid
 *   0x002793d0 kill
 *   0x002793f8 stat
 *   0x00279420 unlink
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


s32 ioctl(void) {
    return -1;
}





s32 close(void) {
    return -1;
}





s32 isatty(void) {
    return 1;
}





s32 lseek(void) {
    return -1;
}





s32 getpid(void) {
    return 1;
}



s32 *__errno();

s32 open(void) {
    *__errno() = 5;
    return -1;
}





s32 stat(void) {
    *__errno() = 5;
    return -1;
}





s32 unlink(void) {
    *__errno() = 5;
    return -1;
}



extern s32 D_00612BB8[];

void sceResetttyinit(void) {
    D_00612BB8[0] = 0;
}



s32 Exit(s32);

s32 kill(s32 arg0, s32 arg1) {
    if (arg0 == 1) {
        Exit(arg1);
    }
    return 0;
}
