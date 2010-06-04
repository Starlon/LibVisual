#ifndef _SCRIPT_VISSCRIPT_H
#define _SCRIPT_VISSCRIPT_H

#include <libvisual/libvisual.h>

#include <v8.h>

typedef struct {
    VisObject object;
    v8::Handle<v8::ObjectTemplate> global;
    v8::Persistent<v8::Context> context;
} PrivateDataOut;

#define PRIVATE_DATA(obj)   (VISUAL_CHECK_CAST ((obj), PrivateDataOut))

#endif
