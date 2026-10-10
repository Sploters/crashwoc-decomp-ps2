/*
 * Unit: nu3d/nutexanm
 *
 * Functions:
 *   0x00159090 NuTexAnimProgAssembleEnd
 *   0x00159188 pftaXDef
 *   0x001592a8 pftaGoto
 *   0x001593d8 pftaXRef
 *   0x00159508 pftaBtex
 *   0x00159760 pftaGosub
 *   0x00159890 pftaUntiltex
 *   0x001599d0 NuTexAnimProgReadScript
 *   0x00159c78 NuTexAnimEnvProc
 *   0x0015a4a8 NuTexAnimProgSysInit
 *   0x0015a4f8 NuTexAnimProgRelease
 *   0x0015a500 NuTexAnimProgFind
 *   0x0015a548 NuTexAnimProgDestroy
 *   0x0015a5a8 NuTexAnimProgCreate
 *   0x0015a690 NuTexAnimProgWrite
 *   0x0015a6e8 NuTexAnimProgRead
 *   0x0015a7e8 NuTexAnimEnvDestroy
 *   0x0015a818 NuTexAnimEnvCreate
 *   0x0015a8f0 NuTexAnimEnvReset
 *   0x0015a920 NuTexAnimRestart
 *   0x0015a988 NuTexAnimSetSignals
 *   0x0015a9b0 NuTexAnimProcessList
 *   0x0015a9f8 NuTexAnimFind
 *   0x0015aa50 NuTexAnimDestroy
 *   0x0015aac0 NuTexAnimCreate
 *   0x0015ac30 NuTexAnimAddList
 *   0x0015ac88 NuTexAnimRemoveList
 *   0x0015ad20 NuTexAnimProcess
 *   0x0015ad90 NuTexAnimProgInit
 *   0x0015adf0 pftaTex
 *   0x0015ae50 pftaTexAdj
 *   0x0015af18 pftaWait
 *   0x0015afb0 pftaRate
 *   0x0015b048 pftaOn
 *   0x0015b090 pftaOff
 *   0x0015b0d8 pftaLabel
 *   0x0015b1e0 pftaRet
 *   0x0015b208 pftaRepeat
 *   0x0015b2a8 pftaRepend
 *   0x0015b2d0 pftaEnd
 *   0x0015b2f8 pftaScriptname
 *   0x0015b330 NuTexAnimResetList
 */

typedef struct NuFPar {
    int handle;
    char buffer[0x1000];
    char line[0x101];
    char word[0x101];
} NuFPar;

struct numtl_s;

enum {
    NTA_TEX = 0,
    NTA_TEXADJ = 1,
    NTA_WAIT = 2,
    NTA_RATE = 5,
    NTA_GOTO = 7,
    NTA_GOSUB = 8,
    NTA_BTEX = 9,
    NTA_RET = 10,
    NTA_REPEAT = 11,
    NTA_REPEND = 12,
    NTA_UNTILTEX = 13,
    NTA_END = 14,
    NTA_XREF = 15
};

typedef struct NuTexAnimProg {
    struct NuTexAnimProg *next;
    struct NuTexAnimProg *prev;
    char name[32];
    int on_pc[32];
    int off_pc[32];
    unsigned int on_mask;
    unsigned int off_mask;
    short xdef_label[32];
    short xdef_pc[32];
    int num_xdefs;
    short num_codes;
    int dynalloc : 1;
    short code[2];
} NuTexAnimProg;

typedef struct NuTexAnimEnv {
    NuTexAnimProg *prog;
    int pc;
    int repeat_count[16];
    int repeat_pc[16];
    int repeat_depth;
    int call_stack[16];
    int call_depth;
    int wait_base;
    int wait_random;
    int wait;
    struct numtl_s *mtl;
    short *tids;
    int tex;
    unsigned int dynalloc : 1;
} NuTexAnimEnv;

typedef struct NuTexAnim {
    struct NuTexAnim *next;
    struct NuTexAnim *prev;
    short *tids;
    short num_tids;
    int dynalloc : 1;
    struct numtl_s *mtl;
    NuTexAnimEnv *env;
    char *name;
} NuTexAnim;

