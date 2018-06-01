//
// Created by CoulsonChen on 2018/5/28.
//

#ifndef SUNGAMEENGINE_SUN_ENGINE_H
#define SUNGAMEENGINE_SUN_ENGINE_H

#include <GLES2/gl2.h>
#include "graphics/Matrix.h"


class sun_engine {
public:
    sun_engine();

    ~sun_engine();

    void init();

    void change(int width, int height);

    void draw();

private:

    Matrix *mViewMatrix;
    Matrix *mModelMatrix;
    Matrix *mProjectionMatrix;
    Matrix *mMVPMatrix;

    GLuint mProgram;

    GLuint mMVPMatrixHandle;
    GLuint mPositionHandle;
    GLuint mColorHandle;

};


#endif //SUNGAMEENGINE_SUN_ENGINE_H
