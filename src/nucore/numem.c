/*
 * Unit: nucore/numem
 *
 * Functions:
 *   0x001047c8 NuMemAlloc
 *   0x00104958 NuMemReAlloc
 *   0x00104a78 NuMemAllocFn
 *   0x00104d08 NuMemReAllocFn
 *   0x00104f78 NuMemFreeFn
 *   0x00105118 NuMemValidateFn
 *   0x001051b0 NuMemDumpFn
 *   0x00105268 NuMemFlushFn
 *   0x001052a0 NuMemSetDiscardable
 *   0x001052b0 NuMemFlushDiscardable
 *   0x001052d0 NuMemCreateDiscardable
 *   0x00105338 NuMemDestroyDiscardable
 *   0x00105360 NuMemSetExternal
 *   0x00105390 NuMemFree
 */

typedef struct NuDiscardable {
    int size;
    int remaining;
    void *next;
    unsigned char pad0C[4];
} NuDiscardable;

typedef struct NuExternalMemory {
    void *buffer;
    int size;
} NuExternalMemory;

extern NuDiscardable *D_0062EA0C;
extern NuExternalMemory *D_0062EA14;
extern NuExternalMemory D_00633018;
#define current_discardable D_0062EA0C
#define current_external D_0062EA14
#define external_memory D_00633018


NuDiscardable *NuMemSetDiscardable(NuDiscardable *pool)
{
    NuDiscardable *previous = current_discardable;

    current_discardable = pool;
    return previous;
}


void NuMemFlushDiscardable(NuDiscardable *pool)
{
    if (pool) {
        pool->remaining = pool->size;
        pool->next = pool + 1;
    }
}


void NuMemSetExternal(void *buffer, int *size)
{
    NuExternalMemory *memory;

    if (buffer) {
        memory = &external_memory;
        memory->buffer = buffer;
        current_external = memory;
        if (size) {
            memory->size = *size;
            return;
        }
    } else {
        current_external = 0;
    }
}


#ifndef NULL
#define NULL ((void *)0)
#endif
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
s32 free();

void NuMemFree(void) {
    free();
}



extern s32 *D_0062EA04;

void NuMemFlushFn(void) {
    s32 *var_a0;
    s32 *var_s0;

    var_s0 = D_0062EA04;
    if (var_s0 != ((void *)0)) {
        var_a0 = var_s0;
        do {
            var_s0 = *var_s0;
            free(var_a0);
            var_a0 = var_s0;
        } while (var_s0 != ((void *)0));
    }
}
