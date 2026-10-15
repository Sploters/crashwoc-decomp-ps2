#ifndef NU3D_NUNODE_H
#define NU3D_NUNODE_H

#include "nu_types.h"
#include "numath/numathtypes.h"

struct nugobj_s;

enum nunodetype_e {
    NUNODETYPE_NODE,
    NUNODETYPE_MESH,
    NUNODETYPE_TRANSFORM,
    NUNODETYPE_JOINT,
    NUNODETYPE_SKIN,
    NUNODETYPE_SPLINE
};

struct nunode_s {
    char *name;                         /* 0x00 */
    int id;                             /* 0x04 */
    enum nunodetype_e type;             /* 0x08 */
    int num_children;                   /* 0x0C */
    struct nunode_s *parent;            /* 0x10 */
    struct nunode_s *child;             /* 0x14 */
    struct nunode_s *next;              /* 0x18 */
    void *data;                         /* 0x1C */
}; /* 0x20 */

struct nujointnode_s {
    struct nunode_s node;               /* 0x000 */
    struct numtx_s orient;              /* 0x020 */
    struct numtx_s mtx;                 /* 0x060 */
    struct numtx_s wmtx;                /* 0x0A0 */
    char pad0E0[0x18];                  /* 0x0E0 */
    struct nuvec_s scale;               /* 0x0F8 */
    struct nuvec_s pivot;               /* 0x104 */
    u8 flags;                           /* 0x110 */
}; /* 0x114 */

struct nuskinnode_s {
    struct nunode_s node;               /* 0x00 */
    struct nugobj_s *geometry;          /* 0x20 */
    char pad24[0x04];                   /* 0x24 */
    int num_joints;                     /* 0x28 */
    union {
        struct numtx_s **joints;        /* 0x2C once NuNodeResolveSkinNodes ran */
        int *joint_ids;                 /* 0x2C node ids, as read from the file */
    };
    struct numtx_s *joint_mtxs;         /* 0x30 */
}; /* 0x34 */

#endif /* NU3D_NUNODE_H */
