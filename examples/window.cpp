#include <vgui/MainWindow.hpp>
#include <vgui/Application.hpp>

using namespace vgui;

int main()
{    
    try
    {
        Application app;
        vgui::Window window(300,200,"Window");

        return app.Run();
    }
    catch(const vgui::Exception& e)
    {
        e.Print();
    }
    catch(const std::exception& e)
    {
        std::cout << "Some std::exception was caught." << std::endl;
        std::cout << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "Unknown Error in vgui application !" << std::endl;
    }

    return 0;
}
