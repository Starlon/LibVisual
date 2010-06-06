
#ifndef _V8SCRIPT_H
#define __V8SCRIPT_H

#include <v8.h>

class v8Script {
    private:
        v8::Persistent<v8::ObjectTemplate> global_;
        v8::Persistent<v8::Context> context_;
    public:
        v8::Persistent<v8::ObjectTemplate> GetGlobal() { return global_; }
        v8::Persistent<v8::Context> GetContext() { return context_; }
        v8Script();
        ~v8Script();
};
#endif
