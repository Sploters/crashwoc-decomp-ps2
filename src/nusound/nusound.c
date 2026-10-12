/*
 * Unit: nusound/nusound
 *
 * Functions:
 *   0x001c2290 SOUND_CopyIOPBuffer
 *   0x001c23f8 SOUND_CancelCheckStereo
 *   0x001c2470 init_adpcm
 *   0x001c27a0 TransferAllSpotFX
 *   0x001c29c0 SOUND_StoreLocalStart
 *   0x001c2ab8 SOUND_RestoreLocalStart
 *   0x001c2bb0 SOUND_InitAdpcmLocal
 *   0x001c2ca8 NuSoundInit
 *   0x001c2da8 NuSoundApply3d
 *   0x001c3008 NuSoundUpdate
 *   0x001c31c8 NuSoundFlushLoops
 *   0x001c32d8 NuSoundPlay
 *   0x001c3498 NuSoundPlayChan
 *   0x001c35f0 NuSoundPlay3d
 *   0x001c37a8 NuSoundPlay3dChan
 *   0x001c3900 NuSoundPlayStereo
 *   0x001c3b28 NuSoundPlayStereo2
 *   0x001c3d48 NuSoundPlay3dLoopSfx
 *   0x001c3f40 NuSoundKillAllAudio
 *   0x001c41f0 SOUND_StopSound
 *   0x001c4230 NuSoundSetChannelPitch
 *   0x001c4290 NuSoundGetSteroPitch
 *   0x001c4298 NuSoundStopStream
 *   0x001c42e0 NuSoundLocalSet
 *   0x001c4440 NuSoundListener
 *   0x001c4448 NuSoundKeyStatus
 *   0x001c4470 NuSoundSetGlobalDat
 *   0x001c44b0 NuSoundSetLocalDat
 *   0x001c44f0 SOUND_SIFTransfer
 *   0x001c4540 SOUND_InitIOP
 *   0x001c45d0 SetIOPCommand
 *   0x001c4698 SetIOPCommandEx
 *   0x001c4760 FlushIOPCommand
 *   0x001c4808 SOUND_AllChannelEffectsOff
 *   0x001c48c8 SOUND_AllChannelEffectsOn
 *   0x001c4988 SOUND_SetChannelEffectOff
 *   0x001c4a48 SOUND_SetChannelEffectOn
 *   0x001c4b08 SOUND_SetEffectMasterVolume
 *   0x001c4bd0 SOUND_DisableEffects
 *   0x001c4c90 SOUND_EnableEffects
 *   0x001c4d68 SOUND_FindFreeSPUChannel
 *   0x001c4de0 SOUND_SetSPUWriteAddress
 *   0x001c4e18 SOUND_AllocateStreamBuffer
 *   0x001c4e60 SOUND_PlayStream
 *   0x001c4ef0 SOUND_PlaySFX
 *   0x001c4f60 SOUND_TransferSFX
 *   0x001c4f90 SOUND_SetChannelVolume
 *   0x001c4fe0 SOUND_SetChannelPitch
 *   0x001c5028 SOUND_GetStatus
 *   0x001c5050 SOUND_Pause
 *   0x001c5070 SOUND_Resume
 *   0x001c5090 SOUND_InitStreamData
 *   0x001c50b0 SOUND_InitSPU
 *   0x001c50f0 SOUND_AddSamples
 *   0x001c5230 SOUND_SetMaxStreamLimit
 *   0x001c5258 SOUND_CheckAndStartStereo
 *   0x001c5320 SOUND_SetChannelPitchStereo
 *   0x001c5368 SOUND_CloseStreamBuffer
 *   0x001c5390 NuSoundInitLoopInfo
 *   0x001c53d0 NuSoundFindFree
 *   0x001c5428 NuSoundFindLoopInfo
 */

typedef struct nuvec_s {
    float x;
    float y;
    float z;
} nuvec_s;

typedef struct sceSifRpcData {
    void *paddr;
    unsigned int pid;
    int tid;
    unsigned int mode;
} sceSifRpcData;

