#pragma once

#include <windows.h>
#include <dwmapi.h>

#include "Point.hpp"
#include "Size.hpp"
#include "Rect.hpp"
#include "Exception.hpp"
#include "GLContext.hpp"


namespace vgui
{

template<typename WindowT>
class Win32Window
{
public:

    HWND Handle()
    {
        return m_handle;
    }

    vgui::GLContext& GLContext()
    {
        return m_context;
    }

    bool IsValid()
    {
        if(::IsWindow(m_handle))
            return true;
        else
            return false;
    }

    void Close()
    {
        ::PostQuitMessage(0);
    }

    int X()
    {
        return Position().x;
    }

    int Y()
    {
        return Position().y;
    }

    vgui::PointI Position()
    {
        RECT rect;
        ::GetWindowRect(m_handle, &rect);
        return vgui::PointI(rect.left, rect.top);
    }

    int Width()
    {
        return Size().width;
    }

    int Height()
    {
        return Size().height;
    }

    vgui::SizeI Size()
    {
        RECT rect;
        ::GetWindowRect(m_handle, &rect);
        return vgui::SizeI(rect.right - rect.left, rect.bottom - rect.top);
    }

    vgui::RectI Rect()
    {
        RECT rect;
        ::GetWindowRect(m_handle, &rect);
        return vgui::RectI(rect.left, rect.top, rect.right, rect.bottom);
    }

    bool Visible()
    {
        return static_cast<bool>(::IsWindowVisible(m_handle));
    }

    WindowT& SetX(int x)
    {
        vgui::RectI rect = Rect();
        return SetRect(x, rect.y, rect.width, rect.height);
    }

    WindowT& SetY(int y)
    {
        vgui::RectI rect = Rect();
        return SetRect(rect.x, y, rect.width, rect.height);
    }

    WindowT& SetPosition(int x, int y)
    {
        vgui::SizeI size = Size();
        return SetRect(x, y, size.width, size.height);
    }

    WindowT& SetPosition(Point position)
    {
        vgui::SizeI size = Size();
        return SetRect(position.x, position.y, size.width, size.height);
    }

    WindowT& SetWidth(int width)
    {
        vgui::RectI rect = Rect();
        return SetRect(rect.x, rect.y, width, rect.height);
    }

    WindowT& SetHeight(int height)
    {
        vgui::RectI rect = Rect();
        return SetRect(rect.x, rect.y, rect.width, height);
    }

    WindowT& SetSize(int width, int height)
    {
        vgui::PointI pos = Position();
        return SetRect(pos.x, pos.y, width, height);
    }

    WindowT& SetSize(vgui::SizeI size)
    {
        vgui::PointI pos = Position();
        return SetRect(pos.x, pos.y, size.width, size.height);
    }

    WindowT& SetRect(vgui::RectI rect)
    {
        return SetRect(rect.x, rect.y, rect.width, rect.height);
    }

    WindowT& SetRect(int x, int y, int width, int height)
    {
        ::MoveWindow(m_handle, x, y, width, height, true);
        return *CastThis();
    }

