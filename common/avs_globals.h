
#ifndef _GLOBAL_PROXY_H_
#define _GLOBAL_PROXY_H_

#include <libvisual/libvisual.h>

#define AVS_GLOBAL_PROXY(obj)   VISUAL_CHECK_CAST((obj), AvsGlobalProxy)
#define AVS_MULTIDELAY(obj)     VISUAL_CHECK_CAST((obj), AvsMultidelayGlobals)

#define MAXBUF 8

typedef struct {
    uint32_t *buffer;
    int w;
    int h;
} AvsGlobalBuffer;

typedef struct {
    void *fb;
} AvsLPVOID;

typedef struct _AvsGlobalProxy AvsGlobalProxy;
typedef struct _AvsMultidelayGlobals AvsMultidelayGlobals;

typedef struct _AvsGlobalProxy {
    VisObject obj;
    unsigned char   blendtable[256][256];
    int reset_vars_on_recompile;
    int line_blend_mode;
    AvsGlobalBuffer *buffers;
    int numbuffers;
    int isBeat;
    float audiodata[2][2][1024]; // Is in the format of [spectrum:0,wave:1][channel][band]

    AvsMultidelayGlobals *multidelay;

    char *path;
};

struct _AvsMultidelayGlobals {
    VisObject obj;

    AvsLPVOID buffer[6];
    AvsLPVOID inpos[6];
    AvsLPVOID outpos[6];
    unsigned long buffersize[6];
    unsigned long virtualbuffersize[6];
    unsigned long oldvirtualbuffersize[6];
    unsigned long framedelay[6];
    unsigned int numinstances;
    unsigned long framessincebeat;
    unsigned long framesperbeat;
    unsigned long framemem;
    unsigned long oldframemem;
    unsigned int renderid;
    uint8_t usebeats[6];
    int32_t delay[6];
};

AvsGlobalBuffer *avs_get_global_buffer(AvsGlobalProxy *obj, int w, int h, int n, int do_alloc);
AvsGlobalProxy *avs_global_proxy_new();

#endif
