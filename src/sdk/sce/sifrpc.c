/*
 * Unit: sdk/sce/sifrpc
 *
 * Functions:
 *   0x0027b368 sceSifInitRpc
 *   0x0027b508 sceSifExitRpc
 *   0x0027b530 _sceRpcGetPacket
 *   0x0027b5d8 _sceRpcFreePacket
 *   0x0027b5f8 _sceRpcGetFPacket
 *   0x0027b628 _sceRpcGetFPacket2
 *   0x0027b668 _request_end
 *   0x0027b720 _request_rdata
 *   0x0027b780 sceSifGetOtherData
 *   0x0027b8d8 _search_svdata
 *   0x0027b928 _request_bind
 *   0x0027b9d8 sceSifBindRpc
 *   0x0027bb18 _request_call
 *   0x0027bba8 sceSifCallRpc
 *   0x0027bd98 sceSifCheckStatRpc
 *   0x0027bdd8 sceSifSetRpcQueue
 *   0x0027be70 sceSifRegisterRpc
 *   0x0027bf40 sceSifRemoveRpc
 *   0x0027bfd8 sceSifRemoveRpcQueue
 *   0x0027c068 sceSifGetNextRequest
 *   0x0027c0c0 sceSifExecRequest
 *   0x0027c288 sceSifRpcLoop
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


void _sceRpcFreePacket(void *arg0) {
    (*( s32 *)((s8 *)(arg0) + ( 0x18))) = 0;
    (*( s32 *)((s8 *)(arg0) + ( 0x10))) = (s32) ((*( s32 *)((s8 *)(arg0) + ( 0x10))) & 0xFFFFFFFE);
}





s32 _sceRpcGetFPacket(void *arg0)
{
  s32 new_var;
  s32 temp_hi;
  temp_hi = ((s32) (*((s32 *) (((s8 *) arg0) + 0x24)))) % ((s32) (*((s32 *) (((s8 *) arg0) + 0x18))));
  new_var = temp_hi << 6;
  *((s32 *) (((s8 *) arg0) + 0x24)) = (s32) (temp_hi + 1);
  return (*((s32 *) (((s8 *) arg0) + 0x14))) + new_var;
}



s32 DIntr(void);
s32 EIntr(void);

void *sceSifRemoveRpc(void *arg0, void *arg1)
{
  void *temp_v0;
  void *var_s0;
  void *var_v1;
  DIntr();
  var_s0 = *((void **) (((s8 *) arg1) + 8));
  if (var_s0 == arg0)
  {
    *((void **) (((s8 *) arg1) + 8)) = (void *) (*((void **) (((s8 *) var_s0) + 0x38)));
  }
  else
  {
 do { if (var_s0 != ((void *) 0)) { var_v1 = *((void **) (((s8 *) var_s0) + 0x38)); if (var_v1 != arg0) { loop_5: var_s0 = var_v1; if (var_s0 != ((void *) 0)) { temp_v0 = *((void **) (((s8 *) var_s0) + 0x38)); var_v1 = temp_v0; if (temp_v0 == arg0) { goto block_7; } goto loop_5; } } else { block_7: *((s32 *) (((s8 *) var_s0) + 0x38)) = (s32) (*((s32 *) (((s8 *) arg0) + 0x38))); } } } while (0);
  }
  EIntr();
  return var_s0;
}



extern s32 D_00705140[];

void sceSifSetRpcQueue(void *arg0, s32 arg1)
{
  void *temp_v0;
  void *var_a0;
  void *var_v1;
  s32 *new_var;
  DIntr();
  *((s32 *) (((s8 *) arg0) + 0)) = arg1;
  *((s32 *) (((s8 *) arg0) + 4)) = 0;
  *((s32 *) (((s8 *) arg0) + 8)) = 0;
  *((s32 *) (((s8 *) arg0) + 0xC)) = 0;
  *((s32 *) (((s8 *) arg0) + 0x10)) = 0;
  new_var = &D_00705140[0];
  *((s32 *) (((s8 *) arg0) + 0x14)) = 0;
  if ((*((void **) (((s8 *) new_var) + 0x28))) == ((void *) 0))
  {
    *((void **) (((s8 *) new_var) + 0x28)) = arg0;
  }
  else
  {
    var_a0 = *((void **) (((s8 *) new_var) + 0x28));
    var_v1 = *((void **) (((s8 *) var_a0) + 0x14));
    if (var_v1 != ((void *) 0))
    {
      do
      {
        var_a0 = var_v1;
        temp_v0 = *((void **) (((s8 *) var_a0) + 0x14));
        var_v1 = temp_v0;
      }
      while (temp_v0 != ((void *) 0));
    }
    *((void **) (((s8 *) var_a0) + 0x14)) = arg0;
  }
  EIntr();
}



s32 DIntr();
s32 EIntr();
extern s32 D_00705140[];

void *sceSifRemoveRpcQueue(void *arg0)
{
  void *temp_v0;
  void *var_s0;
  void *var_v1;
  DIntr();
  var_v1 = &D_00705140[0];
  var_s0 = *((void **) (((s8 *) var_v1) + 0x28));
  if (var_s0 == arg0)
  {
    *((void **) (((s8 *) (&D_00705140[0])) + 0x28)) = (void *) (*((void **) (((s8 *) var_s0) + 0x14)));
  }
  else
    if (var_s0 != ((void *) 0))
  {
    var_v1 = *((void **) (((s8 *) var_s0) + 0x14));
 do { if (var_v1 != arg0) { loop_5: var_s0 = var_v1; if (var_s0 != ((void *) 0)) { temp_v0 = *((void **) (((s8 *) var_s0) + 0x14)); var_v1 = temp_v0; if (temp_v0 == arg0) { goto block_7; } goto loop_5; } } else { block_7: *((s32 *) (((s8 *) var_s0) + 0x14)) = (s32) (*((s32 *) (((s8 *) arg0) + 0x14))); } } while (0);
  }
  EIntr();
  return var_s0;
}
