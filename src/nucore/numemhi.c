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

/* High memory is a K&R-style first-fit allocator: a circular list of free
 * blocks, each starting with this 16-byte header. */
struct nuhighmem_s {
    struct nuhighmem_s *next;       /* 0x0 next free block */
    unsigned int size;              /* 0x4 in headers (16 bytes), incl. this one */
    int pad[2];
}; /* 0x10 */

/* Retail-owned (no Data-from-C yet). */
extern char D_00613CC8[];               /* source file name */
extern char D_00613CF0[];               /* error message */
extern char D_00613D18[];               /* error message */
extern struct nuhighmem_s *D_0062EA28;
#define numemhi_file D_00613CC8
#define reinit_msg D_00613CF0
#define badsize_msg D_00613D18
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
    base->next = base;
}

void *NuAllocHigh(int size)
{
    struct nuhighmem_s *p;
    struct nuhighmem_s *prev;
    unsigned int nunits;

    if (size <= 0) {
        NuErrorProlog(numemhi_file, 0x4C)(badsize_msg);
    }
    nunits = (unsigned int)(size + 0xF) / sizeof(struct nuhighmem_s) + 1;
    prev = highmem;
    for (p = prev->next;; prev = p, p = p->next) {
        if (p->size >= nunits) {
            if (p->size == nunits) {
                prev->next = p->next;
            } else {
                p->size -= nunits;
                p += p->size;
                p->size = nunits;
            }
            highmem = prev;
            return p + 1;
        }
        if (p == highmem) {
            return 0;
        }
    }
}

void NuFreeHigh(void *ptr)
{
    struct nuhighmem_s *bp;
    struct nuhighmem_s *p;

    bp = (struct nuhighmem_s *)ptr - 1;
    for (p = highmem; !(bp > p && bp < p->next); p = p->next) {
        if (p >= p->next && (bp > p || bp < p->next)) {
            break; /* freed block at start or end of arena */
        }
    }
    /* Join to the upper neighbour... */
    if (bp + bp->size == p->next) {
        bp->size += p->next->size;
        bp->next = p->next->next;
    } else {
        bp->next = p->next;
    }
    /* ...and to the lower one. */
    if (p + p->size == bp) {
        p->size += bp->size;
        p->next = bp->next;
    } else {
        p->next = bp;
    }
    highmem = p;
}
