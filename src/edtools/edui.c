/*
 * Unit: edtools/edui
 *
 * Functions:
 *   0x001b7f00 eduiInitMaterials
 *   0x001b8158 eduicbRenderSelWithClipColour
 *   0x001b8468 eduicbRenderCheck
 *   0x001b8650 eduiItemTextPickCreate
 *   0x001b87f0 eduiAddTextPickEnt
 *   0x001b8aa8 eduicbProcessTextPick
 *   0x001b8e38 eduicbRenderTextPick
 *   0x001b9108 eduiAddGradPickCFG
 *   0x001b9860 eduiGradStageSetHSV
 *   0x001b9a60 eduiGradStageAddRGB
 *   0x001b9cb8 eduiGradPickRead
 *   0x001b9f20 eduicbProcessGradPick
 *   0x001ba090 eduicbRenderGradPick
 *   0x001ba380 eduiItemGreyPickCreate
 *   0x001ba540 eduicbProcessColourPick
 *   0x001ba910 eduicbRenderColourPick
 *   0x001bbc50 eduicbRenderGreyPick
 *   0x001bbf10 eduicbProcessTexturePick
 *   0x001bc2d0 eduicbRenderTexturePick
 *   0x001bc8b0 eduiItemSliderCreate
 *   0x001bcac8 eduicbProcessSlider
 *   0x001bcfe8 eduicbRenderSlider
 *   0x001bd2c8 eduicbRenderSliderInt
 *   0x001bd538 eduiCreateMessageMenu
 *   0x001bd800 eduiMenuRender
 *   0x001bdba8 eduiMenuProcess
 *   0x001bde10 CreateTestMenu
 *   0x001befa8 eduicbProcessColourSlider
 *   0x001bf548 eduicbRenderColourSlider
 *   0x001bf828 eduiItemSetText
 *   0x001bf8e0 eduiItemSelCreate
 *   0x001bfa68 eduiItemSelWithClipColourCreate
 *   0x001bfc00 eduiItemCheckCreate
 *   0x001bfdb0 eduiItemToggleCreate
 *   0x001bff60 eduiGradStageAdd
 *   0x001c0050 eduiGradStageDelete
 *   0x001c00b8 eduiItemGradPickCreate
 *   0x001c0228 eduiItemGreyGradPickCreate
 *   0x001c03a8 eduiItemColourPickSetHSV
 *   0x001c03d0 eduiItemColourPickSetRGB
 *   0x001c0560 eduiItemColourPickCreate
 *   0x001c06f8 eduiItemTexturePickCreate
 *   0x001c0890 eduiItemSliderSetVal
 *   0x001c08e0 eduiItemSliderSetValEx
 *   0x001c0938 eduiItemSliderSetFmt
 *   0x001c09f0 eduiItemSliderSetGranularity
 *   0x001c09f8 eduiItemSliderCreateInt
 *   0x001c0b00 eduiItemNumberCreate
 *   0x001c0b38 eduiItemColourSliderCreate
 *   0x001c0d80 eduiMenuDestroy
 *   0x001c0de8 eduiMenuCreate
 *   0x001c0ed0 eduiMenuAttach
 *   0x001c0f18 eduiMenuDetach
 *   0x001c0f68 eduiMenuAddItem
 *   0x001c0fa0 eduiMenuHighlight
 *   0x001c1020 eduiMenuFitWidth
 *   0x001c1138 eduiInit
 *   0x001c1158 eduicbProcessGreyPick
 *   0x001c1238 eduicbProcessSel
 *   0x001c12f0 eduicbRenderSel
 *   0x001c1450 eduicbItemSliderDestroy
 *   0x001c14c8 eduicbRenderNumber
 *   0x001c15d0 eduicbItemDestroy
 *   0x001c1638 eduicbItemGradPickDestroy
 *   0x001c16d8 tpentOnEnter
 *   0x001c1780 cbgpcfgAdd
 *   0x001c1888 cbgpcfgDel
 *   0x001c1930 cbgpcfgCopy
 *   0x001c1970 cbgpcfgPaste
 *   0x001c1a48 cbgpcfgCPPress
 *   0x001c1b00 eduicbPickCFGCancel
 *   0x001c1b68 eduicbCancelMessageMenu
 *   0x001c1bd8 eduiItemRender
 *   0x001c1c70 cbSel
 *   0x001c1c80 cbSubMenu
 *   0x001c1cc0 cbGradChange
 *   0x001c1cc8 TestMenu
 */

typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;
typedef float f32;

#define EDUI_ITEM_SEL      0
#define EDUI_ITEM_CHECK    2
#define EDUI_ITEM_TOGGLE   3
#define EDUI_ITEM_GRADPICK 7

typedef struct eduigradstage_s {
    struct eduigradstage_s *next;
    struct eduigradstage_s *prev;
    f32 pos;
    u8 unk_0x0C[0x4];
    f32 h;
    f32 s;
    f32 v;
} EDUIGRADSTAGE;

typedef struct nupad_s {
    u8 unk_0x000[0x55C];
    u32 paddata;
    u8 unk_0x560[0x4];
    u32 buttons;
    u8 unk_0x568[0x22];
    u8 l2_pressure;
    u8 r2_pressure;
} NUPAD;

struct eduimenu_s;

typedef struct eduiitem_s {
    struct eduiitem_s *next;
    struct eduiitem_s *prev;
    u32 type;
    void *data;
    u8 on;
    u8 unk_0x11[0x3];
    s32 group;
    u8 unk_0x18[0x8];
    char *text;
    u8 unk_0x24[0x10];
    s32 (*press)(struct eduimenu_s *menu, struct eduiitem_s *item, u32 paddata,
                 u32 buttons);
    s32 (*process)(struct eduimenu_s *menu, struct eduiitem_s *item,
                   NUPAD *pad, f32 dt);
    s32 (*render)(struct eduimenu_s *menu, struct eduiitem_s *item, s32 x,
                  s32 y, s32 w, s32 highlight);
    void (*destroy)(struct eduimenu_s *menu, struct eduiitem_s *item);
    union {
        struct {
            void (*select)(struct eduimenu_s *menu, struct eduiitem_s *item,
                           u32 buttons);
        } sel;
        struct {
            char text[32];
        } textpick;
        struct {
            f32 hue;
            f32 brightness;
            f32 h;
            f32 s;
            f32 v;
            u8 unk_0x58[0xC];
            void (*change)(struct eduimenu_s *menu, struct eduiitem_s *item,
                           u32 buttons);
        } colour;
        struct {
            EDUIGRADSTAGE *first;
            EDUIGRADSTAGE *stage;
            void (*change)(struct eduimenu_s *menu, struct eduiitem_s *item,
                           u32 paddata);
            s32 flash;
        } grad;
        struct {
            void (*change)(struct eduimenu_s *menu, struct eduiitem_s *item,
                           s32 buttons);
            f32 frac;
            f32 val;
            f32 min;
            f32 range;
            s32 flash;
            char *fmt;
            f32 granularity;
        } slider;
    } u;
} EDUIITEM;

typedef struct eduimenu_s {
    EDUIITEM *head;
    EDUIITEM *tail;
    EDUIITEM *current;
    EDUIITEM *cursor;
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    void *font;
    void (*cancel)(struct eduimenu_s *menu, struct eduimenu_s *parent);
    char *title;
    struct eduimenu_s *child;
    struct eduimenu_s *parent;
    char titlebuf[0];
} EDUIMENU;

extern EDUIITEM *edui_last_item;
extern EDUIMENU *edui_messagemenu;
extern char D_0061A7A0[];
extern f32 D_0062F8FC;
extern f32 D_0062F900;
extern f32 D_0062F904;
extern f32 D_0062F908;
extern s32 D_0062F970;
extern s32 D_0062F974;
extern void *D_0062F978;
extern s32 D_00633250;
extern EDUIMENU *D_00633254;
extern void *D_006B40C0[6];

