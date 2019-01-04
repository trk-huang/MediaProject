//
// Created by 黄大举 on 2018/12/28.
//

#ifndef TUYAMEDIAPROJECT_EGLUTILS_H
#define TUYAMEDIAPROJECT_EGLUTILS_H

#include <EGL/egl.h>

class EGLUtils {

    public:

        EGLUtils();
        ~EGLUtils();

        void initEGL(ANativeWindow *nativeWindow);
        void drawEGL();
        int getWidth();
        int getHeight();

    private:
        EGLConfig eglConf;
        EGLSurface eglWindow;
        EGLContext eglCtx;
        EGLDisplay eglDisp;

        int windowWidth;
        int windowHeight;

        void releaseEGL();

};


#endif //TUYAMEDIAPROJECT_EGLUTILS_H
