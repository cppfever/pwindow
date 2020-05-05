#include <vgui/vgui.hpp>

#include <windows.h>


//int main(int argc, char **argv)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try {
      vgui::Window window("ECS demo", 400, 400, 640, 480);

      return window.Run();
      // All SDL objects are released at this point or if an error occurs
    }
    catch (vgui::Exception& e)
    {
      e.Print();
      return 111;
    }
    catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
      return 222;
    }
}