typedef struct sceSifClientData {
    sceSifRpcData rpcd;
    unsigned int command;
    void *buff;
    void *cbuff;
    void *end_func;
    void *end_param;
    void *serve;
} sceSifClientData;

typedef struct NuSoundLoopInfo {
    int id;
    short pitch;
    short channel;
    short active;
    short pad;
    int reserved;
} NuSoundLoopInfo;

typedef struct SFXINFO {
    char filename[0x20];
    int pitch;
    int loop_info;
    int id;
} SFXINFO;

typedef void (*NuDebugMsgFn)();

extern int strlen(const char *s);
extern int printf(const char *fmt, ...);
extern void NuFileGetCurrentPath(char *path);
extern void *memcpy();
extern void sceSifInitRpc(unsigned int mode);
extern int sceSifBindRpc(sceSifClientData *client, unsigned int request,
                         unsigned int mode);
extern int sceSifCallRpc(sceSifClientData *client, unsigned int function,
                         unsigned int mode, void *send, int send_size,
                         void *receive, int receive_size, void *end_func,
                         void *end_param);
extern int sceSifCheckStatRpc(sceSifRpcData *rpcd);
extern NuDebugMsgFn NuDebugMsgProlog(char *file, int line);

void SOUND_CopyIOPBuffer(void);
void init_adpcm(SFXINFO *sfx_info);
void TransferAllSpotFX(int mode);
void SOUND_StoreLocalStart(void);
void SetIOPCommand(short command, short count);
void SetIOPCommandEx(short command, short size, void *data);

extern short IOPCommandData[];
extern unsigned short senddata[];
extern volatile int sbuff2[];
extern int IOP_Cnt;
extern int nusound_irx_loaded;
extern char D_0061AA20[];
extern char D_0061AA38[];
extern char D_0061AA78[];
extern char D_0061AB30[];
extern sceSifClientData D_006B4100;
extern SFXINFO D_006B4128[];
extern volatile unsigned char SOUND_SPUKeyStatus[];
extern NuSoundLoopInfo NuSndLoopInfo[];
extern nuvec_s *NuSoundListenPos;
extern int NuSndSteroPitch;
extern int SOUND_STREAM_SPU_CHAN[];
extern int SOUND_STREAM_ID[];
extern int SOUND_STREAM_SPU_ADDR[];
extern int SOUND_STREAM_PLAY_HALF[];
extern int SOUND_STREAM_ENV[];
extern int SOUND_STREAM_CD_ACCESS;
extern int SOUND_STREAM_STATUS;
extern int SOUND_CD_STATUS;
extern int SOUND_IOP_REQUEST_LOAD;
extern int SOUND_SHUTOFF_STATUS;
extern int SOUND_SFX_TRANSFER_STATUS;
extern int SOUND_SFX_TRANSFER_END;
extern int SOUND_STEREO_READY_FLAG;
extern int SChan;
extern int Attack;
extern int Release;

#define bind_error_message D_0061AA20
#define nusound_source_file D_0061AA38
#define irx_not_loaded_message D_0061AB30
#define iop_buffer_full_message D_0061AA78
#define sound_rpc_client D_006B4100
#define sound_sample_info D_006B4128

#define SOUND_RPC_ID 0x12345

static inline int sound_rpc_transfer(int function, void *data, int mode)
{
    sceSifCallRpc(&sound_rpc_client, function, mode, data, 0x800,
                  (void *)sbuff2, 0x200, 0, 0);
    return sbuff2[0];
}


static inline void queue_iop_command(short command, short count)
{
    int i;

    if (IOP_Cnt + count + 3 >= 0x400) {
        printf(iop_buffer_full_message);
        return;
    }
    senddata[IOP_Cnt + 1] = command;
    senddata[IOP_Cnt + 2] = count;
    for (i = 0; i < count; i++) {
        senddata[IOP_Cnt + 3 + i] = IOPCommandData[i];
    }
    IOP_Cnt += count + 2;
    senddata[0]++;
}


