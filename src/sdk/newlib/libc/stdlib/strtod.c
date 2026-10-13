/*
 * Unit: sdk/newlib/libc/stdlib/strtod
 *
 * Functions:
 *   0x00272288 _strtod_r
 *   0x00273188 strtod
 *   0x002731b8 strtodf
 */

struct _reent;

#define _REENT (_impure_ptr[0])

extern struct _reent *_impure_ptr[];

extern double _strtod_r(struct _reent *ptr, const char *s00, char **se);

double strtod(const char *s00, char **se)
{
    return _strtod_r(_REENT, s00, se);
}

float strtodf(const char *s00, char **se)
{
    return strtod(s00, se);
}
