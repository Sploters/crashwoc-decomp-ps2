/*
 * Unit: sdk/sce/iopreset
 *
 * Functions:
 *   0x002808a0 sceSifResetIop
 *   0x002809e0 sceSifIsAliveIop
 *   0x00280a08 sceSifSyncIop
 *   0x00280a40 sceSifRebootIop
 */

#define SIF_REG_SMFLAG   4
#define SIF_STAT_SIFINIT 0x10000

extern int sceSifGetReg(unsigned int reg);

int sceSifIsAliveIop(void)
{
    if (sceSifGetReg(SIF_REG_SMFLAG) & SIF_STAT_SIFINIT)
        return 1;
    return 0;
}
