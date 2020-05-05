#include <vgui/vgui.hpp>

class A
{
public:
    A(){}

protected:

    virtual ~A(){}
};

class B : public A
{
public:
    B(){}

protected:

    ~B(){}
};

class C : public B
{
public:
    C(){}

protected:

    ~C(){}
};

int main()
{    
    try
    {
        //C cs;
        //auto c = std::make_shared<C>();

        vgui::PointI int_point1 {1, 1};
        vgui::PointI int_point2 {10, 20};
        vgui::PointF float_point1 {4, 0.5};

        //int_point1 = {0,0};
        int_point1 *= 0.5f;
        int_point2 = 0.5 * int_point2;
        //float_point1 *= 1.0;
        float_point1 = 1 * float_point1;
        int_point1 = int_point1 + int_point2;

        std::cout << int_point1 << std::endl;
        std::cout << int_point2 << std::endl;
        std::cout << float_point1 << std::endl;

        float f=0.7f;
        int i=static_cast<int>(f);
        std::cout << i << std::endl;
    }
    catch(const vgui::Exception& e)
    {
        e.Print();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "Error" << std::endl;
    }

    return 777;
}
