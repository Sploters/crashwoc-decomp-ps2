/*
 * Unit: sdk/sce/sdr_main
 *
 * Functions:
 *   0x0026d008 sceSdRemoteInit
 *   0x0026d0b0 sceSdTransToIOP
 *   0x0026d138 sceSdCallBack
 *   0x0026d148 sceSdRemote
 */

typedef int (*sceSdSpu2IntrHandler)(int core, void *common);

extern sceSdSpu2IntrHandler D_00612418[];
#define sd_callback (D_00612418[0])

/* Install the SPU2 interrupt callback; returns the previous one. */
sceSdSpu2IntrHandler sceSdCallBack(sceSdSpu2IntrHandler func)
{
    sceSdSpu2IntrHandler old;

    old = sd_callback;
    sd_callback = func;
    return old;
}
