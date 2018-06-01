//
// Created by CoulsonChen on 2018/5/29.
//

#ifndef SUNGAMEENGINE_V8UTIL_H
#define SUNGAMEENGINE_V8UTIL_H


#include "include/libplatform/libplatform.h"
#include "include/v8.h"


using namespace v8;



// V8 --------------------------------------------------------------------------

//void init();
//void change(int w , int h);
//void draw();
Handle<Script> loadScript(const char *name);

Handle<Value> requireScript(const FunctionCallback &args);

bool ExecuteString(Isolate *isolate, const char *name, bool print_result,
                   bool report_exceptions);

void ReportException(Isolate *isolate, TryCatch *try_catch);

Handle<Value> log(const FunctionCallback &args);

bool callFunction(const char *name, Handle<Value> *args, int argc);

inline int ToInt32(Handle<Value> i) {
    return i->Int32Value();
}

inline float ToFloat(Handle<Value> f) {
    return static_cast<float>(f->NumberValue());
}

void GraphicsTriangle(const FunctionCallbackInfo<Value> &args);

void init();

void change(int w, int h);

void draw();

Isolate *getIsolate();

Global<Context> *getGlobalContext();

Global<Function> *getGlobalFunction();


#endif //SUNGAMEENGINE_V8UTIL_H
