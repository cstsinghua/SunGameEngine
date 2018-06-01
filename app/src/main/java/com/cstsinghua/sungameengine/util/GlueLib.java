package com.cstsinghua.sungameengine.util;

import android.content.res.AssetManager;
public class GlueLib{

    // Used to load the 'sun' library on application startup.
    static {
        System.loadLibrary("sun");
    }

    /**
     * initialize v8 and openGL
     */
    public native static void nativeInit(AssetManager assetManager);

    /**
     * openGL surfaceview changed,notify to glviewport and generate projection matrix
     * @param width
     * @param height
     */
    public native static void nativeSurfaceChange(int width, int height);

    /**
     * draw graphics per frame
     */
    public native static void nativeDrawFrame();
}