#define gpcfg_clip_h    D_0062F8FC
#define gpcfg_clip_s    D_0062F900
#define gpcfg_clip_v    D_0062F904
#define gpcfg_clip_grey D_0062F908
#define edui_up_repeat   D_0062F970
#define edui_down_repeat D_0062F974
#define edui_test_sel   D_0062F978
#define edui_test_menu  D_00633254
#define edui_item_width D_00633250
#define edui_mtl        D_006B40C0
#define edui_srcfile    D_0061A7A0

extern s32 NuStrLen(char *str);
extern void NuStrCpy(char *dst, char *src);
extern void *NuMemAllocFn(s32 size, char *file, s32 line);
extern void NuMemFreeFn(void *ptr, char *file, s32 line);
extern void *memset(void *dst, s32 c, u32 n);
extern void eduiInitMaterials(void);
extern void NuRndrLineRect2di(s32 x, s32 y, s32 w, s32 h, u32 colour,
                              void *mtl);
s32 eduiMenuProcess(EDUIMENU *menu, f32 dt, NUPAD *pad);
extern void eduiMenuRender(EDUIMENU *menu);
extern EDUIITEM *eduiItemSliderCreate(void *data, void *style, s32 group,
                                      void *cb, f32 val, f32 min, f32 max,
                                      char *text);
void eduiAddGradPickCFG(EDUIMENU *menu, EDUIITEM *pick);
void eduiGradStageSetHSV(EDUIGRADSTAGE *stage, f32 h, f32 s, f32 v);
void eduiGradStageDelete(EDUIITEM *pick, EDUIGRADSTAGE *stage);
s32 eduicbRenderNumber(EDUIMENU *menu, EDUIITEM *item, s32 x, s32 y, s32 w,
                       s32 highlight);


static inline void eduiRGBToHSV(f32 r, f32 g, f32 b, f32 *h, f32 *s, f32 *v) {
    f32 max;
    f32 min;
    f32 delta;

    max = (r > ((g > b) ? g : b)) ? r : ((g > b) ? g : b);
    min = (r < ((g < b) ? g : b)) ? r : ((g < b) ? g : b);
    *v = max;
    *s = (max != 0.0f) ? (max - min) / max : 0.0f;
    if (*s == 0.0f) {
        *h = 0.0f;
        return;
    }
    delta = max - min;
    if (r == max) {
        *h = (g - b) / delta;
    } else if (g == max) {
        *h = 2.0f + (b - r) / delta;
    } else if (b == max) {
        *h = 4.0f + (r - g) / delta;
    }
    *h *= 60.0f;
    if (*h < 0.0f) {
        *h += 360.0f;
    }
}


s32 eduicbProcessGradPick(EDUIMENU *menu, EDUIITEM *item, NUPAD *pad) {
    EDUIGRADSTAGE *stage;
    EDUIGRADSTAGE *step;
    f32 min;
    f32 max;
    f32 pos;

    if (item->u.grad.change) {
        item->u.grad.change(menu, item, pad->paddata);
    }
    if (pad->buttons & 0x40) {
        eduiAddGradPickCFG(menu, item);
        return 0;
    }
    if (!item->u.grad.stage) {
        item->u.grad.stage = item->u.grad.first;
        if (!item->u.grad.stage) {
            return 0;
        }
    }
    if (pad->buttons & 0x8000) {
        step = item->u.grad.stage->prev;
    change:
        if (step) {
            item->u.grad.stage = step;
        }
        return 0;
    }
    if (pad->buttons & 0x2000) {
        step = item->u.grad.stage->next;
        goto change;
    }
    stage = item->u.grad.stage;
    min = 0.0f;
    max = 1.0f;
    if (stage->prev) {
        min = stage->prev->pos;
    }
    if (stage->next) {
        max = stage->next->pos;
    }
    if (pad->l2_pressure) {
        pos = stage->pos - pad->l2_pressure / 15360.0f;
        if (pos < min) {
            pos = min;
        }
    } else if (pad->r2_pressure) {
        pos = stage->pos + pad->r2_pressure / 15360.0f;
        if (pos > max) {
            pos = max;
        }
    } else {
        goto done;
    }
    stage->pos = pos;
    item->u.grad.flash = 60;
    return 0;
done:
    return 0;
}


