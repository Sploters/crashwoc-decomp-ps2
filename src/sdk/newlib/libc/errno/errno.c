/*
 * Unit: sdk/newlib/libc/errno/errno
 *
 * Functions:
 *   0x002703d0 __errno
 */

struct _reent {
    int _errno;
};

extern struct _reent *_impure_ptr[];

int *__errno(void)
{
    return &_impure_ptr[0]->_errno;
}
