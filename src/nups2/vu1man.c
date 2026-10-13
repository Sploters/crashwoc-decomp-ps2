/*
 * Unit: nups2/vu1man
 *
 * Functions:
 *   0x0016b7a0 NuPs2InitVU
 *   0x0016b7f8 VU1_DisplayFRegs
 *   0x0016b8c8 VU1_DisplaySRegs
 *   0x0016b948 VU1_LoadXD
 */

typedef void (*nudebugmsg_fn)(char *fmt, ...);
extern nudebugmsg_fn NuDebugMsgProlog(char *file, int line);
extern int FlushCache(int operation);
/* Callers pass a second argument (as in nustream.c); the definition ignores it. */
extern void VU1_DmaWrite(void *chain, int wait);

/* Retail-owned (no Data-from-C yet). */
extern char D_00281530[];
extern char D_002834E0[];
extern char D_00616A08[];               /* source file name */
extern char D_00616A60[];               /* debug message */
#define fpatch_microcode D_00281530
#define rndr3d_l_microcode D_002834E0
#define vu1man_file D_00616A08
#define initvu_msg D_00616A60

void NuPs2InitVU(void)
{
    FlushCache(0);
    VU1_DmaWrite(fpatch_microcode, 1);
    VU1_DmaWrite(rndr3d_l_microcode, 1);
    NuDebugMsgProlog(vu1man_file, 0x38)(initvu_msg);
}

/* Reload the VU1 microcode; the same sequence as NuPs2InitVU. */
void VU1_LoadXD(void)
{
    FlushCache(0);
    VU1_DmaWrite(fpatch_microcode, 1);
    VU1_DmaWrite(rndr3d_l_microcode, 1);
    NuDebugMsgProlog(vu1man_file, 0x38)(initvu_msg);
}