s32 eduiItemSetText(EDUIITEM *item, char *text) {
    if (item->text) {
        if (NuStrLen(item->text) < NuStrLen(text)) {
            NuMemFreeFn(item->text, edui_srcfile, 0x24F);
            item->text = 0;
        }
    }
    if (!item->text) {
        item->text = NuMemAllocFn(NuStrLen(text) + 1, edui_srcfile, 0x254);
        if (!item->text) {
            return 0;
        }
    }
    NuStrCpy(item->text, text);
    return 1;
}


EDUIGRADSTAGE *eduiGradStageAdd(EDUIITEM *pick, f32 pos, f32 h, f32 s, f32 v) {
    EDUIGRADSTAGE *stage;
    EDUIGRADSTAGE *prev;
    EDUIGRADSTAGE *next;

    stage = NuMemAllocFn(sizeof(EDUIGRADSTAGE), edui_srcfile, 0x521);
    if (stage) {
        memset(stage, 0, sizeof(EDUIGRADSTAGE));
        prev = 0;
        for (next = pick->u.grad.first; next; next = next->next) {
            if (pos < next->pos) {
                break;
            }
            prev = next;
        }
        if (prev) {
            prev->next = stage;
        } else {
            pick->u.grad.first = stage;
        }
        if (next) {
            next->prev = stage;
        }
        stage->pos = pos;
        stage->prev = prev;
        stage->next = next;
        eduiGradStageSetHSV(stage, h, s, v);
        pick->u.grad.stage = stage;
    }
    return stage;
}


void eduiItemColourPickSetHSV(EDUIITEM *item, f32 h, f32 s, f32 v) {
    item->u.colour.h = h;
    item->u.colour.s = s;
    item->u.colour.v = v;
    item->u.colour.hue = h / 360.0f;
    item->u.colour.brightness = v;
}


void eduiItemColourPickSetRGB(EDUIITEM *item, f32 r, f32 g, f32 b) {
    eduiRGBToHSV(r, g, b, &item->u.colour.h, &item->u.colour.s,
                 &item->u.colour.v);
    item->u.colour.hue = item->u.colour.h / 360.0f;
    item->u.colour.brightness = item->u.colour.v;
}


void eduiItemSliderSetVal(EDUIITEM *item, f32 val) {
    item->u.slider.flash = 60;
    item->u.slider.val = val;
    item->u.slider.frac = (val - item->u.slider.min) / item->u.slider.range;
    if (item->u.slider.change) {
        item->u.slider.change(0, item, 0);
    }
}


void eduiItemSliderSetValEx(EDUIITEM *item, f32 val, s32 flash, s32 notify) {
    item->u.slider.val = val;
    item->u.slider.frac = (val - item->u.slider.min) / item->u.slider.range;
    if (flash) {
        item->u.slider.flash = 60;
    }
    if (notify && item->u.slider.change) {
        item->u.slider.change(0, item, 0);
    }
}


void eduiItemSliderSetFmt(EDUIITEM *item, char *fmt) {
    if (item->u.slider.fmt) {
        if (NuStrLen(item->u.slider.fmt) < NuStrLen(fmt)) {
            NuMemFreeFn(item->u.slider.fmt, edui_srcfile, 0x833);
            item->u.slider.fmt = NuMemAllocFn(NuStrLen(fmt) + 1, edui_srcfile,
                                              0x834);
        }
    } else {
        item->u.slider.fmt = NuMemAllocFn(NuStrLen(fmt) + 1, edui_srcfile,
                                          0x838);
    }
    NuStrCpy(item->u.slider.fmt, fmt);
}


