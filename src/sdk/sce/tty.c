/*
 * Unit: sdk/sce/tty
 *
 * Functions:
 *   0x00279d88 QueueInit
 *   0x00279db0 QueuePeekWriteDone
 *   0x00279df0 QueuePeekReadDone
 *   0x00279e30 sceTtyHandler
 *   0x00279fc8 sceTtyWrite
 *   0x0027a118 sceTtyRead
 *   0x0027a1e8 sceTtyInit
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


void QueuePeekReadDone(void *arg0)
{
  s32 *new_var;
  void *temp_a0;
  s32 new_var2;
  new_var2 = *((s32 *) (((s8 *) arg0) + 4));
  *((void **) (((s8 *) arg0) + 8)) = (temp_a0 = (*((void **) (((s8 *) arg0) + 8))) + 1);
  *((s32 *) (((s8 *) arg0) + 4)) = (s32) (new_var2 - 1);
  new_var = &(*((s32 *) (((s8 *) arg0) + 0)));
  if (temp_a0 == (arg0 + ((*new_var) + 0x10)))
  {
    *((void **) (((s8 *) arg0) + 8)) = (void *) (arg0 + 0x10);
  }
}