    WindowT& SetVisible(bool show)
    {
        if(show)
            ::ShowWindow(m_handle, SW_SHOW);
        else
            ::ShowWindow(m_handle, SW_HIDE);

        return *CastThis();
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

protected:

    WindowT* CastThis()
    {
        return dynamic_cast<WindowT*>(this);
    }

    virtual void OnIdle()
    {
        if(m_context.IsValid())
            OnDraw();
    }

    virtual void OnDraw(){}
    virtual void OnSize(int width, int height){}
    virtual void OnLostFocus(){}
    virtual void OnGainedFocus(){}
    virtual void OnTextEnter(){}
    virtual void OnKeyPress(){}
    virtual void OnKeyRelease(){}
    virtual void OnMouseWheelMove(){}
    virtual void OnMouseWheelScroll(){}
    virtual void OnMouseButtonPress(){}
    virtual void OnMouseButtonRelease(){}
    virtual void OnMouseMove(){}
    virtual void OnMouseEnter(){}
    virtual void OnMouseLeft(){}
    virtual void OnJoystickButtonPress(){}
    virtual void OnJoystickButtonRelease(){}
    virtual void OnJoystickMove(){}
    virtual void OnJoystickConnect(){}
    virtual void OnJoystickDisconnect(){}
    virtual void OnTouchBegin(){}
    virtual void OnTouchMove(){}
    virtual void OnTouchEnd(){}
    virtual void OnSensorChange(){}
    virtual void OnClose(bool& close){}

    virtual bool ProcessEvent(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam, LRESULT& result)
    {
        if(handle == m_handle)
        {
            if(msg == WM_PAINT)
            {
                PAINTSTRUCT ps;
                HDC dc = ::BeginPaint(m_handle, &ps);
                m_context.MakeCurrent(dc);
                OnDraw();
                m_context.SwapBuffers(dc);
                m_context.UnmakeCurrent();
                ::EndPaint(m_handle, &ps);
                result = 0;
                return true;
            }
            else if(msg == WM_ERASEBKGND)
            {
                result = 1;
                return true;
            }
            else if(msg == WM_SIZE)
            {
                OnSize(LOWORD(lparam), HIWORD(lparam));
                result = 0;
                return true;
            }
            else if(msg == WM_CLOSE)
            {
                bool close = true;
                OnClose(close);

                if(close)
                    Destroy();

                result = 0;
                return true;
            }
        }

        OnIdle();
        return false;
    }

    static LRESULT CALLBACK StaticProcessEvent(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        if(msg == WM_CREATE)
        {
            LONG_PTR win_ptr = LONG_PTR(LPCREATESTRUCT(lparam)->lpCreateParams);

            if(win_ptr)
                SetWindowLongPtr(handle, GWLP_USERDATA, win_ptr);
            else
                throw ExceptionInfo << "NULL Win32Window pointer in CreateWin32Window function.";

            return 0;
        }
        else if(msg == WM_DESTROY)
        {
            PostQuitMessage(0);
            return 0;
        }
        else if(msg == WM_NCCREATE)
        {
            return ::DefWindowProc(handle, msg, wparam, lparam);
        }

        Win32Window* window = (Win32Window*)::GetWindowLongPtr(handle, GWLP_USERDATA);
        LRESULT result = 0;

        if(window)
        {
            if(window->ProcessEvent(handle, msg, wparam, lparam, result))
                return result;
        }

        //Do not call DefWindowProc from Win32Window::ProcessEvent,  this will cause an OS error.
        return ::DefWindowProc(handle, msg, wparam, lparam);
    }

    static int ProcessEvents()
    {
        MSG msg;
        BOOL ret = FALSE;

        while((ret = GetMessage( &msg, NULL, 0, 0 )) != 0)
        {
            if(ret == -1)
                throw ExceptionInfo << "Win32 event loop error.";
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        return msg.wParam;
    }

    virtual void Create(const char* title, int x, int y, int width, int height)
    {
        if(m_handle)
            throw ExceptionInfo << "Wind32 window handle is not NULL. Maybe window created ?.";

        std::wstring classname = L"vgu::iWin32Window";
        classname += std::to_wstring(reinterpret_cast<int64_t>(this));
        //std::wcout << classname << std::endl;

        HINSTANCE hinstance = (HINSTANCE)::GetModuleHandle(NULL);

        WNDCLASSEX wndclassex = {0};
        wndclassex.cbSize = sizeof(WNDCLASSEX);
        wndclassex.lpfnWndProc = Win32Window::StaticProcessEvent;
        wndclassex.style = CS_DBLCLKS |CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
        wndclassex.hInstance = hinstance;
        wndclassex.lpszClassName = classname.c_str();
        wndclassex.hCursor = LoadCursor(NULL, IDC_ARROW);

        if(!::RegisterClassEx(&wndclassex))
            throw ExceptionInfo << "Can't register Win32 window class.";

        //::DwmEnableComposition(DWM_EC_DISABLECOMPOSITION);

        std::wstring wtitle = vgui::Utf8ToWstring(title);
        m_handle = ::CreateWindowEx(0, classname.c_str(), wtitle.c_str(), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                                    x, y, width, height, NULL, NULL, hinstance, this);

        if(!m_handle)
            throw ExceptionInfo << "Can't create Win32 window.";

        HDC hdc = ::GetDC(m_handle);
        m_context.Create(hdc);
        m_context.MakeCurrent(hdc);
        m_context.UnmakeCurrent();

        ::ShowWindow(m_handle, SW_SHOW);
        ::UpdateWindow(m_handle);

        AddToEventLoop(CastThis());
    }

    virtual void Destroy()
    {
        if(IsValid())
        {
            RemoveFromEventLoop(CastThis());

            m_context.Destroy();

            ::DestroyWindow(m_handle);
            m_handle = NULL;
        }
    }

private:

    static void AddToEventLoop(WindowT* window)
    {
        auto i = std::find(s_windows.begin(), s_windows.end(), window);

        if( i== s_windows.end())
            s_windows.push_back(window);
    }

    static void RemoveFromEventLoop(WindowT* window)
    {
        auto i = std::find(s_windows.begin(), s_windows.end(), window);

        if( i!= s_windows.end())
            s_windows.erase(i);
    }

    HWND m_handle{NULL};
    vgui::GLContext m_context;
    static inline std::vector<WindowT*> s_windows;
};


}//namespace vgui