void eduiItemSliderSetGranularity(EDUIITEM *item, f32 granularity) {
    item->u.slider.granularity = granularity;
}


EDUIITEM *eduiItemNumberCreate(void *data, void *style, s32 group, void *cb,
                               f32 val, f32 min, f32 max, char *text) {
    EDUIITEM *item;

    item = eduiItemSliderCreate(data, style, group, cb, val, min, max, text);
    if (item) {
        item->render = eduicbRenderNumber;
    }
    return item;
}


inline void eduiMenuDestroy(EDUIMENU *menu) {
    EDUIITEM *item;
    EDUIITEM *next;

    while (menu->head) {
        item = menu->head;
        next = item->next;
        item->destroy(menu, item);
        menu->head = next;
    }
    NuMemFreeFn(menu, edui_srcfile, 0x192);
}


EDUIMENU *eduiMenuCreate(s32 x, s32 y, s32 w, s32 h, void *font,
                         void (*cancel)(EDUIMENU *menu, EDUIMENU *parent),
                         char *title) {
    EDUIMENU *menu;
    s32 size;

    size = sizeof(EDUIMENU);
    if (title) {
        size = NuStrLen(title) + sizeof(EDUIMENU) + 1;
    }
    menu = NuMemAllocFn(size, edui_srcfile, 0x1A1);
    if (menu) {
        memset(menu, 0, size);
        menu->x = x;
        menu->y = y;
        menu->w = w;
        menu->h = h;
        menu->font = font;
        menu->cancel = cancel;
        menu->title = menu->titlebuf;
        if (title) {
            NuStrCpy(menu->title, title);
        }
    }
    return menu;
}


s32 eduiMenuAttach(EDUIMENU *menu, EDUIMENU *child) {
    if (!child->parent) {
        while (menu->child) {
            menu = menu->child;
        }
        child->parent = menu;
        menu->child = child;
        return 1;
    }
    return 0;
}


inline s32 eduiMenuDetach(EDUIMENU *menu) {
    if (menu->parent) {
        if (menu->child) {
            eduiMenuDetach(menu->child);
        }
        menu->parent->child = 0;
        menu->parent = 0;
        return 1;
    }
    return 0;
}


s32 eduiMenuProcess(EDUIMENU *menu, f32 dt, NUPAD *pad) {
    EDUIMENU *parent;
    s32 up;
    s32 down;
    s32 upfast;
    s32 downfast;

    if (!pad) {
        return 0;
    }
    if (menu->child) {
        eduiMenuProcess(menu->child, dt, pad);
        return 0;
    }
    if (!menu->current) {
        menu->current = menu->head;
        if (!menu->current) {
            return 0;
        }
    }
    if (menu->current->press && pad->paddata) {
        if (menu->current->press(menu, menu->current, pad->paddata,
                                 pad->buttons)) {
            return 0;
        }
    }
    if (menu->current->process(menu, menu->current, pad, dt)) {
        return 0;
    }
    downfast = 0;
    upfast = 0;
    down = 0;
    up = 0;
    if (pad->buttons & 0x1000) {
        edui_up_repeat = 20;
        up = 1;
    }
    if ((pad->paddata & 0x1000) && edui_up_repeat) {
        if (--edui_up_repeat == 0) {
            edui_up_repeat = 10;
            up = 1;
        }
    }
    if (pad->buttons & 0x4000) {
        edui_down_repeat = 20;
        down = 1;
    }
    if ((pad->paddata & 0x4000) && edui_down_repeat) {
        if (--edui_down_repeat == 0) {
            edui_down_repeat = 10;
            down = 1;
        }
    }
    if (up) {
        upfast = pad->paddata & 1;
    }
    if (down) {
        downfast = pad->paddata & 1;
    }
    if (up && menu->current->prev) {
        do {
            if (menu->current == menu->cursor) {
                menu->cursor = menu->current->prev;
            }
            menu->current = menu->current->prev;
            if (!menu->current->prev) {
                return 0;
            }
        } while (upfast);
        return 0;
    }
    if (down && menu->current->next) {
        do {
            menu->current = menu->current->next;
            if (!menu->current->next) {
                return 0;
            }
        } while (downfast);
        return 0;
    }
    if (pad->buttons & 0x10) {
        parent = 0;
        if (menu->parent) {
            parent = menu->parent;
            eduiMenuDetach(menu);
        }
        if (menu->cancel) {
            menu->cancel(menu, parent);
        }
    }
    return 0;
}


