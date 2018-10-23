#include <wayland-client.h>
#include <wayland-egl.h> 

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <assert.h>

struct globals{
    struct wl_compositor *compositor;
    struct wl_shell *shell;
};

struct globals g_globals;

static void registry_add_object (void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version) {
    if (!strcmp(interface,"wl_compositor")) {
        g_globals.compositor = (wl_compositor *)wl_registry_bind (registry, name, &wl_compositor_interface, 1);
    }
    else if (!strcmp(interface,"wl_shell")) {
        g_globals.shell =(wl_shell *)wl_registry_bind (registry, name, &wl_shell_interface, 1);
    }
}
static void registry_remove_object (void *data, struct wl_registry *registry, uint32_t name) {

}

int main(void)
{
    int width, height;
    static struct wl_display *native_display;
    EGLNativeWindowType native_window;

    /* =========================================================================== */
    /* Native Windowing System adapter                                             */
    /* =========================================================================== */
    native_display = wl_display_connect(NULL);
    struct wl_registry *registry = wl_display_get_registry (native_display);
    static struct wl_registry_listener registry_listener = {&registry_add_object, &registry_remove_object};
    wl_registry_add_listener(registry, &registry_listener, (void *) &g_globals);
    wl_display_dispatch(native_display);
    wl_display_roundtrip (native_display);
    
    struct wl_surface *surface_wl;
    surface_wl = wl_compositor_create_surface(g_globals.compositor);
    struct wl_shell_surface* shellSurface = wl_shell_get_shell_surface(g_globals.shell,surface_wl);
    wl_shell_surface_set_toplevel(shellSurface);

    if (native_display == NULL) {
	fprintf(stderr, "Can't connect to display\n");
	return -1;
    }
    printf("connected to display\n");
    native_window = wl_egl_window_create(surface_wl,
			      1920, 1080);
    if (native_window == EGL_NO_SURFACE) {
	fprintf(stderr, "Can't create native window\n");
	return -1;
    } else {
	fprintf(stderr, "Created native window\n");
    }

    /* =========================================================================== */

    EGLBoolean ret;
    const EGLint context_attrib_list[] =
    {
        //EGL_NONE,
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

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
        printf("Error 0x%x\n EGL_NO_DISPLAY", error);
        return -1;
    }
    printf("Got EGL display 0x%x\n", (intptr_t)display);

    EGLint major, minor;
    ret = eglInitialize(display, &major, &minor);
    if (ret == EGL_FALSE)
    {
        EGLint error = eglGetError();
        printf("Error 0x%x\n EGL_FALSE", error);
        return -1;
    }
    printf("Got version %d.%d\n", major, minor);

    EGLint num_configs;
    EGLConfig configs[100];
    ret = eglGetConfigs(display,NULL, 0, &num_configs);
    if (ret == EGL_FALSE)
    {
        EGLint error = eglGetError();
        printf("Error 0x%x\n", error);
        return -1;
    }
    printf("Has %d configurations\n", num_configs);

    const EGLint config_attrib_list[] =
    {
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
    ret = eglChooseConfig(display, config_attrib_list, configs, 100, &num_configs);
    if (ret == EGL_FALSE)
    {
        EGLint error = eglGetError();
        printf("Error 0x%x\n", error);
        return -1;
    }
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

        printf("Trying id=%d rgba=%d%d%d%d d=%d, s=%d aa=%d opengl=%d surface=%d\n",
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
        EGLContext context = eglCreateContext(display, configs[i], EGL_NO_CONTEXT, context_attrib_list);
        if (context == EGL_NO_CONTEXT)
        {
            EGLint error = eglGetError();
            printf("Error 0x%x\n", error);
            continue;
        }
        printf("Got context\n");

        ret = eglMakeCurrent(display, surface, surface, context);
        if (ret == EGL_FALSE)
        {
            EGLint error = eglGetError();
            printf("Error 0x%x\n", error);
            continue;
        }
        printf("Made current\n");

        sleep(1);

        glClearColor(1.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
        eglSwapBuffers(display, surface);
        sleep(1);

        glClearColor(0.0, 1.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
        eglSwapBuffers(display, surface);
        sleep(1);

        glClearColor(0.0, 0.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
        eglSwapBuffers(display, surface);
        sleep(1);

        printf("Cleared screen\n");

        ret = eglDestroyContext(display, context);
        if (ret == EGL_FALSE)
        {
            EGLint error = eglGetError();
            printf("Error 0x%x\n", error);
            continue;
        }
        printf("Destroyed context\n");

        ret = eglDestroySurface(display, surface);
        if (ret == EGL_FALSE)
        {
            EGLint error = eglGetError();
            printf("Error 0x%x\n", error);
            continue;
        }
        printf("Destroyed surface\n");


        eglDestroySurface(display, surface);
        printf(" -> SUCCESS\n");
        //break;
    }

    ret = eglTerminate(display);
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