static inline int flush_iop_commands(int mode)
{
    int count;

    SOUND_CopyIOPBuffer();
    if (sceSifCheckStatRpc(&sound_rpc_client.rpcd) != 0) {
        return -1;
    }
    if (senddata[0] == 0) {
        return -2;
    }
    sound_rpc_transfer(0, senddata, mode);
    count = IOP_Cnt;
    senddata[0] = 0;
    IOP_Cnt = 0;
    return count;
}


static inline int play_stream(int stream, int bank, unsigned int channel,
                              int left, int right, int pitch, int start,
                              int flags, int priority)
{
    int envelope;

    if (channel >= 0x30) {
        return -1;
    }
    channel += bank << 8;
    left &= 0x7FFF;
    envelope = flags + (priority << 8);
    right &= 0x7FFF;
    if (bank != 0x80) {
        SOUND_SPUKeyStatus[channel] = 0;
    }
    IOPCommandData[0] = stream;
    IOPCommandData[1] = channel;
    IOPCommandData[2] = left;
    IOPCommandData[3] = right;
    IOPCommandData[4] = pitch;
    IOPCommandData[5] = start;
    IOPCommandData[6] = envelope;
    SetIOPCommand(1, 7);
    return 0;
}


static inline int play_sfx(int sample, unsigned int channel, int left,
                           int right, int pitch, int flags, int priority)
{
    if (channel >= 0x30) {
        return -1;
    }
    IOPCommandData[0] = sample;
    IOPCommandData[1] = channel;
    left &= 0x7FFF;
    right &= 0x7FFF;
    IOPCommandData[2] = left;
    IOPCommandData[3] = right;
    IOPCommandData[4] = pitch;
    IOPCommandData[5] = flags + (priority << 8);
    SetIOPCommand(0, 6);
    return 0;
}


static inline int find_free_channel(int channel)
{
    int i;
    int candidate;

    for (i = 0; i < 0x12; i++) {
        candidate = (channel - 6 + i) % 0x12 + 6;
        if (SOUND_SPUKeyStatus[candidate] == 0) {
            return candidate;
        }
    }
    return channel;
}


void SOUND_CopyIOPBuffer(void)
{
    int i;
    int j;

    SOUND_CD_STATUS = sbuff2[23];
    SOUND_SHUTOFF_STATUS = sbuff2[27];
    SOUND_STREAM_CD_ACCESS = sbuff2[0];
    SOUND_IOP_REQUEST_LOAD = sbuff2[24];
    SOUND_STREAM_STATUS = sbuff2[1];
    for (i = 0; i < 4; i++) {
        SOUND_STREAM_SPU_CHAN[i] = sbuff2[2 + i];
        SOUND_STREAM_ID[i] = sbuff2[6 + i];
        SOUND_STREAM_SPU_ADDR[i] = sbuff2[10 + i];
        SOUND_STREAM_PLAY_HALF[i] = sbuff2[14 + i];
        SOUND_STREAM_ENV[i] = sbuff2[18 + i];
    }
    SOUND_SFX_TRANSFER_STATUS = sbuff2[26];
    SOUND_SFX_TRANSFER_END = sbuff2[22];
    SOUND_STEREO_READY_FLAG = sbuff2[28];
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 16; j++) {
            SOUND_SPUKeyStatus[i * 16 + j] = (sbuff2[29 + i] >> (30 - j * 2)) & 3;
        }
    }
}


void SOUND_CancelCheckStereo(void)
{
    queue_iop_command(0x12, 0);
}


void SOUND_StoreLocalStart(void)
{
    queue_iop_command(0x1B, 0);
    flush_iop_commands(0);
}


void SOUND_RestoreLocalStart(void)
{
    queue_iop_command(0x1C, 0);
    flush_iop_commands(0);
}


void SOUND_InitAdpcmLocal(void)
{
    queue_iop_command(0x1D, 0);
    flush_iop_commands(0);
}