EDUIITEM *eduiMenuAddItem(EDUIMENU *menu, EDUIITEM *item) {
    if (menu->tail) {
        menu->tail->next = item;
    } else {
        menu->head = item;
    }
    item->prev = menu->tail;
    item->next = 0;
    menu->tail = item;
    menu->cursor = menu->head;
    edui_last_item = item;
    return item;
}


inline void eduiMenuHighlight(EDUIMENU *menu, EDUIITEM *item) {
    EDUIITEM *scan;

    if (!item->group) {
        return;
    }
    for (scan = menu->head; scan; scan = scan->next) {
        if (scan == item) {
            switch (scan->type) {
            case EDUI_ITEM_SEL:
            case EDUI_ITEM_CHECK:
            default:
                scan->on = 1;
                break;
            case EDUI_ITEM_TOGGLE:
                scan->on = 1 - scan->on;
                break;
            }
        } else if (scan->group == item->group) {
            scan->on = 0;
        }
    }
}


void eduiInit(void) {
    eduiInitMaterials();
}


s32 eduicbProcessGreyPick(EDUIMENU *menu, EDUIITEM *item, NUPAD *pad) {
    f32 v;

    if (pad->l2_pressure) {
        v = item->u.colour.v - pad->l2_pressure / 4192.0f;
        if (v < 0.0f) {
            v = 0.0f;
        }
        item->u.colour.v = v;
        return 1;
    }
    if (pad->r2_pressure) {
        v = item->u.colour.v + pad->r2_pressure / 4192.0f;
        if (v > 1.0f) {
            v = 1.0f;
        }
        item->u.colour.v = v;
        return 1;
    }
    item->u.colour.h = item->u.colour.s = 0.0f;
    if ((pad->buttons & 0x40) && item->u.colour.change) {
        item->u.colour.change(menu, item, pad->buttons);
    }
    return 0;
}


s32 eduicbProcessSel(EDUIMENU *menu, EDUIITEM *item, NUPAD *pad) {
    if (!(pad->buttons & 0x40)) {
        return 0;
    }
    eduiMenuHighlight(menu, item);
    if (!item->u.sel.select) {
        return 0;
    }
    item->u.sel.select(menu, item, pad->buttons);
    return 0;
}


inline void eduicbItemDestroy(EDUIMENU *menu, EDUIITEM *item) {
    if (item->text) {
        NuMemFreeFn(item->text, edui_srcfile, 0x180);
    }
    NuMemFreeFn(item, edui_srcfile, 0x182);
    menu->cursor = menu->head;
}


void eduicbItemSliderDestroy(EDUIMENU *menu, EDUIITEM *item) {
    if (item->u.slider.fmt) {
        NuMemFreeFn(item->u.slider.fmt, edui_srcfile, 0x84F);
    }
    eduicbItemDestroy(menu, item);
}


void cbgpcfgDel(EDUIMENU *menu, EDUIITEM *item) {
    EDUIITEM *pick;

    pick = item->data;
    if (pick->u.grad.stage) {
        eduiGradStageDelete(pick, pick->u.grad.stage);
    }
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}