typedef struct NuTexAnimList {
    NuTexAnim *anims;
    struct NuTexAnimList *next;
    struct NuTexAnimList *prev;
} NuTexAnimList;


extern int nta_labels[64];
extern NuTexAnimList ntalsysbuff[64];
extern NuTexAnimList *ntal_first;
extern NuTexAnimList *ntal_free;

extern NuTexAnimProg *D_0062EF30;
extern int D_0063318C;
extern NuTexAnimProg *D_00633190;
extern unsigned int D_00633194;
extern unsigned int D_00633198;
extern unsigned int D_0063319C;
extern char D_00615900[];

#define ntap_first D_0062EF30
#define nta_numxdefs D_0063318C
#define nta_prog D_00633190
#define nta_signals_on D_00633194
#define nta_signals_off D_00633198
#define nta_signals D_0063319C
#define nutexanm_srcfile D_00615900

extern void NuMemFreeFn(void *ptr, char *file, int line);
extern int NuFileOpen(char *name, int mode);
extern int NuFileWrite(int fh, void *data, int size);
extern void NuFileClose(int fh);
extern int NuFParGetWord(NuFPar *parser);
extern int NuFParGetInt(NuFPar *parser);
extern void NuDisableVBlank(void);
extern void NuEnableVBlank(void);
extern void NuTexAnimEnvProc(NuTexAnimEnv *env);
extern int strcasecmp(char *a, char *b);
extern char *strcpy(char *dst, const char *src);


void NuTexAnimProgAssembleEnd(NuTexAnimProg *prog)
{
    short i;

    for (i = 0; i < prog->num_codes;) {
        switch (prog->code[i]) {
        case NTA_TEXADJ:
            i += 4;
            break;
        case NTA_WAIT:
            i += 3;
            break;
        case NTA_RATE:
            i += 3;
            break;
        case NTA_BTEX:
            prog->code[i + 3] = nta_labels[prog->code[i + 3]];
            i += 4;
            break;
        case NTA_GOTO:
        case NTA_GOSUB:
            prog->code[i + 1] = nta_labels[prog->code[i + 1]];
            i += 2;
            break;
        case NTA_RET:
            i += 1;
            break;
        case NTA_REPEAT:
            i += 3;
            break;
        case NTA_REPEND:
            i += 1;
            break;
        case NTA_UNTILTEX:
            i += 3;
            break;
        case NTA_END:
            i += 1;
            break;
        case NTA_TEX:
        case NTA_XREF:
            i += 2;
            break;
        }
    }
}


void NuTexAnimProgSysInit(void)
{
    int i;

    ntap_first = 0;
    for (i = 0; i < 63; i++) {
        ntalsysbuff[i].next = &ntalsysbuff[i + 1];
    }
    ntal_first = 0;
    ntalsysbuff[63].next = 0;
    ntal_free = ntalsysbuff;
    nta_numxdefs = 0;
    nta_signals = 0;
    nta_signals_off = 0;
    nta_signals_on = 0;
}


void NuTexAnimProgRelease(void)
{
    ntap_first = 0;
}


NuTexAnimProg *NuTexAnimProgFind(char *name)
{
    NuTexAnimProg *prog;

    for (prog = ntap_first; prog; prog = prog->next) {
        if (!strcasecmp(name, prog->name)) {
            break;
        }
    }
    return prog;
}


void NuTexAnimProgDestroy(NuTexAnimProg *prog)
{
    if (prog->prev) {
        prog->prev->next = prog->next;
    } else {
        ntap_first = prog->next;
    }
    if (prog->next) {
        prog->next->prev = prog->prev;
    }
    if (prog->dynalloc) {
        NuMemFreeFn(prog, nutexanm_srcfile, 0xAD);
    }
}


void NuTexAnimProgWrite(char *name, NuTexAnimProg *prog)
{
    int fh;

    fh = NuFileOpen(name, 1);
    if (fh) {
        NuFileWrite(fh, prog, sizeof(NuTexAnimProg) + prog->num_codes * sizeof(short));
        NuFileClose(fh);
    }
}


inline void NuTexAnimEnvDestroy(NuTexAnimEnv *env)
{
    if (env->dynalloc) {
        NuMemFreeFn(env, nutexanm_srcfile, 0x161);
    }
}


