/*
 * Unit: sdk/newlib/libm/common/s_matherr
 *
 * Functions:
 *   0x0026cbe0 matherr
 */

struct exception {
    int type;
    char *name;
    double arg1;
    double arg2;
    double retval;
    int err;
};

int matherr(struct exception *x)
{
    int n = 0;

    if (x->arg1 != x->arg1)
        return 0;
    return n;
}
