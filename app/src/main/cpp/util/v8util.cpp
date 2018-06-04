//
// Created by CoulsonChen on 2018/5/29.
//

#include "v8util.h"
#include <fstream>
#include <string>
#include <stdio.h>
#include <assert.h>
#include "sun_engine.h"
#include "graphics/GLUtils.h"
#include "include/v8.h"
#include "log.h"

namespace v8::internal {
    void ReadNatives() {}

    void DisposeNatives() {}

    void SetNativesFromFile(v8::StartupData *s) {}

    void SetSnapshotFromFile(v8::StartupData *s) {}
}

using namespace v8;
using namespace std;

static sun_engine *engine;


Local<Object> global;
Persistent<Object> sun;
Isolate *isolate;
Global<Context> context_;
Global<Function> function_;

Global<Context> *getGlobalContext() {
    return &context_;
}

Global<Function> *getGlobalFunction() {
    return &function_;
}

Isolate *getIsolate() {
    return isolate;
}

//Here is a helper function to ease the process - This inserts a named property with a callback
//into the object requested. For example, game.start <- this would be simpler using the function here
void
ExposeProperty(Handle<Object> intoObject, Handle<Value> namev8String, FunctionCallback funcID) {
    Locker lock(isolate);
    HandleScope handle_scope(isolate);

    intoObject->Set(namev8String, FunctionTemplate::New(isolate, funcID)->GetFunction());
}

//This will expose an object with the type Game, into the global scope.
//It will return a handle to the JS object that represents this c++ instance.
//Handle<Object> WrapGameObject( sun_engine *engine )
//{
//    // We will be creating temporary handles so we use a handle scope.
//    EscapableHandleScope handle_scope(isolate);
//
//    //This will persist when we leave the handle scope,
//    //meaning that the actual c++ instance is preserved
//    //Persistent<ObjectTemplate> class_template;
//
//    //This is just the preset for an emtpy object
////    Local<ObjectTemplate> raw_template = ObjectTemplate::New();
//
//    //This is so we can store a c++ object on it
////    raw_template->SetInternalFieldCount(1);
////
////    //Create the actual template object,
////    Persistent<ObjectTemplate> class_template(isolate,raw_template);
////    //class_template = Persistent<ObjectTemplate>::New( isolate,raw_template );
////
////    //Create an instance of the js object
////    Handle<Object> result = raw_template->NewInstance();
////
////    //Create a wrapper for the c++ instance
////    Handle<External> class_ptr = External::New( isolate,engine);
////
////    //Store the 'external c++ pointer' inside the JS object
////    result->SetInternalField( 0 , class_ptr );
//
//    //Return the JS object representing this class
//    return handle_scope.Escape(result);
//}
void GraphicsTriangle(const FunctionCallbackInfo<Value> &args) {
    if (engine != nullptr) {
        engine->draw();
    }
}

void initGL() {
    LOGI("init openGL");
    engine = new sun_engine();
    engine->init();
}

//draw triangle using openGL
void gl_draw() {
    if (engine != nullptr) {
        engine->draw();
    }
}

//exposed to javascript, it can be invoked in js context
void js_draw(const FunctionCallbackInfo<Value> &args) {
    LOGI("start draw ...");
    gl_draw();
}

// Extracts a C string from a V8 Utf8Value.
const char* ToCString(const v8::String::Utf8Value& value) {
    return *value ? *value : "<string conversion failed>";
}

void js_log(const FunctionCallbackInfo<Value> &args) {
    // LOGI("js log:%d",args.Length());
    String::Utf8Value utf8(isolate, args[0]);
    const char* cstr = ToCString(utf8);
    LOGI("js log:%s",cstr);
}

Isolate::CreateParams create_params;

void initv8() {
    LOGI("init v8 engine ...");
    Platform *platform = platform::CreateDefaultPlatform();
    V8::InitializePlatform(platform);
    V8::Initialize();
    create_params.array_buffer_allocator =
            ArrayBuffer::Allocator::NewDefaultAllocator();
    isolate = Isolate::New(create_params);
    Isolate::Scope isolate_scope(isolate);
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(isolate);

    Local<ObjectTemplate> global = ObjectTemplate::New(isolate);
    global->Set(String::NewFromUtf8(isolate, "log", NewStringType::kNormal)
                        .ToLocalChecked(),
                FunctionTemplate::New(isolate,js_log));
    global->Set(String::NewFromUtf8(isolate, "draw", NewStringType::kNormal)
                        .ToLocalChecked(),
                FunctionTemplate::New(isolate,js_draw));

    v8::Local<v8::Context> context = Context::New(isolate, NULL, global);
    context_.Reset(isolate, context);
    // Enter the new context so all the following operations take place
    // within it.
    Context::Scope context_scope(context);
    // Create a string containing the JavaScript source code.
    bool result = ExecuteString(context->GetIsolate(), "script/main.js", true, true);
    LOGI("JS Script Execute Result :%d", result);

    Local<String> process_name =
            String::NewFromUtf8(isolate, "onDraw", NewStringType::kNormal)
                    .ToLocalChecked();
    Local<Value> process_val;
    // If there is no Process function, or if it is not a function,
    if (!context->Global()->Get(context, process_name).ToLocal(&process_val) ||
        !process_val->IsFunction()) {
        LOGI("onDraw is not a function\n");
    }
    // It is a function; cast it to a Function
    Local<Function> process_fun = Local<Function>::Cast(process_val);
    function_.Reset(isolate, process_fun);



//    // Create a new context.
//    Local<Context> l_context = Context::New(isolate);
////    LOGI("init v8 engine local context");
//    Persistent<Context> p_context(isolate,l_context);
//    //context = p_context;
//    global = l_context->Global();
//    Context::Scope contextScope(l_context);
//    LOGI("init v8 engine context");

//    Handle<Object> jsGame = WrapGameObject( engine );
    //Now, we can easily expose the start function for js to use
//    ExposeProperty( jsGame, String::NewFromUtf8(isolate,"draw"), FunctionCallback( draw ) );
//    LOGI("init v8 engine ExposeProperty");
    //And finally, add the game object with its functions, into the global scope
//    global->Set( String::NewFromUtf8(isolate,"sun"), jsGame);
//    LOGI("init v8 engine end");
}

