/*
 * Unit: sdk/newlib/libc/stdlib/strtol
 *
 * Functions:
 *   0x002731e0 _strtol_r
 *   0x00273418 strtol
 */

struct _reent;

#define _REENT (_impure_ptr[0])

extern struct _reent *_impure_ptr[];

extern long _strtol_r(struct _reent *rptr, const char *nptr, char **endptr, int base);

long strtol(const char *s, char **ptr, int base)
{
    return _strtol_r(_REENT, s, ptr, base);
}
