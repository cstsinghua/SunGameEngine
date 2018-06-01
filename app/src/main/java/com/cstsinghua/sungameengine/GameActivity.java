package com.cstsinghua.sungameengine;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.os.Bundle;
import android.util.Log;

public class GameActivity extends Activity {

    private static final String TAG = "SunGameEngine";

    private MyGLSurfaceView mGLSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //create GLSurfaceView and setContentView

        mGLSurfaceView = new MyGLSurfaceView(this);

        setContentView(mGLSurfaceView);
    }

    @Override
    protected void onPause() {
        Log.d(TAG,"activity onPause");
        super.onPause();
        // The following call pauses the rendering thread.
        // If your OpenGL application is memory intensive,
        // you should consider de-allocating objects that
        // consume significant memory here.
        mGLSurfaceView.onPause();
    }

    @Override
    protected void onResume() {
        Log.d(TAG,"activity onResume");
        super.onResume();
        // The following call resumes a paused rendering thread.
        // If you de-allocated graphic objects for onPause()
        // this is a good place to re-allocate them.
        mGLSurfaceView.onResume();
    }
}
