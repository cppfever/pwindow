#include <vgui/vgui.hpp>

#include <windows.h>


int main(int argc, char **argv)
//int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try {
      vgui::Window window("PWindow demo", 400, 400, 640, 480);

      return window.Run();
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

int agg_main(int, char**){return 0;}
