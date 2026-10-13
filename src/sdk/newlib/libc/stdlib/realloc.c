/*
 * Unit: sdk/newlib/libc/stdlib/realloc
 *
 * Functions:
 *   0x002710d0 realloc
 */

typedef unsigned int size_t;

struct _reent;

#define _REENT (_impure_ptr[0])

extern struct _reent *_impure_ptr[];

extern void *_realloc_r(struct _reent *ptr, void *old, size_t newsize);

void *realloc(void *ap, size_t nbytes)
{
    return _realloc_r(_REENT, ap, nbytes);
}
