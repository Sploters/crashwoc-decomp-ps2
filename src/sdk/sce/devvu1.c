/*
 * Unit: sdk/sce/devvu1
 *
 * Functions:
 *   0x00263938 sceVuCheckBusy
 *   0x00263968 sceVu0MemWriteQ
 *   0x002639a8 sceVu0MemReadQ
 *   0x002639f0 sceDevVu1Reset
 *   0x00263a08 sceDevVu1Pause
 *   0x00263a68 sceDevVu1Continue
 *   0x00263ab8 sceDevVu1PutDBit
 *   0x00263ae8 sceDevVu1PutTBit
 *   0x00263b18 sceDevVu1GetDBit
 *   0x00263b28 sceDevVu1GetTBit
 *   0x00263b38 sceDevVu1Sync
 *   0x00263b80 sceDevVu1Exec
 *   0x00263b90 sceDevVu1GetTpc
 *   0x00263bb8 sceDevVu1GetCnd
 *   0x00263d48 sceDevVu1PutCnd
 */

extern void sceVu0MemReadQ(void *data, unsigned int addr);

#define VU1_TPC_ADDR 0x43A /* VU1 TPC, mapped into VU0 register space */

unsigned short sceDevVu1GetTpc(void)
{
    unsigned short data[8];

    sceVu0MemReadQ(data, VU1_TPC_ADDR);
    return data[0];
}
