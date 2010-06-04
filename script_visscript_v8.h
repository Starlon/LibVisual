#ifndef _SCRIPT_VISSCRIPT_H
#define _SCRIPT_VISSCRIPT_H

#include <libvisual/libvisual.h>

typedef struct {
    VisObject object;
    void *context;
    void *global;
} PrivateDataOut;

#define PRIVATE_DATA(obj)   (VISUAL_CHECK_CAST ((obj), PrivateDataOut))

#endif
