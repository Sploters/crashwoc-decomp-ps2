#ifndef NUMATH_NUMATHTYPES_H
#define NUMATH_NUMATHTYPES_H

struct nuvec_s {
    float x; /* 0x0 */
    float y; /* 0x4 */
    float z; /* 0x8 */
}; /* 0xC */

struct nuvec4_s {
    float x; /* 0x0 */
    float y; /* 0x4 */
    float z; /* 0x8 */
    float w; /* 0xC */
}; /* 0x10 */

struct nuangvec_s {
    int x; /* 0x0 */
    int y; /* 0x4 */
    int z; /* 0x8 */
}; /* 0xC */

struct nuquat_s {
    float x; /* 0x0 */
    float y; /* 0x4 */
    float z; /* 0x8 */
    float w; /* 0xC */
}; /* 0x10 */

/* Row-major; row 3 (_30.._32) is the translation. */
struct numtx_s {
    float _00, _01, _02, _03; /* 0x00 */
    float _10, _11, _12, _13; /* 0x10 */
    float _20, _21, _22, _23; /* 0x20 */
    float _30, _31, _32, _33; /* 0x30 */
}; /* 0x40 */

/* ax + by + cz + d = 0 */
struct nuplane_s {
    float a; /* 0x0 */
    float b; /* 0x4 */
    float c; /* 0x8 */
    float d; /* 0xC */
}; /* 0x10 */

struct nucolour3_s {
    float r; /* 0x0 */
    float g; /* 0x4 */
    float b; /* 0x8 */
}; /* 0xC */

struct nucolour_s {
    unsigned int r : 8;
    unsigned int g : 8;
    unsigned int b : 8;
    unsigned int a : 8;
}; /* 0x4 */

#endif /* NUMATH_NUMATHTYPES_H */
