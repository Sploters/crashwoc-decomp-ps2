/*
 * Unit: sdk/sce/libdma
 *
 * Functions:
 *   0x00262a58 memclr
 *   0x00262a90 sceDmaGetChan
 *   0x00262ab8 sceDmaReset
 *   0x00262b98 sceDmaDebug
 *   0x00262ba8 sceDmaPutEnv
 *   0x00262d80 sceDmaGetEnv
 *   0x00262db8 sceDmaPutStallAddr
 *   0x00262de0 sceDmaSend
 *   0x00262eb8 sceDmaSendN
 *   0x00262fa0 sceDmaSendI
 *   0x00263088 sceDmaRecv
 *   0x00263150 sceDmaRecvN
 *   0x00263240 sceDmaRecvI
 *   0x00263330 sceDmaSync
 *   0x002633f8 sceDmaWatch
 *   0x002634c8 sceDmaPause
 *   0x00263510 sceDmaRestart
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


s32 sceDmaRestart(u32 *arg0, u32 arg1) {
    u32 temp_v0;

    temp_v0 = *arg0;
    *arg0 = arg1;
    return (temp_v0 >> 8) & 1;
}





void memclr(s8 *arg0, s32 arg1) {
    s32 var_v0;
    s8 *var_a0;

    var_a0 = arg0;
    var_v0 = arg1 - 1;
    if (arg1 != 0) {
        do {
            *var_a0 = 0;
            var_v0 -= 1;
            var_a0 += 1;
        } while (var_v0 != -1);
    }
}



extern s32 sceDmaDebugMode[];

s32 sceDmaDebug(s32 arg0) {
    s32 temp_v0;

    temp_v0 = sceDmaDebugMode[0];
    sceDmaDebugMode[0] = arg0;
    return temp_v0;
}
