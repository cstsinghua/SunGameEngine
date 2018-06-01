#include <jni.h>
#include <string>
#include "log.h"
#include "sun_engine.h"
#include "graphics/GLUtils.h"
#include "util/v8util.h"

static JNIEnv *sEnv = NULL;
static sun_engine *engine;

static void nativeInit(JNIEnv *env, jclass clazz,jobject assetManager){
    GLUtils::setEnvAndAssetManager(sEnv, assetManager);
//    engine = new sun_engine();
//    if(engine != nullptr){
//        engine->init();
//    }
    init();
}

static void nativeSurfaceChange(JNIEnv *env, jclass clazz,jint w,jint h){
//    if(engine != nullptr){
//        engine->change(width,height);
//    }
    LOGI("nativeSurfaceChange");
    change(w,h);
}

static void nativeDrawFrame(JNIEnv *env, jclass clazz){
//    if(engine != nullptr){
//        engine->draw();
//    }
    draw();
}
static const char *classPathName = "com/cstsinghua/sungameengine/util/GlueLib";
static JNINativeMethod methods[] = {
        {"nativeInit", "(Landroid/content/res/AssetManager;)V", (void*)nativeInit },
        {"nativeSurfaceChange", "(II)V", (void*)nativeSurfaceChange },
        {"nativeDrawFrame", "()V", (void*)nativeDrawFrame },
};
/*
 * Register several native methods for one class.
 */
static int registerNativeMethods(JNIEnv* env, const char* className,
                                 JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        LOGE("Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        LOGE("RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }
    return JNI_TRUE;
}
/*
 * Register native methods for all classes we know about.
 *
 * returns JNI_TRUE on success.
 */
static int registerNatives(JNIEnv* env)
{
    if (!registerNativeMethods(env, classPathName,
                               methods, sizeof(methods) / sizeof(methods[0]))) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

extern "C" JNIEXPORT jint
JNICALL
JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // Get jclass with env->FindClass.
    // Register methods with env->RegisterNatives.
    if (registerNatives(env) != JNI_TRUE) {
        LOGE("ERROR: registerNatives failed");
        return JNI_ERR;;
    }
    sEnv = env;
    return JNI_VERSION_1_6;
}


//extern "C"
//JNIEXPORT void JNICALL
//Java_com_cstsinghua_sungameengine_util_GlueLib_nativeInit(JNIEnv *env, jclass type) {
//
//    engine = new sun_engine();
//    if(engine != nullptr){
//        engine->init();
//    }
//
//}