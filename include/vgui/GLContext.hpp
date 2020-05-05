#pragma once

#include "3dparty.hpp"
#include "Exception.hpp"


namespace vgui
{

class GLContext
{
public:

    HGLRC Handle() const
    {
        return m_glrc;
    }

    bool IsValid()
    {
        if(m_glrc)
            return true;
        else
            return false;
    }

    virtual void Create(HDC hdc)
    {
        if(IsValid())
            throw ExceptionInfo << "OpenGL context already exists.";

        //Screen HDC
        HDC screendc = ::GetDC(NULL);
        if(!screendc)
            throw ExceptionInfo << "Can't create desktop device context.";

        int colorbits = ::GetDeviceCaps(screendc, BITSPIXEL);
        if(!colorbits)
            throw ExceptionInfo << "Can't get colorbits of desktop device context.";

        if(!::ReleaseDC(NULL, screendc))
            throw ExceptionInfo << "Can't release desktop device context.";
        screendc = NULL;

        PIXELFORMATDESCRIPTOR pfd{0};
        pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion   = 1;
        pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = colorbits;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;
        pfd.iLayerType = PFD_MAIN_PLANE;

        int pixelformat = ::ChoosePixelFormat(hdc, &pfd);
        if(!pixelformat)
            throw ExceptionInfo << "Can't choose OpenGL pixelformat.";

        if(!::SetPixelFormat(hdc, pixelformat, &pfd))
            throw ExceptionInfo << "Can't set desktop pixelformat.";

        HGLRC tmp_context = wglCreateContext(hdc);

        if(!tmp_context)
            throw ExceptionInfo << "Can't create OpenGL temporary context.";

        if(!::wglMakeCurrent(hdc, tmp_context))
            throw ExceptionInfo << "Can't make current OpenGL context.";

        if(::glewInit() != GLEW_OK)
            throw ExceptionInfo << "GLEW initialization error.";

        int attribs[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 2,
            WGL_CONTEXT_MINOR_VERSION_ARB, 0,
            WGL_CONTEXT_FLAGS_ARB, 0,
            0
        };

        if(!wglewIsSupported("WGL_ARB_create_context"))
            throw ExceptionInfo << "WGL_ARB_create_context error.";

        m_glrc = ::wglCreateContextAttribsARB(hdc,0, attribs);
        if(!m_glrc)
            throw ExceptionInfo << "Can't create extended OpneGL context.";

        UnmakeCurrent();

        if(!::wglDeleteContext(tmp_context))
            throw ExceptionInfo << "Can't delete temporary OpneGL context.";

        MakeCurrent(hdc);
        /*
        ::glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
        ::glEnable(GL_STENCIL_BITS);
        ::glClearStencil(0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        RECT rect;
        ::GetClientRect(::WindowFromDC(hdc), &rect);
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;

        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glOrtho(0.0f, width, height, 0.0f, 0.0f, 1.0f);
        glMatrixMode(GL_MODELVIEW);
*/
        UnmakeCurrent();
    }

    virtual void Destroy()
    {
        if(IsValid())
        {
            UnmakeCurrent();

            if(!::wglDeleteContext(m_glrc))
                throw ExceptionInfo << "Can't delete OpenGL context.";

            m_glrc = NULL;
        }
    }

    virtual void MakeCurrent(HDC dc)
    {
        if(!::wglMakeCurrent(dc, m_glrc))
            throw ExceptionInfo << "Can't make current OpenGL context.";
    }

    virtual void UnmakeCurrent()
    {
        if(!::wglMakeCurrent(::wglGetCurrentDC(), ::wglGetCurrentContext()))
            throw ExceptionInfo << "Can't unmake current OpenGL context.";
    }

    void SwapBuffers(HDC dc)
    {
        if(!::SwapBuffers(dc))
            throw ExceptionInfo << "Can't swap buffers of OpenGL context.";
    }

private:

    HGLRC m_glrc{NULL};
};//class GLContext

}//namespace vgui