void NuSoundInit(SFXINFO *sfx_info)
{
    int i;

    if (!nusound_irx_loaded) {
        NuDebugMsgProlog(nusound_source_file, 0x433)(irx_not_loaded_message);
        return;
    }
    for (i = 0; i < 8; i++) {
        NuSndLoopInfo[i].id = 0;
        NuSndLoopInfo[i].channel = i + 0x18;
        NuSndLoopInfo[i].active = 0;
        NuSndLoopInfo[i].pitch = 0;
    }
    init_adpcm(sfx_info);
    flush_iop_commands(0);
    TransferAllSpotFX(0);
    SOUND_StoreLocalStart();
}


void NuSoundPlay(int sfx, int left, int right, int pitch)
{
    if (nusound_irx_loaded) {
        if (left >= 0x4000) {
            left = 0x3FFF;
        }
        if (right >= 0x4000) {
            right = 0x3FFF;
        }
        if (left <= -0x4000) {
            left = -0x3FFF;
        }
        if (right <= -0x4000) {
            right = -0x3FFF;
        }
        if (sound_sample_info[sfx].id >= 0x1000) {
            SChan = find_free_channel(SChan);
            play_sfx(sound_sample_info[sfx].id, SChan, left, right, pitch,
                     Attack, Release);
            SChan = (SChan - 6 + 1) % 0x12 + 6;
        } else {
            play_stream(sound_sample_info[sfx].id, 0x80, 0, left, right, pitch,
                        2, Attack, Release);
        }
    }
}


void NuSoundPlayChan(int sfx, int left, int right, int pitch, int channel)
{
    int sample;

    if (nusound_irx_loaded) {
        if (left >= 0x4000) {
            left = 0x3FFF;
        }
        if (right >= 0x4000) {
            right = 0x3FFF;
        }
        if (left <= -0x4000) {
            left = -0x3FFF;
        }
        if (right <= -0x4000) {
            right = -0x3FFF;
        }
        sample = sound_sample_info[sfx].id;
        if (sample >= 0x1000) {
            play_sfx(sample, channel, left, right, pitch, Attack, Release);
        } else {
            play_stream(sample, channel, channel, left, right, pitch, 2, Attack,
                        Release);
        }
    }
}


int SOUND_StopSound(unsigned int channel)
{
    if (channel >= 0x30) {
        return -1;
    }
    IOPCommandData[0] = channel;
    SetIOPCommand(4, 1);
    return 0;
}


int NuSoundSetChannelPitch(unsigned int channel, int pitch, int stereo)
{
    if (stereo) {
        if (channel >= 0x2F) {
            return -1;
        }
        IOPCommandData[0] = channel;
        IOPCommandData[1] = pitch;
        SetIOPCommand(0x20, 2);
        return 0;
    }
    if (channel < 0x30) {
        IOPCommandData[0] = channel;
        IOPCommandData[1] = pitch;
        SetIOPCommand(3, 2);
        return 0;
    }
    return -1;
}


int NuSoundGetSteroPitch(void)
{
    return NuSndSteroPitch;
}


int NuSoundStopStream(int stream)
{
    if (nusound_irx_loaded && stream >= 0) {
        IOPCommandData[0] = stream;
        SetIOPCommand(5, 1);
        return 0;
    }
    return -1;
}


void NuSoundListener(nuvec_s *pos)
{
    NuSoundListenPos = pos;
}


int NuSoundKeyStatus(unsigned int channel)
{
    if (channel >= 0x30) {
        return 0;
    }
    return SOUND_SPUKeyStatus[channel];
}


void NuSoundSetGlobalDat(char *dat)
{
    SetIOPCommandEx(0x1E, strlen(dat) + 1, dat);
}


void NuSoundSetLocalDat(char *dat)
{
    SetIOPCommandEx(0x1F, strlen(dat) + 1, dat);
}


int SOUND_SIFTransfer(int function, void *data, int mode)
{
    return sound_rpc_transfer(function, data, mode);
}


