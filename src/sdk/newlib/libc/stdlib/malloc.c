/*
 * Unit: sdk/newlib/libc/stdlib/malloc
 *
 * Functions:
 *   0x002703e0 malloc
 *   0x00270408 free
 */

typedef unsigned int size_t;

struct _reent;

#define _REENT (_impure_ptr[0])

extern struct _reent *_impure_ptr[];

extern void *_malloc_r(struct _reent *ptr, size_t nbytes);

void *malloc(size_t nbytes)
{
    return _malloc_r(_REENT, nbytes);
}
