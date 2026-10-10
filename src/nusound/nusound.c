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

typedef struct sceSifClientData sceSifClientData;

typedef struct NuSoundLoopInfo {
    int id;
    short pitch;
    short channel;
    short active;
    short pad;
    int reserved;
} NuSoundLoopInfo;

extern int strlen(const char *s);
extern int printf(const char *fmt, ...);
extern void NuFileGetCurrentPath(char *path);
extern int sceSifCallRpc(sceSifClientData *client, unsigned int function,
                         unsigned int mode, void *send, int send_size,
                         void *receive, int receive_size, void *end_func,
                         void *end_param);

void SetIOPCommand(short command, short count);
void SetIOPCommandEx(short command, short size, void *data);

extern short IOPCommandData[];
extern short senddata[];
extern int sbuff2[];
extern int IOP_Cnt;
extern int nusound_irx_loaded;
extern char D_0061AA78[];
extern sceSifClientData D_006B4100;
extern volatile unsigned char SOUND_SPUKeyStatus[];
extern NuSoundLoopInfo NuSndLoopInfo[];
extern nuvec_s *NuSoundListenPos;
extern int NuSndSteroPitch;

#define iop_buffer_full_message D_0061AA78
#define sound_rpc_client D_006B4100


void SOUND_CancelCheckStereo(void)
{
    short command = 0x12;
    short count = 0;
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
    sceSifCallRpc(&sound_rpc_client, function, mode, data, 0x800, sbuff2,
                  0x200, 0, 0);
    return sbuff2[0];
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


void SOUND_TransferSFX(int sample, int channel)
{
    IOPCommandData[0] = sample;
    IOPCommandData[1] = channel;
    SetIOPCommand(6, 2);
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