int SOUND_InitIOP(void)
{
    int delay;

    sceSifInitRpc(0);
    while (1) {
        if (sceSifBindRpc(&sound_rpc_client, SOUND_RPC_ID, 0) < 0) {
            printf(bind_error_message);
            while (1) {
            }
        }
        delay = 10000;
        while (delay--) {
        }
        if (sound_rpc_client.serve != 0) {
            break;
        }
    }
    return 1;
}


void SetIOPCommand(short command, short count)
{
    queue_iop_command(command, count);
}


void SetIOPCommandEx(short command, short size, void *data)
{
    if (IOP_Cnt + size + 3 >= 0x400) {
        printf(iop_buffer_full_message);
        return;
    }
    senddata[IOP_Cnt + 1] = command;
    senddata[IOP_Cnt + 2] = (size + 1) / 2;
    memcpy(&senddata[IOP_Cnt + 3], data, size);
    IOP_Cnt += (size + 1) / 2 + 2;
    senddata[0]++;
}


int FlushIOPCommand(int mode)
{
    int count;

    SOUND_CopyIOPBuffer();
    if (sceSifCheckStatRpc(&sound_rpc_client.rpcd) != 0) {
        return -1;
    }
    if (senddata[0] == 0) {
        return -2;
    }
    sound_rpc_transfer(0, senddata, mode);
    count = IOP_Cnt;
    senddata[0] = 0;
    IOP_Cnt = 0;
    return count;
}


void SOUND_AllChannelEffectsOff(int core)
{
    IOPCommandData[0] = core;
    queue_iop_command(0x17, 1);
}


void SOUND_AllChannelEffectsOn(int core)
{
    IOPCommandData[0] = core;
    queue_iop_command(0x16, 1);
}


void SOUND_SetChannelEffectOff(int channel)
{
    IOPCommandData[0] = channel;
    queue_iop_command(0x19, 1);
}


void SOUND_SetChannelEffectOn(int channel)
{
    IOPCommandData[0] = channel;
    queue_iop_command(0x18, 1);
}


void SOUND_SetEffectMasterVolume(int core, int left, int right)
{
    IOPCommandData[0] = core;
    IOPCommandData[1] = left;
    IOPCommandData[2] = right;
    queue_iop_command(0x15, 3);
}


void SOUND_DisableEffects(int core)
{
    IOPCommandData[0] = core;
    queue_iop_command(0x14, 1);
}


void SOUND_EnableEffects(int core, int mode, int depth_left, int depth_right,
                         int delay, int feedback)
{
    IOPCommandData[0] = core;
    IOPCommandData[1] = mode;
    IOPCommandData[2] = depth_left;
    IOPCommandData[3] = depth_right;
    IOPCommandData[4] = delay;
    IOPCommandData[5] = feedback;
    queue_iop_command(0x13, 6);
}


int SOUND_FindFreeSPUChannel(unsigned int first, unsigned int last)
{
    unsigned int channel;

    if (first >= 0x30) {
        return -2;
    }
    if (last < first || last >= 0x30) {
        return -3;
    }
    for (channel = first; channel <= last; channel++) {
        if (SOUND_SPUKeyStatus[channel] == 0) {
            SOUND_SPUKeyStatus[channel] = 3;
            return channel;
        }
    }
    return -1;
}


void SOUND_SetSPUWriteAddress(unsigned int address)
{
    IOPCommandData[0] = address >> 16;
    IOPCommandData[1] = address;
    SetIOPCommand(0xE, 2);
}


void SOUND_AllocateStreamBuffer(int stream, unsigned int address,
                                unsigned int size)
{
    IOPCommandData[0] = stream;
    IOPCommandData[1] = address >> 16;
    IOPCommandData[2] = address;
    IOPCommandData[3] = size >> 16;
    IOPCommandData[4] = size;
    SetIOPCommand(0xD, 5);
}


