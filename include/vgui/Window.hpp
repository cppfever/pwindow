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
        int width = Width();
        int height = Height();
    }

    void OnSize(int width, int height) override
    {
    }

};//class Window


}//namespace vgui

