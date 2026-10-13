/*
 * Unit: nucore/numemhi
 *
 * Functions:
 *   0x00105688 NuAllocHighInit
 *   0x00105708 NuAllocHigh
 *   0x001057b8 NuFreeHigh
 */

typedef void (*NuErrorFunc)(const char *fmt, ...);
extern NuErrorFunc NuErrorProlog(const char *file, int line);

/* Header placed at the start of the high-memory region. */
struct nuhighmem_s {
    struct nuhighmem_s *base;       /* 0x0 */
    int size;                       /* 0x4 */
};

/* Retail-owned (no Data-from-C yet). */
extern char D_00613CC8[];               /* source file name */
extern char D_00613CF0[];               /* error message */
extern struct nuhighmem_s *D_0062EA28;
#define numemhi_file D_00613CC8
#define reinit_msg D_00613CF0
#define highmem D_0062EA28

void NuAllocHighInit(struct nuhighmem_s *base, int size)
{
    if (highmem != 0) {
        NuErrorProlog(numemhi_file, 0x2F)(reinit_msg);
    }
    /* Align the region to 16 bytes. Retail masks with 0xF rather than ~0xF. */
    if ((int)base & 0xF) {
        size -= 0xF;
        base = (struct nuhighmem_s *)(((int)base + 0xF) & 0xF);
    }
    if (size & 0xF) {
        size = (size - 0xF) & 0xF;
    }
    highmem = base;
    base->size = size;
    base->base = base;
}