int SOUND_PlayStream(int stream, int bank, unsigned int channel, int left,
                     int right, int pitch, int start, int flags, int priority)
{
    return play_stream(stream, bank, channel, left, right, pitch, start,
                       flags, priority);
}


int SOUND_PlaySFX(int sample, unsigned int channel, int left, int right,
                  int pitch, int flags, int priority)
{
    if (channel >= 0x30) {
        return -1;
    }
    IOPCommandData[0] = sample;
    IOPCommandData[1] = channel;
    left &= 0x7FFF;
    right &= 0x7FFF;
    IOPCommandData[2] = left;
    IOPCommandData[3] = right;
    IOPCommandData[4] = pitch;
    IOPCommandData[5] = flags + (priority << 8);
    SetIOPCommand(0, 6);
    return 0;
}


void SOUND_TransferSFX(int sample, int channel)
{
    IOPCommandData[0] = sample;
    IOPCommandData[1] = channel;
    SetIOPCommand(6, 2);
}


int SOUND_SetChannelVolume(unsigned int channel, int left, int right)
{
    if (channel >= 0x30) {
        return -1;
    }
    IOPCommandData[0] = channel;
    IOPCommandData[1] = left;
    IOPCommandData[2] = right;
    SetIOPCommand(2, 3);
    return 0;
}


int SOUND_SetChannelPitch(unsigned int channel, int pitch)
{
    if (channel >= 0x30) {
        return -1;
    }
    IOPCommandData[0] = channel;
    IOPCommandData[1] = pitch;
    SetIOPCommand(3, 2);
    return 0;
}


void SOUND_GetStatus(int request)
{
    IOPCommandData[0] = request;
    SetIOPCommand(9, 1);
}


void SOUND_Pause(void)
{
    SetIOPCommand(0xA, 0);
}


void SOUND_Resume(void)
{
    SetIOPCommand(0xB, 0);
}


void SOUND_InitStreamData(void)
{
    SetIOPCommand(0xC, 0);
}


void SOUND_InitSPU(void)
{
    char path[256];

    NuFileGetCurrentPath(path);
    SetIOPCommandEx(7, strlen(path) + 1, path);
}


void SOUND_SetMaxStreamLimit(int limit)
{
    IOPCommandData[0] = limit;
    SetIOPCommand(0x10, 1);
}


void SOUND_CheckAndStartStereo(int stream, int left_channel, int right_channel)
{
    IOPCommandData[0] = stream;
    IOPCommandData[1] = left_channel;
    IOPCommandData[2] = right_channel;
    queue_iop_command(0x11, 3);
}


int SOUND_SetChannelPitchStereo(unsigned int channel, int pitch)
{
    if (channel >= 0x2F) {
        return -1;
    }
    IOPCommandData[0] = channel;
    IOPCommandData[1] = pitch;
    SetIOPCommand(0x20, 2);
    return 0;
}


void SOUND_CloseStreamBuffer(int stream)
{
    IOPCommandData[0] = stream;
    SetIOPCommand(0xF, 1);
}


void NuSoundInitLoopInfo(void)
{
    int i;

    for (i = 0; i < 8; i++) {
        NuSndLoopInfo[i].id = 0;
        NuSndLoopInfo[i].channel = i + 0x18;
        NuSndLoopInfo[i].active = 0;
        NuSndLoopInfo[i].pitch = 0;
    }
}


int NuSoundFindFree(int channel)
{
    return find_free_channel(channel);
}


NuSoundLoopInfo *NuSoundFindLoopInfo(int id)
{
    NuSoundLoopInfo *info;
    int free;
    int i;

    free = -1;
    for (i = 0; i < 8; i++) {
        info = &NuSndLoopInfo[i];
        if (info->active == 0) {
            free = i;
        }
        if (info->id == id) {
            return info;
        }
    }
    if (free == -1) {
        return 0;
    }
    NuSndLoopInfo[free].id = id;
    NuSndLoopInfo[free].pitch = 0;
    return &NuSndLoopInfo[free];
}
