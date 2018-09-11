#include <GLES/egl.h>
#include <GLES/gl.h>

#include <stdio.h>
#include <unistd.h>


int main(void)
{
    int width, height;
    EGLNativeDisplayType native_display;
    EGLNativeWindowType native_window;

    /* =========================================================================== */
    /* Native Windowing System adapter                                             */
    /* =========================================================================== */

    native_display = fbGetDisplayByIndex(0); // Vivante fbdev api
    printf("Got native display 0x%x\n", (unsigned int)native_display);
  
    fbGetDisplayGeometry(native_display, &width, &height); // Vivante fbdev api
    printf("Got display geometry %dx%d\n", width, height);

    native_window = fbCreateWindow(native_display, 0, 0, 0, 0); // Vivante fbdev api
    printf("Got native window 0x%x\n", (unsigned int)native_window);

    /* =========================================================================== */

    EGLBoolean ret;
    ret = eglBindAPI(EGL_OPENGL_ES_API);
    if (ret == EGL_FALSE)
    {
        EGLint error = eglGetError();
        printf("Error 0x%x\n", error);
        return -1;
    }
    printf("Bound API\n");

    //EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLDisplay display = eglGetDisplay(native_display);
    if (display == EGL_NO_DISPLAY)
    {
        EGLint error = eglGetError();
        printf("Error 0x%x\n", error);
        return -1;
    }
    printf("Got EGL display 0x%x\n", (unsigned int)display);

    EGLint major, minor;
    ret = eglInitialize(display, &major, &minor);
    if (ret == EGL_FALSE)
    {
        EGLint error = eglGetError();
        printf("Error 0x%x\n", error);
        return -1;
    }
    printf("Got version %d.%d\n", major, minor);

    EGLint num_configs;
    ret = eglGetConfigs(display, NULL, 0, &num_configs);
    if (ret == EGL_FALSE)
    {
        EGLint error = eglGetError();
        printf("Error 0x%x\n", error);
        return -1;
    }
    printf("Has %d configurations\n", num_configs);

    const EGLint config_attrib_list[] =
    {
        EGL_NONE,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_SAMPLES, 0,
        EGL_STENCIL_SIZE, 0,
        EGL_DEPTH_SIZE, 16,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };
    EGLConfig configs[100];
    ret = eglChooseConfig(display, config_attrib_list, configs, 100, &num_configs);
    if (ret == EGL_FALSE)
    {
        EGLint error = eglGetError();
        printf("Error 0x%x\n", error);
        return -1;
    }
    //printf("(%d=%d)\n", EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT);
    printf("Accepted %d configurations\n", num_configs);

    EGLSurface surface;
    int i;
    for (i = 0; i < num_configs; ++i)
    {
        int red_size = 0, green_size = 0, blue_size = 0, alpha_size = 0;
        int depth_size = 0, stencil_size = 0;
        int samples = 0, id = 0;
        int opengl_bit = 0, surface_bit = 0;

        eglGetConfigAttrib(display, configs[i], EGL_RED_SIZE, &red_size);
        eglGetConfigAttrib(display, configs[i], EGL_GREEN_SIZE, &green_size);
        eglGetConfigAttrib(display, configs[i], EGL_BLUE_SIZE, &blue_size);
        eglGetConfigAttrib(display, configs[i], EGL_ALPHA_SIZE, &alpha_size);
        eglGetConfigAttrib(display, configs[i], EGL_DEPTH_SIZE, &depth_size);
        eglGetConfigAttrib(display, configs[i], EGL_STENCIL_SIZE, &stencil_size);
        eglGetConfigAttrib(display, configs[i], EGL_SAMPLES, &samples);
        eglGetConfigAttrib(display, configs[i], EGL_RENDERABLE_TYPE, &opengl_bit);
        eglGetConfigAttrib(display, configs[i], EGL_CONFIG_ID, &id);
        eglGetConfigAttrib(display, configs[i], EGL_SURFACE_TYPE, &surface_bit);

        printf("Trying id=%d rgba=%d%d%d%d d=%d, s=%d aa=%d opengl=%d surface=%d",
               id,
               red_size, green_size, blue_size, alpha_size,
               depth_size, stencil_size,
               samples, opengl_bit, surface_bit);
        fflush(stdout);

        surface = eglCreateWindowSurface(display, configs[i], native_window, NULL);
        if (surface == EGL_NO_SURFACE)
        {
            EGLint error = eglGetError();
            printf(" -> Error 0x%x\n", error);
            if (i == num_configs - 1)
            {
                //return -1;
            }
            else
            {
                //continue;
            }
        }
        eglDestroySurface(display, surface);
        printf(" -> SUCCESS\n");
        //break;
    }

    const EGLint context_attrib_list[] =
    {
        //EGL_NONE,
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    EGLContext context = eglCreateContext(display, configs[i], EGL_NO_CONTEXT, context_attrib_list);
    if (context == EGL_NO_CONTEXT)
    {
        EGLint error = eglGetError();
        printf("Error 0x%x\n", error);
        return -1;
    }
    printf("Got context\n");

    ret = eglMakeCurrent(display, surface, surface, context);
    if (ret == EGL_FALSE)
    {
        EGLint error = eglGetError();
        printf("Error 0x%x\n", error);
        return -1;
    }
    printf("Made current\n");

    sleep(2);

    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    eglSwapBuffers(display, surface);
    sleep(2);

    glClearColor(0.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    eglSwapBuffers(display, surface);
    sleep(2);

    glClearColor(0.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    eglSwapBuffers(display, surface);
    sleep(6);

    printf("Cleared screen\n");

    ret = eglDestroyContext(display, context);
    if (ret == EGL_FALSE)
    {
        EGLint error = eglGetError();
        printf("Error 0x%x\n", error);
        return -1;
    }
    printf("Destroyed context\n");

    ret = eglDestroySurface(display, surface);
    if (ret == EGL_FALSE)
    {
        EGLint error = eglGetError();
        printf("Error 0x%x\n", error);
        return -1;
    }
    printf("Destroyed surface\n");

    eglTerminate(display);
    if (ret == EGL_FALSE)
    {
        EGLint error = eglGetError();
        printf("Error 0x%x\n", error);
        return -1;
    }
    printf("Terminated\n");

   // TODO: close native window & display

    return 0;
}
