/*
 * Unit: gamelib/edgra
 *
 * Functions:
 *   0x001b5e70 edgraInitAllClumps
 *   0x001b6180 edgracbInstanceMenu
 *   0x001b6318 edgracbClumpPropertiesMenu
 *   0x001b64b8 edgraDetermineNearestClump
 *   0x001b65f8 edgraClumpCreate
 *   0x001b66f8 edgraDoInput
 *   0x001b6a30 edgraDrawCursor
 *   0x001b6d60 edgraFileSave
 *   0x001b6eb0 edgraFileLoad
 *   0x001b7060 edgraRender
 *   0x001b7190 edgraInit
 *   0x001b73f0 edgraClumpsReset
 *   0x001b7418 edgracbSetInstanceType
 *   0x001b7460 edgracbSetClumpWind
 *   0x001b74a8 edgracbSetClumpHeight
 *   0x001b74f0 edgracbToggleClumpReactive
 *   0x001b7520 edgracbCancelOptMenu
 *   0x001b7528 edgracbCancelInstanceMenu
 *   0x001b7550 edgracbCancelClumpPropertiesMenu
 *   0x001b7578 edgraClose
 *   0x001b7598 edgraEnter
 *   0x001b75b8 edgraClumpPlace
 *   0x001b7608 edgraClumpDestroy
 *   0x001b7638 edgraProc
 *   0x001b7698 edgracbFileSave
 *   0x001b7770 edgracbFileLoad
 */


struct nuvec_s {
    float x;
    float y;
    float z;
};

struct edgraitem_s {
    char pad00[0x0C];
    int value;
    unsigned char toggle;
    char pad11[0x4C - 0x11];
    float fvalue;
};

struct grassclump_s {
    int type;
    int size;
    struct nuvec_s pos;
    float radius;
    float wind;
    int reactive;
    float height;
};

extern struct grassclump_s GrassClumps[64];
extern void *edgra_active_menu;
extern void *edgra_options_menu;
extern void *edgra_instance_menu;
extern void *edgra_clumpproperties_menu;
extern float edgra_radius;
extern int edgra_nearest;
extern int edgra_instance_type;
extern int edgra_next_clump;
extern int edgra_clump_size;

extern void eduiMenuDestroy(void *menu);
extern void eduiMenuDetach(void *menu);
extern void edgraInitAllClumps(void);


void edgraClumpsReset(void)
{
    int i;

    for (i = 0; i < 64; i++) {
        GrassClumps[i].size = 0;
    }
    edgra_next_clump = 0;
}


void edgracbSetInstanceType(void *menu, struct edgraitem_s *item)
{
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
    edgra_instance_menu = 0;
    edgra_instance_type = item->value;
    edgra_active_menu = 0;
}


void edgracbSetClumpWind(void *menu, struct edgraitem_s *item)
{
    if (edgra_nearest != -1) {
        GrassClumps[edgra_nearest].wind = item->fvalue;
    }
    edgraInitAllClumps();
}


void edgracbSetClumpHeight(void *menu, struct edgraitem_s *item)
{
    if (edgra_nearest != -1) {
        GrassClumps[edgra_nearest].height = item->fvalue;
    }
    edgraInitAllClumps();
}


void edgracbToggleClumpReactive(void *menu, struct edgraitem_s *item)
{
    if (edgra_nearest != -1) {
        GrassClumps[edgra_nearest].reactive = item->toggle;
    }
}


void edgracbCancelOptMenu(void)
{
    edgra_active_menu = 0;
}


void edgracbCancelInstanceMenu(void)
{
    eduiMenuDestroy(edgra_instance_menu);
    edgra_instance_menu = 0;
}


void edgracbCancelClumpPropertiesMenu(void)
{
    eduiMenuDestroy(edgra_clumpproperties_menu);
    edgra_clumpproperties_menu = 0;
}


void edgraClose(void)
{
    eduiMenuDestroy(edgra_options_menu);
}


void edgraEnter(void)
{
    edgra_nearest = -1;
    edgra_radius = 0.5f;
    edgra_clump_size = 64;
}


void edgraClumpPlace(int clump, struct nuvec_s *pos)
{
    GrassClumps[clump].pos = *pos;
    GrassClumps[clump].radius = edgra_radius;
    edgraInitAllClumps();
}


void edgraClumpDestroy(int clump)
{
    GrassClumps[clump].size = 0;
    edgraInitAllClumps();
}
