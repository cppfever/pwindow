#pragma once

#include <vector>
#include "Window.hpp"


namespace vgui
{

class EventLoop
{
public:

    int Run()
    {
        while(Window::s_windows.size())
        {
            for(auto window : Window::s_windows)
                window->ProcessEvents();

            for(auto window : Window::s_windows)
                window->OnIdle();
        }

        return 0;
    }

};//class EventLoopT

} // namespace vgui
