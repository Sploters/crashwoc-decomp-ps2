/*
 * Unit: nu3d/nunode
 *
 * Functions:
 *   0x001165b0 NuNodeDestroyChildren
 *   0x001166f8 NuNodeGraphInit
 *   0x001168c0 NuMtxEqual
 *   0x00116a40 NuNodeCreate
 *   0x00116ab8 NuNodeDestroy
 *   0x00116ba8 NuNodeSetName
 *   0x00116c50 NuNodeDestroyHierarchy
 *   0x00116d88 NuNodePreAddChild
 *   0x00116e08 NuNodeAddChild
 *   0x00116eb0 NuNodeRemoveChild
 *   0x00116f20 NuNodeNumChildren
 *   0x00116f28 NuNodeFindByName
 *   0x00116fa8 NuNodeFindByID
 *   0x00117018 NuNodeIterator
 *   0x001170b8 NuNodeDestroyHierarchyData
 *   0x00117118 NuNodeNumBranchNodesOfType
 *   0x00117180 NuNodeNumSubNodes
 *   0x001171d8 NuNodeUpdateXfms
 *   0x001173f8 NuMeshNodeCreate
 *   0x00117488 NuMeshNodeDestroy
 *   0x001174b0 NuTransformNodeCreate
 *   0x001175b8 NuTransformNodeDestroy
 *   0x001175e0 NuJointNodeCreate
 *   0x00117710 NuJointNodeDestroy
 *   0x00117738 NuSkinNodeCreate
 *   0x00117800 NuSkinNodeDestroy
 *   0x00117870 NuSplineNodeCreate
 *   0x00117938 NuSplineNodeDestroy
 *   0x001179a0 NuNodeStrCpy
 *   0x00117a28 NuNodeStrToUpper
 *   0x00117aa0 NuNodeStrCat
 *   0x00117b18 NuNodeKeepNames
 *   0x00117b20 resolveReferenceNodes
 *   0x00117be8 NuNodeResolveReferenceNodes
 *   0x00117ca8 NuMeshNodeSetGeometry
 *   0x00117cb0 resolveSkinNodes
 *   0x00117de8 NuNodeResolveSkinNodes
 */

typedef enum nunodetype_e {
    NUNODETYPE_NODE,
    NUNODETYPE_MESH,
    NUNODETYPE_TRANSFORM,
    NUNODETYPE_JOINT,
    NUNODETYPE_SKIN,
    NUNODETYPE_SPLINE
} NuNodeType;

typedef struct nunode_s {
    char *name;
    int id;
    NuNodeType type;
    int num_children;
    struct nunode_s *parent;
    struct nunode_s *child;
    struct nunode_s *next;
    void *data;
} NuNode;

struct numeshnode_s {
    NuNode node;
    void *geometry;
};

typedef struct nuskinnode_s {
    NuNode node;
    char pad_20[8];
    int num_joints;
    void *joints;
    void *joint_mtxs;
} NuSkinNode;


typedef void (*NuErrorFunc)(const char *fmt, ...);
typedef void (*NuNodeDataFunc)(void *data);

extern NuErrorFunc NuErrorProlog(const char *file, int line);
extern void *NuMemAllocFn(int size, char *file, int line);
extern void NuMemFreeFn(void *ptr, char *file, int line);
extern void *memset(void *s, int c, unsigned int n);
extern int strcmp(const char *a, const char *b);

extern char D_006141E0[];
#define nunode_file D_006141E0

extern char D_00614248[];
#define bad_parent_type_msg D_00614248

extern int D_0062EB88;
#define next_node_id D_0062EB88

extern int D_0062EB8C;
#define keep_names D_0062EB8C


NuNode *NuNodeCreate(void)
{
    NuNode *node;

    node = NuMemAllocFn(sizeof(NuNode), nunode_file, 0xDC);
    memset(node, 0, sizeof(NuNode));
    node->type = NUNODETYPE_NODE;
    node->id = next_node_id++;
    node->num_children = 0;
    node->child = 0;
    node->next = 0;
    node->parent = 0;
    node->name = 0;
    node->data = 0;
    return node;
}


