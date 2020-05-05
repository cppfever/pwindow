#pragma once

#include "Win32Window.hpp"


namespace vgui
{

class Window : public Win32Window<Window>
{
public:

    Window(const char* title, int x, int y, int width, int height)
    {
        Win32Window<Window>::Create(title, x, y, width, height);
    }

    virtual ~Window()
    {
        Win32Window<Window>::Destroy();
    }

protected:


    void OnDraw() override
    {
        ::glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
        ::glClear(GL_COLOR_BUFFER_BIT);

        int width = Width();
        int height = Height();

        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glOrtho(0.0f, width, height, 0.0f, 0.0f, 1.0f);
        glMatrixMode(GL_MODELVIEW);
    }

    void OnSize(int width, int height) override
    {
    }

};//class Window


}//namespace vgui

