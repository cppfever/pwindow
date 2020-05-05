#pragma once

#include "Win32Window.hpp"
#include "EventLoop.hpp"


namespace vgui
{

template<typename T>
class EventLoopT;

template<typename T>
class WindowBase : public Win32Window<T>
{
protected:

    void Create(const char* title, int x, int y, int width, int height) override
    {
        Win32Window<T>::Create(title, x, y, width, height);
        AddToEventLoop(this);
    }

    void Destroy()
    {
        RemoveFromEventLoop(this);
        Win32Window<T>::Destroy();
    }

    int Run()
    {
        while(s_windows.size())
        {
            for(auto window : s_windows)
                window->ProcessEvents();

            for(auto window : s_windows)
                window->OnIdle();
        }

        return 0;
    }

private:    

    static void AddToEventLoop(WindowBase* window)
    {
        auto i = std::find(s_windows.begin(), s_windows.end(), window);

        if( i== s_windows.end())
            s_windows.push_back(window);
    }

    static void RemoveFromEventLoop(WindowBase* window)
    {
        auto i = std::find(s_windows.begin(), s_windows.end(), window);

        if( i!= s_windows.end())
            s_windows.erase(i);
    }

    friend class vgui::EventLoopT<T>;
    static inline std::vector<vgui::WindowBase<T>*> s_windows;

};//class WindowBase

}//namespace vgui

