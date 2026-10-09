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

/* Partial layouts: only the fields accessed by these helpers are known. */
struct nunode_s {
    char pad_00[0x0C];
    int num_children;              /* 0x0C */
};

struct numeshnode_s {
    char pad_00[0x20];
    void *geometry;                /* 0x20 */
};

extern int D_0062EB8C;
#define keep_names D_0062EB8C

int NuNodeNumChildren(struct nunode_s *node)
{
    return node->num_children;
}

void NuNodeKeepNames(int keep)
{
    keep_names = keep;
}

void NuMeshNodeSetGeometry(struct numeshnode_s *node, void *geometry)
{
    node->geometry = geometry;
}
