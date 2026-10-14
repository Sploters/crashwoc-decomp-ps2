/*
 * Unit: sdk/sce/initsys
 *
 * Functions:
 *   0x00280bb0 supplement_crt0
 *   0x00280bf8 _InitSys
 */

extern void supplement_crt0(void);
extern void InitAlarm(void);
extern int InitThread(void);

void _InitSys(void)
{
    supplement_crt0();
    InitAlarm();
    InitThread();
}
