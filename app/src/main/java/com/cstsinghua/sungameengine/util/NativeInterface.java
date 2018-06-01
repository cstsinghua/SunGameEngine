package com.cstsinghua.sungameengine.util;

/**
 * native interface for Java side
 */
public interface NativeInterface {
    void nativeInit();

    void nativeSurfaceChange(int width, int height);

    void nativeDrawFrame();
}
