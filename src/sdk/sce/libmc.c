/*
 * Unit: sdk/sce/libmc
 *
 * Functions:
 *   0x00266c68 sceMcInit
 *   0x00266e20 _lmcGetClientPtr
 *   0x00266e50 sceMcChangeThreadPriority
 *   0x00266f08 sceMcGetSlotMax
 *   0x00266fc8 sceMcOpen
 *   0x00267100 sceMcMkdir
 *   0x00267138 sceMcClose
 *   0x002671f0 sceMcSeek
 *   0x002672c8 mceIntrReadFixAlign
 *   0x00267358 sceMcRead
 *   0x00267478 sceMcWrite
 *   0x002675f8 mcHearAlarm
 *   0x00267620 mcDelayThread
 *   0x00267668 sceMcSync
 *   0x00267748 mceGetInfoApdx
 *   0x002677a0 sceMcGetInfo
 *   0x00267928 sceMcGetDir
 *   0x00267a80 mceStorePwd
 *   0x00267b08 sceMcChdir
 *   0x00267c58 sceMcFormat
 *   0x00267d20 sceMcDelete
 *   0x00267e48 sceMcFlush
 *   0x00267f00 sceMcSetFileInfo
 *   0x002680d8 sceMcRename
 *   0x00268240 sceMcUnformat
 *   0x00268308 sceMcGetEntSpace
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
s32 GetThreadId(void);
s32 SetAlarm(s32, s32 *, s32);
s32 SleepThread();
extern s32 mcHearAlarm[];

void mcDelayThread(s32 arg0) {
    SetAlarm(arg0 & 0xFFFF, &mcHearAlarm[0], GetThreadId());
    SleepThread();
}



extern s32 D_00612400[];
extern unsigned long long D_00612404[];
extern s32 D_00700140[];
extern s32 D_00701700[];

s32 *_lmcGetClientPtr(s32 **arg0, s32 **arg1)
{
  *arg0 = &D_00701700[0];
  *arg1 = &D_00612400[0];
  *((s32 *) (((s8 *) (&D_00701700[0])) + 0x3C)) = (s32) D_00612404[0];
  return &D_00700140[0];
}
