//
// Created by CoulsonChen on 2018/5/28.
//

#include "sun_engine.h"
#include "graphics/GLUtils.h"
#include "log.h"

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s \n", name, v);
}

static void checkGlError(const char *op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

const char *VERTEX_SHADER =
        "uniform mat4 u_MVPMatrix;        \n"     // A constant representing the combined model/view/projection matrix.
                "attribute vec4 a_Position;     \n"     // Per-vertex position information we will pass in.
                "attribute vec4 a_Color;        \n"     // Per-vertex color information we will pass in.
                "varying vec4 v_Color;          \n"     // This will be passed into the fragment shader.
                "void main()                    \n"     // The entry point for our vertex shader.
                "{                              \n"
                "   v_Color = a_Color;          \n"     // Pass the color through to the fragment shader.
                "   gl_Position = u_MVPMatrix * a_Position; \n"     // gl_Position is a special variable used to store the final position.
                "}                              \n";    // normalized screen coordinates.


const char *FRAGMENT_SHADER = "precision mediump float;         \n"     // Set the default precision to medium. We don't need as high of a
        "varying vec4 v_Color;          \n"     // This is the color from the vertex shader interpolated across the
        "void main()                    \n"     // The entry point for our fragment shader.
        "{                              \n"
        "   gl_FragColor = v_Color;     \n"     // Pass the color directly through the pipeline.
        "}                              \n";

// This triangle is red, green, and blue.
GLfloat triangleVerticesData[] = {
        // X, Y, Z,
        // R, G, B, A
        -0.5f, -0.25f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f,

        0.5f, -0.25f, 0.0f,
        0.0f, 0.0f, 1.0f, 1.0f,

        0.0f, 0.559016994f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f};

sun_engine::sun_engine() {
    mModelMatrix = NULL;
    mMVPMatrix = NULL;
    mProjectionMatrix = NULL;
    mViewMatrix = NULL;
}

sun_engine::~sun_engine() {
    delete mModelMatrix;
    mModelMatrix = NULL;
    delete mMVPMatrix;
    mMVPMatrix = NULL;
    delete mProjectionMatrix;
    mProjectionMatrix = NULL;
    delete mViewMatrix;
    mViewMatrix = NULL;
}

void sun_engine::init() {

    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    mProgram = GLUtils::createProgram(&VERTEX_SHADER, &FRAGMENT_SHADER);
    if (!mProgram) {
        LOGD("Could not create program");
        return;
    }
    // LOGI("glprogram index:%d",mProgram);
    mModelMatrix = new Matrix();
    mMVPMatrix = new Matrix();

    // Position the eye in front of the origin.
    float eyeX = 0.0f;
    float eyeY = 0.0f;
    float eyeZ = 1.5f;

    // We are looking at the origin
    float centerX = 0.0f;
    float centerY = 0.0f;
    float centerZ = 0.0f;

    // Set our up vector.
    float upX = 0.0f;
    float upY = 1.0f;
    float upZ = 0.0f;

    // Set the view matrix.
    mViewMatrix = Matrix::newLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
}
void sun_engine::change(int width, int height) {

    glViewport(0, 0, width, height);

    // Create a new perspective projection matrix. The height will stay the same
    // while the width will vary as per aspect ratio.
    float ratio = (float) width / height;
    float left = -ratio;
    float right = ratio;
    float bottom = -1.0f;
    float top = 1.0f;
    float near = 1.0f;
    float far = 2.0f;

    mProjectionMatrix = Matrix::newFrustum(left, right, bottom, top, near, far);
}

void sun_engine::draw() {
    glClearColor(1.0F, 1.0F, 1.0F, 1.5F);//red
    glClear(GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    glUseProgram(mProgram);

    mMVPMatrixHandle = (GLuint) glGetUniformLocation(mProgram, "u_MVPMatrix");
    mPositionHandle = (GLuint) glGetAttribLocation(mProgram, "a_Position");
    mColorHandle = (GLuint) glGetAttribLocation(mProgram, "a_Color");

    long time = GLUtils::currentTimeMillis() % 10000L;
    float angleInDegrees = (360.0f / 10000.0f) * ((int) time);

    // Draw the triangle facing straight on.
    mModelMatrix->identity();
    mModelMatrix->rotate(angleInDegrees, 0.0f, 0.0f, 1.0f);
    glVertexAttribPointer(
            (GLuint) mPositionHandle,
            3,
            GL_FLOAT,
            GL_FALSE,
            4 * 7,
            triangleVerticesData
    );
    glEnableVertexAttribArray((GLuint) mPositionHandle);

    glVertexAttribPointer(
            (GLuint) mColorHandle,
            4,
            GL_FLOAT,
            GL_FALSE,
            4 * 7,
            triangleVerticesData + 3
    );
    glEnableVertexAttribArray((GLuint) mColorHandle);

    // model * view
    mMVPMatrix->multiply(*mViewMatrix, *mModelMatrix);

    // model * view * projection
    mMVPMatrix->multiply(*mProjectionMatrix, *mMVPMatrix);

    glUniformMatrix4fv(mMVPMatrixHandle, 1, GL_FALSE, mMVPMatrix->mData);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    // LOGI("draw triangle");
    checkGlError("glDrawArrays");
}