inline void eduiGradStageDelete(EDUIITEM *pick, EDUIGRADSTAGE *stage) {
    if (stage->prev) {
        stage->prev->next = stage->next;
    } else {
        pick->u.grad.first = stage->next;
    }
    if (stage->next) {
        stage->next->prev = stage->prev;
    }
    if (stage->next) {
        pick->u.grad.stage = stage->next;
    } else {
        pick->u.grad.stage = stage->prev;
    }
    NuMemFreeFn(stage, edui_srcfile, 0x55B);
}


void eduicbItemGradPickDestroy(EDUIMENU *menu, EDUIITEM *item) {
    while (item->u.grad.first) {
        eduiGradStageDelete(item, item->u.grad.first);
    }
    NuMemFreeFn(item, edui_srcfile, 0x58C);
}


void tpentOnEnter(EDUIMENU *menu, EDUIITEM *item) {
    EDUIITEM *pick;

    pick = item->data;
    NuStrCpy(pick->u.textpick.text, item->u.textpick.text);
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}


void cbgpcfgCopy(EDUIMENU *menu, EDUIITEM *item) {
    EDUIITEM *pick;
    EDUIGRADSTAGE *stage;

    pick = item->data;
    stage = pick->u.grad.stage;
    if (stage) {
        if (item->type == EDUI_ITEM_GRADPICK) {
            gpcfg_clip_h = stage->h;
            gpcfg_clip_s = stage->s;
            gpcfg_clip_v = stage->v;
        } else {
            gpcfg_clip_grey = stage->v;
        }
    }
}


void cbgpcfgPaste(EDUIMENU *menu, EDUIITEM *item) {
    EDUIITEM *pick;

    pick = item->data;
    if (pick->u.grad.stage) {
        if (item->type == EDUI_ITEM_GRADPICK) {
            eduiGradStageSetHSV(pick->u.grad.stage, gpcfg_clip_h, gpcfg_clip_s,
                                gpcfg_clip_v);
        } else {
            eduiGradStageSetHSV(pick->u.grad.stage, 0.0f, 0.0f,
                                gpcfg_clip_grey);
        }
    }
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}


void cbgpcfgCPPress(EDUIMENU *menu, EDUIITEM *item) {
    EDUIITEM *pick;

    pick = item->data;
    if (pick->u.grad.stage) {
        eduiGradStageSetHSV(pick->u.grad.stage, item->u.colour.h,
                            item->u.colour.s, item->u.colour.v);
    }
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}


void eduicbPickCFGCancel(EDUIMENU *menu, EDUIITEM *item) {
    eduiMenuDestroy(menu);
}


void eduicbCancelMessageMenu(EDUIMENU *menu, EDUIITEM *item) {
    eduiMenuDestroy(edui_messagemenu);
    edui_messagemenu = 0;
}


s32 eduiItemRender(EDUIITEM *item, s32 x, s32 y, s32 w, s32 highlight) {
    s32 height;

    edui_item_width = w;
    height = item->render(0, item, x, y, w, highlight);
    if (highlight) {
        NuRndrLineRect2di(x * 16, y * 8, edui_item_width * 16, height * 8,
                          0x80FFFFFF, edui_mtl[0]);
    }
    return height;
}


void cbSel(EDUIMENU *menu, EDUIITEM *item) {
    edui_test_sel = item->data;
}


void cbSubMenu(EDUIMENU *menu, EDUIITEM *item) {
    EDUIMENU *submenu;

    submenu = item->data;
    if (!submenu->parent) {
        while (menu->child) {
            menu = menu->child;
        }
        submenu->parent = menu;
        menu->child = submenu;
    }
}


void cbGradChange(EDUIMENU *menu, EDUIITEM *item) {
}


void TestMenu(void *pad) {
    eduiMenuProcess(edui_test_menu, 1.0f / 60.0f, pad);
    eduiMenuRender(edui_test_menu);
}
