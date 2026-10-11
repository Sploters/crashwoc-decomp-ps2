/*
 * Unit: sdk/sce/filestub
 *
 * Functions:
 *   0x0027c2c8 _sceFsIobSemaMK
 *   0x0027c328 new_iob
 *   0x0027c3b0 get_iob
 *   0x0027c420 _sceFs_Rcv_Intr
 *   0x0027c7e0 _sceFsSemInit
 *   0x0027c830 _sceFsWaitS
 *   0x0027c860 _sceFsSigSema
 *   0x0027c870 sceFsInit
 *   0x0027ca10 _fs_version
 *   0x0027caa0 sceFsReset
 *   0x0027cad8 sceOpen
 *   0x0027cd60 sceClose
 *   0x0027cee0 sceLseek
 *   0x0027d118 sceRead
 *   0x0027d388 sceWrite
 *   0x0027d648 sceIoctl
 *   0x0027d998 sceIoctl2
 *   0x0027db70 _sceCallCode
 *   0x0027dd18 sceRemove
 *   0x0027dd38 sceMkdir
 *   0x0027dee8 sceRmdir
 *   0x0027df08 sceFormat
 *   0x0027e178 sceAddDrv
 *   0x0027e298 sceDelDrv
 *   0x0027e2b8 sceDopen
 *   0x0027e358 sceDclose
 *   0x0027e4c0 sceDread
 *   0x0027e618 sceGetstat
 *   0x0027e7b8 sceChstat
 *   0x0027e9f8 sceRename
 *   0x0027ebe8 sceChdir
 *   0x0027ec08 sceSync
 *   0x0027eda0 sceMount
 *   0x0027f010 sceUmount
 *   0x0027f030 sceLseek64
 *   0x0027f268 sceDevctl
 *   0x0027f498 sceSymlink
 *   0x0027f678 sceReadlink
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
extern s32 D_00613400[];
extern s32 D_007064A8[];

int sceFsReset(void)
{
  int new_var;
  D_00613400[0] = 0;
  new_var = 4;
  memset(&D_007064A8[0], 0, new_var);
  return 0;
}



s32 SignalSema(s32);
s32 WaitSema(s32);
extern s32 D_00613408[];
extern char D_00706280[];

void *get_iob(u32 arg0)
{
  s32 *new_var;
  s32 *new_var2;
  _sceFsIobSemaMK();
  new_var2 = D_00613408;
  WaitSema(D_00613408[0]);
  if (arg0 >= 0x20U)
  {
    SignalSema(D_00613408[0]);
 do { } while (0); return (void *) 0;
  }
  new_var = (arg0 * 0x10) + (&D_00706280[0]);
  SignalSema(new_var2[0]);
  return new_var;
}
