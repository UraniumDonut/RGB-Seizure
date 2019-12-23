#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <switch.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <glad/glad.h>

#define TRACE(fmt,...) ((void)0)


//-----------------------------------------------------------------------------
// EGL initialization
//-----------------------------------------------------------------------------

static EGLDisplay s_display;
static EGLContext s_context;
static EGLSurface s_surface;

static bool initEgl(NWindow* win)
{
    // Connect to the EGL default display
    s_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (!s_display)
    {
        TRACE("Could not connect to display! error: %d", eglGetError());
        goto _fail0;
    }

    // Initialize the EGL display connection
    eglInitialize(s_display, nullptr, nullptr);

    // Select OpenGL (Core) as the desired graphics API
    if (eglBindAPI(EGL_OPENGL_API) == EGL_FALSE)
    {
        TRACE("Could not set API! error: %d", eglGetError());
        goto _fail1;
    }

    // Get an appropriate EGL framebuffer configuration
    EGLConfig config;
    EGLint numConfigs;
    static const EGLint framebufferAttributeList[] =
    {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_RED_SIZE,     8,
        EGL_GREEN_SIZE,   8,
        EGL_BLUE_SIZE,    8,
        EGL_ALPHA_SIZE,   8,
        EGL_DEPTH_SIZE,   24,
        EGL_STENCIL_SIZE, 8,
        EGL_NONE
    };
    eglChooseConfig(s_display, framebufferAttributeList, &config, 1, &numConfigs);
    if (numConfigs == 0)
    {
        TRACE("No config found! error: %d", eglGetError());
        goto _fail1;
    }

    // Create an EGL window surface
    s_surface = eglCreateWindowSurface(s_display, config, win, nullptr);
    if (!s_surface)
    {
        TRACE("Surface creation failed! error: %d", eglGetError());
        goto _fail1;
    }

    // Create an EGL rendering context
    static const EGLint contextAttributeList[] =
    {
        EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR,
        EGL_CONTEXT_MAJOR_VERSION_KHR, 4,
        EGL_CONTEXT_MINOR_VERSION_KHR, 3,
        EGL_NONE
    };
    s_context = eglCreateContext(s_display, config, EGL_NO_CONTEXT, contextAttributeList);
    if (!s_context)
    {
        TRACE("Context creation failed! error: %d", eglGetError());
        goto _fail2;
    }

    // Connect the context to the surface
    eglMakeCurrent(s_display, s_surface, s_surface, s_context);
    return true;

_fail2:
    eglDestroySurface(s_display, s_surface);
    s_surface = nullptr;
_fail1:
    eglTerminate(s_display);
    s_display = nullptr;
_fail0:
    return false;
}

static void deinitEgl()
{
    if (s_display)
    {
        eglMakeCurrent(s_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (s_context)
        {
            eglDestroyContext(s_display, s_context);
            s_context = nullptr;
        }
        if (s_surface)
        {
            eglDestroySurface(s_display, s_surface);
            s_surface = nullptr;
        }
        eglTerminate(s_display);
        s_display = nullptr;
    }
}

//-----------------------------------------------------------------------------
// Main program
//-----------------------------------------------------------------------------

static GLuint s_program;
static GLuint s_vbo;

int main(int argc, char* argv[])
{
	setenv("MESA_NO_ERROR", "1", 1);

	svcOutputDebugString("<<<<<<<<<<<<<<-`````````````````````````````````````````````````````````````````````````-<<<<<<<<<<<<<<", 105);
	svcOutputDebugString("<<<<<<<<<<<<<<-`.......................................................................`-<<<<<<<<<<<<<<", 105);
	svcOutputDebugString("=<<<<<<<<<<<<<-`.....\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"-....`-<<<<<<<<<<<<<<", 105);
	svcOutputDebugString("<<<<<*\"\"*<<<<<-`.....\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"-....`-<<<<<*,,*<<<<:", 105);
	svcOutputDebugString("<<<:,....,:<<<-`.....\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"-....`-<<:\":;\"\";:\":<<", 105);
	svcOutputDebugString("<<<*......*<<<-`.....\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"-....`-<<-.-<<<<-.-<<", 105);
	svcOutputDebugString("<<<<*`.,`*<<<<-`.....\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"-....`-<<<<<<<<<<<<<<", 105);
	svcOutputDebugString("<<<<<<<<<<<<<<-`.....\"\"\"\"\"\"\"\"\"\"\"\"\"\"                                  \"\"\"\"\"\"\"\"\"\"\"\"\"-....`-<<<<<<<<<<<<<<", 105);
	svcOutputDebugString("<<<<<<<<<<<<<<-`.....\"\"\"\"\"\"\"\"\"\"\"\"\"\"         Prepare your eyes        \"\"\"\"\"\"\"\"\"\"\"\"\"-....`-<<<<<<<<<<<<<<", 105);
	svcOutputDebugString("<<<<<*,,*<<<<<-`.....\"\"\"\"\"\"\"\"\"\"\"\"\"\"  Things are about to get flashy  \"\"\"\"\"\"\"\"\"\"\"\"\"-....`-<<<<<*\"\"*<<<<<", 105);
	svcOutputDebugString("<<:;:;;;;:;:<<-`.....\"\"\"\"\"\"\"\"\"\"\"\"\"\"                                  \"\"\"\"\"\"\"\"\"\"\"\"\"-....`-<<<:,....,:<<<", 105);
	svcOutputDebugString("<<-.-<<<<-.-<<-`.....\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"-....`-<<<\"......\"<<<", 105);
	svcOutputDebugString("<<<<<*,,*<<<<<-`.....\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"-....`-<<<<\"`,.`\"<<<<", 105);
	svcOutputDebugString("<<<<<<<<<<<<<<-`.....\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"-....`-<<<<<<<<<<<<<<", 105);
	svcOutputDebugString("<<<<<<<<<<<<<<-`.....\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"-....`-<<<<<<<<<<<<<<", 105);
	svcOutputDebugString("<<<<<<<<<<<<<<-`.....\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"-....`-<<<<<<<<<<<<<<", 105);
	svcOutputDebugString("<<<<<<<<<<<<<<-`.....\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"-....`-<<<<<<<<<<<<<<", 105);
	svcOutputDebugString("<<<<<<<<<<<<<<-`.....\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"-....`-<<<<<<<<<<<<<<", 105);
	svcOutputDebugString("<<<<<<<<<<<<<<-`.....\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"-....`-<<<<<<<<<<<<<<", 105);
	svcOutputDebugString("<<<<<<<<<<<<<<-`.......................................................................`-<<<<<<<<<<<<<<", 105);
	svcOutputDebugString("<<<<<<<<<<<<<<-`.......................................................................`-<<<<<<<<<<<<<<", 105);
	svcOutputDebugString("$$$%/;;;;;;;;;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;;;;;;;;;!?$$$", 105);
	
	if (!initEgl(nwindowGetDefault()))
        return EXIT_FAILURE;

    gladLoadGL();

	u32 cnt = 0;

    while (appletMainLoop())
    {
        hidScanInput();
        u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        if (kDown & KEY_PLUS)
            break;

		if (cnt != 60)
			cnt = cnt + 6;
		else
			cnt = 0;

		if (cnt <= 20)
			glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		else if (cnt <= 40)
			glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		else
			glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);

        eglSwapBuffers(s_display, s_surface);
    }

	glDeleteBuffers(1, &s_vbo);
	glDeleteProgram(s_program);

    deinitEgl();
    return EXIT_SUCCESS;
}
