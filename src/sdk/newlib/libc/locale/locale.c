/*
 * Unit: sdk/newlib/libc/locale/locale
 *
 * Functions:
 *   0x00276f78 _setlocale_r
 *   0x00277000 _localeconv_r
 *   0x00277010 setlocale
 *   0x00277040 localeconv
 */

struct _reent;

struct lconv;

#define _REENT (_impure_ptr[0])

extern struct _reent *_impure_ptr[];

extern char *_setlocale_r(struct _reent *p, int category, const char *locale);

/* The "C" locale's conventions (retail-owned, no Data-from-C yet). */
extern struct lconv D_0062C408;
#define c_lconv D_0062C408

struct lconv *_localeconv_r(struct _reent *data)
{
    return &c_lconv;
}

char *setlocale(int category, const char *locale)
{
    return _setlocale_r(_REENT, category, locale);
}
