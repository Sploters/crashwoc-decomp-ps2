/*
 * Unit: sdk/sce/eecdvd
 *
 * Functions:
 *   0x00265950 CB_DelayTh
 *   0x00265978 sceCdDelayThread
 *   0x002659e0 sceCdCallback
 *   0x00265a38 _sceCd_cd_callback
 *   0x00265ad8 _Cdvd_cbLoop
 *   0x00265b98 sceCdInitEeCB
 *   0x00265c70 _sceCd_cd_read_intr
 *   0x00265d10 cmd_sem_init
 *   0x00265da8 cdvd_exit
 *   0x00265e28 sceCdPOffCallback
 *   0x00265e98 _sceCd_Poff_Intr
 *   0x00265ed8 PowerOffCB
 *   0x00265f40 sceCdSearchFile
 *   0x00266240 _sceCd_ncmd_prechk
 *   0x002663b0 sceCdNcmdDiskReady
 *   0x00266448 sceCdSync
 *   0x002664e8 sceCdSyncS
 *   0x00266558 _sceCd_scmd_prechk
 *   0x002666c8 sceCdInit
 *   0x002669a8 sceCdDiskReady
 *   0x00266ba0 sceCdMmode
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
s32 DIntr();
s32 EIntr();
extern s32 D_006109BC[];
extern s32 D_006FFE84[];
extern s32 D_006FFE88[];

s32 sceCdPOffCallback(s32 arg0, s32 arg1) {
    s32 temp_s0;

    if (D_006109BC[0] < 0) {
        PowerOffCB();
    }
    DIntr();
    temp_s0 = D_006FFE84[0];
    D_006FFE88[0] = arg1;
    D_006FFE84[0] = arg0;
    EIntr();
    return temp_s0;
}



s32 DeleteSema(s32);
s32 SignalSema(s32);
s32 sceSifRemoveCmdHandler(s32);
extern s32 D_00610994[];
extern s32 D_006109A0[];
extern s32 _sceCd_ncmd_semid[];
extern s32 _sceCd_scmd_semid[];
extern s32 sceCdCbfunc_num[];

void cdvd_exit(void)
{
  if (D_00610994[0] != 0)
  {
    sceCdCbfunc_num[0] = -1;
 do { SignalSema(D_006109A0[0]); } while (0);
  }
  DeleteSema(_sceCd_ncmd_semid[0]);
  DeleteSema(_sceCd_scmd_semid[0]);
  DeleteSema(D_006109A0[0]);
  DIntr();
  sceSifRemoveCmdHandler(0x80000012);
  EIntr();
}



s32 ChangeThreadPriority(s32 thid, s32 prio);
s32 CreateThread(struct ThreadParam *param);
s32 GetThreadId(void);
s32 ReferThreadStatus(s32, s32 *);
s32 StartThread(s32 thid, void *arg);
extern s32 D_00610994[];
extern s32 D_006FFE90[];
extern s32 D_006FFE98[];
extern s32 D_006FFEC8[];
extern s32 _Cdvd_cbLoop[];
extern s32 _gp[];

s32 sceCdInitEeCB(s32 arg0, s32 arg1, s32 arg2)
{
  s8 *new_var;
  s32 temp_v0;
  s32 var_s3;
  s32 temp_v0_2;
  var_s3 = 1;
  if (D_00610994[0] == 0)
  {
    temp_v0 = GetThreadId();
    D_006FFE90[0] = temp_v0;
    ReferThreadStatus(temp_v0, &D_006FFE98[0]);
    new_var = (s8 *) (&D_006FFEC8[0]);
    *((s32 *) (((s8 *) (&D_006FFEC8[0])) + 0xC)) = arg2;
    *((s32 **) (new_var + 0x10)) = &_gp[0];
    *((s32 **) (((s8 *) (&D_006FFEC8[0])) + 4)) = &_Cdvd_cbLoop[0];
    *((s32 *) (new_var + 8)) = arg1;
    *((s32 *) (((s8 *) (&D_006FFEC8[0])) + 0x14)) = arg0;
    temp_v0_2 = CreateThread(&D_006FFEC8[0]);
    D_00610994[0] = temp_v0_2;
    StartThread(temp_v0_2, 0);
  }
  else
  {
    ChangeThreadPriority(D_00610994[0], arg0);
    var_s3 = 0;
  }
  return var_s3;
}
