/*
 * Unit: sdk/sce/sifcmd
 *
 * Functions:
 *   0x0027ac20 _set_sreg
 *   0x0027ac40 _change_addr
 *   0x0027ac50 sceSifGetSreg
 *   0x0027ac68 sceSifSetSreg
 *   0x0027ac88 sceSifGetDataTable
 *   0x0027ac98 sceSifInitCmd
 *   0x0027af18 sceSifExitCmd
 *   0x0027af50 sceSifSetCmdBuffer
 *   0x0027af68 sceSifSetSysCmdBuffer
 *   0x0027af80 sceSifAddCmdHandler
 *   0x0027afb0 sceSifRemoveCmdHandler
 *   0x0027afd8 _sceSifSendCmd
 *   0x0027b110 sceSifSendCmd
 *   0x0027b150 isceSifSendCmd
 *   0x0027b190 _sceSifCmdIntrHdlr
 *   0x0027b2b8 sceSifWriteBackDCache
 */

typedef struct SifCmdHeader {
    unsigned int size;
    void *dest;
    int cid;
    unsigned int opt;
} SifCmdHeader;

typedef struct SifCmdSRegData {
    SifCmdHeader header;
    int index;
    unsigned int value;
} SifCmdSRegData;

typedef struct SifCmdChangeAddrData {
    SifCmdHeader header;
    void *newaddr;
} SifCmdChangeAddrData;

typedef void (*SifCmdHandler)(void *packet, void *data);

typedef struct SifCmdHandlerData {
    SifCmdHandler func;
    void *data;
} SifCmdHandlerData;

typedef struct SifCmdData {
    void *pktbuf;
    void *unused;
    void *iopbuf;
    SifCmdHandlerData *sys_cmd_handlers;
    int nr_sys_handlers;
    SifCmdHandlerData *usr_cmd_handlers;
    int nr_usr_handlers;
    unsigned int *sregs;
} SifCmdData;

extern int D_00613374[];
extern int D_00703794[];
extern SifCmdData D_00703798;
extern unsigned int D_007038C0[];

#define sif_cmd_initialized D_00613374
#define sif_dmac_handler_id D_00703794
#define sif_cmd_data        D_00703798
#define sif_sregs           D_007038C0

extern int DisableDmac(int channel);
extern int RemoveDmacHandler(int channel, int handler_id);
extern unsigned int _sceSifSendCmd(int cid, int mode, void *packet,
                                   int packet_size, void *src, void *dest,
                                   int size);


void _set_sreg(SifCmdSRegData *packet, SifCmdData *cmd) {
    cmd->sregs[packet->index] = packet->value;
}


void _change_addr(SifCmdChangeAddrData *packet, SifCmdData *cmd) {
    cmd->iopbuf = packet->newaddr;
}


unsigned int sceSifGetSreg(int index) {
    return sif_sregs[index];
}


unsigned int sceSifSetSreg(int index, unsigned int value) {
    return sif_sregs[index] = value;
}


SifCmdData *sceSifGetDataTable(void) {
    return &sif_cmd_data;
}


void sceSifExitCmd(void) {
    DisableDmac(5);
    RemoveDmacHandler(5, sif_dmac_handler_id[0]);
    sif_cmd_initialized[0] = 0;
}


SifCmdHandlerData *sceSifSetCmdBuffer(SifCmdHandlerData *buffer, int count) {
    SifCmdHandlerData *old;

    old = sif_cmd_data.usr_cmd_handlers;
    sif_cmd_data.usr_cmd_handlers = buffer;
    sif_cmd_data.nr_usr_handlers = count;
    return old;
}


SifCmdHandlerData *sceSifSetSysCmdBuffer(SifCmdHandlerData *buffer, int count) {
    SifCmdHandlerData *old;

    old = sif_cmd_data.sys_cmd_handlers;
    sif_cmd_data.sys_cmd_handlers = buffer;
    sif_cmd_data.nr_sys_handlers = count;
    return old;
}


void sceSifAddCmdHandler(int cid, SifCmdHandler handler, void *data) {
    if (cid < 0) {
        sif_cmd_data.sys_cmd_handlers[cid].func = handler;
        sif_cmd_data.sys_cmd_handlers[cid].data = data;
    } else {
        sif_cmd_data.usr_cmd_handlers[cid].func = handler;
        sif_cmd_data.usr_cmd_handlers[cid].data = data;
    }
}


void sceSifRemoveCmdHandler(int cid) {
    if (cid < 0) {
        sif_cmd_data.sys_cmd_handlers[cid].func = 0;
    } else {
        sif_cmd_data.usr_cmd_handlers[cid].func = 0;
    }
}


unsigned int sceSifSendCmd(int cid, void *packet, int packet_size, void *src,
                           void *dest, int size) {
    return _sceSifSendCmd(cid, 0, packet, packet_size, src, dest, size);
}


unsigned int isceSifSendCmd(int cid, void *packet, int packet_size, void *src,
                            void *dest, int size) {
    return _sceSifSendCmd(cid, 1, packet, packet_size, src, dest, size);
}