inline void NuTexAnimEnvReset(NuTexAnimEnv *env)
{
    if (env) {
        env->pc = 0;
        env->repeat_depth = 0;
        env->call_depth = 0;
        env->wait_base = 0;
        env->wait_random = 0;
        env->wait = 0;
        env->tex = 0;
    }
}


void NuTexAnimRestart(void)
{
    NuTexAnimList *list;
    NuTexAnim *anim;

    for (list = ntal_first; list; list = list->next) {
        for (anim = list->anims; anim; anim = anim->next) {
            NuTexAnimEnvReset(anim->env);
        }
    }
}


void NuTexAnimSetSignals(unsigned int signals)
{
    unsigned int changed;

    changed = signals ^ nta_signals;
    nta_signals = signals;
    nta_signals_on = changed & signals;
    nta_signals_off = changed & ~signals;
}


void NuTexAnimProcessList(NuTexAnim *anim)
{
    for (; anim; anim = anim->next) {
        if (anim->env) {
            NuTexAnimEnvProc(anim->env);
        }
    }
}


NuTexAnim *NuTexAnimFind(NuTexAnim *anim, char *name)
{
    for (; anim; anim = anim->next) {
        if (!strcasecmp(anim->name, name)) {
            return anim;
        }
    }
    return 0;
}


void NuTexAnimDestroy(NuTexAnim *anim)
{
    if (anim->env) {
        NuTexAnimEnvDestroy(anim->env);
    }
    if (anim->dynalloc) {
        NuMemFreeFn(anim, nutexanm_srcfile, 0x4AE);
    }
}


void NuTexAnimAddList(NuTexAnim *anim)
{
    NuTexAnimList *list;

    NuDisableVBlank();
    list = ntal_free;
    if (list) {
        ntal_free = list->next;
        list->anims = anim;
        list->next = ntal_first;
        list->prev = 0;
        if (ntal_first) {
            ntal_first->prev = list;
        }
        ntal_first = list;
    }
    NuEnableVBlank();
}


void NuTexAnimProcess(void)
{
    NuTexAnimList *list;
    NuTexAnim *anim;

    for (list = ntal_first; list; list = list->next) {
        for (anim = list->anims; anim; anim = anim->next) {
            if (anim->env) {
                NuTexAnimEnvProc(anim->env);
            }
        }
    }
}


void NuTexAnimProgInit(NuTexAnimProg *prog)
{
    int i;

    if (prog) {
        for (i = 0; i < 32; i++) {
            prog->off_pc[i] = -1;
            prog->on_pc[i] = -1;
        }
        prog->num_codes = 0;
        prog->on_mask = 0;
        prog->off_mask = 0;
        prog->name[0] = 0;
        prog->num_xdefs = 0;
        prog->dynalloc = 0;
    }
}


void pftaTex(NuFPar *parser)
{
    int tex;

    tex = NuFParGetInt(parser);
    nta_prog->code[nta_prog->num_codes++] = NTA_TEX;
    nta_prog->code[nta_prog->num_codes++] = tex;
}


void pftaOn(NuFPar *parser)
{
    int signal;

    signal = NuFParGetInt(parser);
    nta_prog->on_pc[signal] = nta_prog->num_codes;
    nta_prog->on_mask |= 1 << signal;
}


void pftaOff(NuFPar *parser)
{
    int signal;

    signal = NuFParGetInt(parser);
    nta_prog->off_pc[signal] = nta_prog->num_codes;
    nta_prog->off_mask |= 1 << signal;
}


void pftaRet(NuFPar *parser)
{
    nta_prog->code[nta_prog->num_codes++] = NTA_RET;
}


void pftaRepend(NuFPar *parser)
{
    nta_prog->code[nta_prog->num_codes++] = NTA_REPEND;
}


void pftaEnd(NuFPar *parser)
{
    nta_prog->code[nta_prog->num_codes++] = NTA_END;
}


void pftaScriptname(NuFPar *parser)
{
    NuFParGetWord(parser);
    parser->word[32] = 0;
    strcpy(nta_prog->name, parser->word);
}


void NuTexAnimResetList(NuTexAnim *anim)
{
    for (; anim; anim = anim->next) {
        NuTexAnimEnvReset(anim->env);
    }
}