void init() {
    initGL();
    initv8();
}

//execute javascript source code
bool ExecuteString(Isolate *isolate, const char *path, bool print_result,
                   bool report_exceptions) {

    LOGI("ExecuteString ...");
    // Create filename

    char *jsChar = GLUtils::openTextFile(path);

    Local<String> source = String::NewFromUtf8(isolate, jsChar,
                                               NewStringType::kNormal).ToLocalChecked();

    HandleScope handle_scope(isolate);
    TryCatch try_catch(isolate);

    Local<Context> context(isolate->GetCurrentContext());
    Local<Script> script;
    if (!Script::Compile(context, source).ToLocal(&script)) {
        // Print errors that happened during compilation.
        if (report_exceptions)
            ReportException(isolate, &try_catch);
        return false;
    } else {
        Local<Value> result;
        if (!script->Run(context).ToLocal(&result)) {
            assert(try_catch.HasCaught());
            // Print errors that happened during execution.
            if (report_exceptions)
                ReportException(isolate, &try_catch);
            return false;
        } else {
            assert(!try_catch.HasCaught());
            if (print_result && !result->IsUndefined()) {
                // If all went well and the result wasn't undefined then print
                // the returned value.
                String::Utf8Value str(isolate, result);
                LOGI("===========\n%s\n", *str);
            }
            return true;
        }
    }
}

void ReportException(Isolate *isolate, TryCatch *try_catch) {
    HandleScope handle_scope(isolate);
    String::Utf8Value exception(isolate, try_catch->Exception());
//    const char *exception_string = ToCString(exception);
    Local<Message> message = try_catch->Message();
    if (message.IsEmpty()) {
        // V8 didn't provide any extra information about this error; just
        // print the exception.
        LOGI("exception_string ; %s\n", *exception);
    } else {
        // Print (filename):(line number): (message).
        String::Utf8Value filename(isolate,
                                   message->GetScriptOrigin().ResourceName());
        Local<Context> context(isolate->GetCurrentContext());
        int linenum = message->GetLineNumber(context).FromJust();
        LOGI("exception_string : %s:%i: %s\n", *filename, linenum, *exception);
        // Print line of source code.
        String::Utf8Value sourceline(
                isolate, message->GetSourceLine(context).ToLocalChecked());
        LOGI("stderr :%s\n", *sourceline);
        // Print wavy underline (GetUnderline is deprecated).
        int start = message->GetStartColumn(context).FromJust();
        for (int i = 0; i < start; i++) {
            fprintf(stderr, " ");
        }
        int end = message->GetEndColumn(context).FromJust();
        for (int i = start; i < end; i++) {
            fprintf(stderr, "^");
        }
        fprintf(stderr, "\n");
        Local<Value> stack_trace_string;
        if (try_catch->StackTrace(context).ToLocal(&stack_trace_string) &&
            stack_trace_string->IsString() &&
            Local<v8::String>::Cast(stack_trace_string)->Length() > 0) {
            String::Utf8Value stack_trace(isolate, stack_trace_string);
            LOGI("exception_string : %s\n\n", *stack_trace);
        }
    }

}

// surfaceView changed
void change(int w, int h) {
    if (engine != nullptr) {
        engine->change(w, h);
    }
}

//call js function onDraw()
void draw() {
    // Create a handle scope to keep the temporary object references.
    HandleScope handle_scope(getIsolate());

    v8::Local<v8::Context> context =
            v8::Local<v8::Context>::New(getIsolate(), *getGlobalContext());
    // Enter this processor's context so all the remaining operations
    // take place there
    Context::Scope context_scope(context);

    // Set up an exception handler before calling the Process function
    TryCatch try_catch(getIsolate());

    const int argc = 0;
    Local<Value> argv[argc] = {};
    v8::Local<v8::Function> process =
            v8::Local<v8::Function>::New(getIsolate(), *getGlobalFunction());
    Local<Value> result;
    if (!process->Call(context, context->Global(), argc, argv).ToLocal(&result)) {
        String::Utf8Value error(getIsolate(), try_catch.Exception());
        LOGI("call js function error:%s", *error);
    }
}

