#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <stdio.h>
#include <unistd.h>

int main(void)
{
    EGLNativeDisplayType native_display = fbGetDisplayByIndex(0);
    printf("Got display 0x%x\n", (intptr_t)native_display);

    int width, height;
    fbGetDisplayGeometry(native_display, &width, &height);
    printf("Got geometry %dx%d\n", width, height);

    EGLNativeWindowType native_window = fbCreateWindow(native_display, 0, 0, 0, 0);
    printf("Got window 0x%x\n", (intptr_t)native_window);

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
    printf("Got EGL display 0x%x\n", (intptr_t)display);

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
        else
        {
            printf(" -> SUCCESS\n");
        }

        if(i < num_configs - 1)
        {
            eglDestroySurface(display, surface);
        }
        //break;
    }

    const EGLint context_attrib_list_es2[] =
    {
        EGL_NONE,
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };


    EGLContext context = eglCreateContext(display, configs[54], EGL_NO_CONTEXT, context_attrib_list_es2);
    //EGLContext context = eglCreateContext(display, configs[i], EGL_NO_CONTEXT, context_attrib_list_es2);
    if (context == EGL_NO_CONTEXT)
    {
        printf("Could not get es2 context.\n");
        EGLint error = eglGetError();
        printf("eglCreateContext returned error Error 0x%x\n", error);
        return -1;

    }
    printf("Got context.\n");




    ret = eglMakeCurrent(display, surface, surface, context);
    if (ret == EGL_FALSE)
    {
        EGLint error = eglGetError();
        printf("eglMakeCurrent returned Error 0x%x\n", error);
        return -1;
    }
    printf("Made current.\n");

    sleep(2);

    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    ret = eglSwapBuffers(display, surface);
    if(ret == EGL_BAD_SURFACE)
    {
        EGLint error = eglGetError();
        printf("eglSwapBuffers EGL_BAD_SURFACE : Error 0x%x\n", error);
        return -1;
    }
    if(ret == EGL_NOT_INITIALIZED)
    {
        EGLint error = eglGetError();
        printf("eglSwapBuffers EGL_NOT_INITIALIZED : Error 0x%x\n", error);
        return -1;
    }
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

    return 0;
}