void NuNodePreAddChild(NuNode *parent, NuNode *child)
{
    NuNode *first;

    if (parent != 0 && child != 0) {
        switch (parent->type) {
        default:
            NuErrorProlog(nunode_file, 0x23F)(bad_parent_type_msg);
            break;
        case NUNODETYPE_NODE:
        case NUNODETYPE_TRANSFORM:
        case NUNODETYPE_JOINT:
            child->parent = parent;
            first = parent->child;
            parent->child = child;
            child->next = first;
            parent->num_children++;
            break;
        }
    }
}


void NuNodeRemoveChild(NuNode *parent, NuNode *child)
{
    NuNode *node;

    if (parent != 0 && child != 0) {
        node = parent->child;
        if (node == child) {
            parent->child = child->next;
            parent->num_children--;
        } else {
            do {
                node = node->next;
            } while (node != child && node != 0);
            if (node == 0) {
                return;
            }
            child->next = node->next;
            parent->num_children--;
        }
        child->parent = 0;
    }
}


int NuNodeNumChildren(struct nunode_s *node)
{
    return node->num_children;
}


NuNode *NuNodeFindByName(NuNode *node, char *name)
{
    NuNode *child;
    NuNode *found;

    child = node->child;
    if (node->name != 0 && strcmp(node->name, name) == 0) {
        return node;
    }
    for (; child != 0; child = child->next) {
        found = NuNodeFindByName(child, name);
        if (found != 0) {
            return found;
        }
    }
    return 0;
}


NuNode *NuNodeFindByID(NuNode *node, int id)
{
    NuNode *child;
    NuNode *found;

    child = node->child;
    if (node->id == id) {
        return node;
    }
    for (; child != 0; child = child->next) {
        found = NuNodeFindByID(child, id);
        if (found != 0) {
            return found;
        }
    }
    return 0;
}


void NuNodeDestroyHierarchyData(NuNode *node, NuNodeDataFunc destroy)
{
    NuNode *child;

    if (node->data != 0) {
        destroy(node->data);
    }
    for (child = node->child; child != 0; child = child->next) {
        NuNodeDestroyHierarchyData(child, destroy);
    }
}


int NuNodeNumBranchNodesOfType(NuNode *node, NuNodeType type)
{
    NuNode *child;
    int count;

    count = node->type == type;
    for (child = node->child; child != 0; child = child->next) {
        count += NuNodeNumBranchNodesOfType(child, type);
    }
    return count;
}


int NuNodeNumSubNodes(NuNode *node)
{
    NuNode *child;
    int count;

    count = 1;
    if (node->num_children != 0) {
        for (child = node->child; child != 0; child = child->next) {
            count += NuNodeNumSubNodes(child);
        }
    }
    return count;
}


void NuMeshNodeDestroy(NuNode *node)
{
    NuMemFreeFn(node, nunode_file, 0x377);
}


void NuTransformNodeDestroy(NuNode *node)
{
    NuMemFreeFn(node, nunode_file, 0x2EB);
}


void NuJointNodeDestroy(NuNode *node)
{
    NuMemFreeFn(node, nunode_file, 0x31A);
}


void NuSkinNodeDestroy(NuSkinNode *skin)
{
    if (skin->num_joints != 0) {
        NuMemFreeFn(skin->joints, nunode_file, 0x346);
        NuMemFreeFn(skin->joint_mtxs, nunode_file, 0x347);
    }
    NuMemFreeFn(skin, nunode_file, 0x349);
}


void NuNodeKeepNames(int keep)
{
    keep_names = keep;
}


void NuMeshNodeSetGeometry(struct numeshnode_s *node, void *geometry)
{
    node->geometry = geometry;
}
