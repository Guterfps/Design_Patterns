/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 4/6/2023	               */
/* WORKSHEET NAME: Singleton              */
/* REVIEWER: Eitan 		                 */
/*--------------------------------------*/

#include <iostream>
#include <string>
#include <vector>
#include <future>

#include "shapes.hpp"
#include "simple_singleton.hpp"
#include "singleton.hpp"
#include "factory_singleton.hpp"

class Creator
{
public:
    virtual ilrd::Shape *operator()(const ilrd::Point& p) =0;
    virtual ~Creator() {};
};

class CircleCreator : public Creator
{
public:
    CircleCreator(int radius) : m_radius(radius) {}
    ilrd::Shape *operator()(const ilrd::Point& p)
    {
        return new ilrd::Circle(p, m_radius);
    }

private:
    double m_radius;
};

class SquareCreator : public Creator
{
public:
    SquareCreator(double width) : m_width(width) {}
    ilrd::Shape *operator()(const ilrd::Point& p)
    {
        return new ilrd::Square(p, m_width);
    }

private:
    double m_width;
};

class RectangleCreator : public Creator
{
public:
    RectangleCreator(double width, double height) : 
    m_width(width), m_height(height) {}
    ilrd::Shape *operator()(const ilrd::Point& p)
    {
        return new ilrd::Rectangle(p, m_width, m_height);
    }

private:
    double m_width;
    double m_height;
};

class LineCreator : public Creator
{
public:
    LineCreator(const ilrd::Point& start) : m_start(start) {}
    ilrd::Shape *operator()(const ilrd::Point& p)
    {
        return new ilrd::Line(m_start, p);
    }

private:
    ilrd::Point m_start;
};

void TestSimpleSingleton(void);
void TestSingleton(void);
void TestMultiThreadSingleton(void);

int main()
{
    TestSimpleSingleton();
    TestSingleton();
    TestMultiThreadSingleton();

    return 0;
}

void TestSimpleSingleton(void)
{
    ilrd::FactorySingleton<ilrd::Shape, std::string, ilrd::Point> *factory = 
    ilrd::FactorySingleton<ilrd::Shape, std::string, ilrd::Point>::GetInstance();
    std::vector<ilrd::Shape *> shapes;
    CircleCreator circleCreator(10);
    SquareCreator squareCreator(5);
    RectangleCreator rectangleCreator(1, 15);
    LineCreator lineCreator(ilrd::Point(1, 2));

    std::cout << "Factory Simple Singleton:" << std::endl;

    factory->Add("circle", circleCreator);
    factory->Add("square", squareCreator);
    factory->Add("rectangle", rectangleCreator);
    factory->Add("Line", lineCreator);

    shapes.push_back((factory->Create("circle", ilrd::Point(0, 0))));
    shapes.push_back((factory->Create("square", ilrd::Point(1, 2))));
    shapes.push_back((factory->Create("rectangle", ilrd::Point(3, 4))));
    shapes.push_back((factory->Create("Line", ilrd::Point(5, 6))));

    for (auto shape : shapes)
    {
        shape->Draw();
        delete shape;
    }

}

void TestSingleton(void)
{
    auto *factory = ilrd::Singleton<ilrd::Factory<ilrd::Shape, std::string, 
                                    ilrd::Point>>::GetInstance();
    std::vector<ilrd::Shape *> shapes;
    CircleCreator circleCreator(10);
    SquareCreator squareCreator(5);
    RectangleCreator rectangleCreator(1, 15);
    LineCreator lineCreator(ilrd::Point(1, 2));

    std::cout << "\nSingleton:" << std::endl;

    factory->Add("circle", circleCreator);
    factory->Add("square", squareCreator);
    factory->Add("rectangle", rectangleCreator);
    factory->Add("Line", lineCreator);

    shapes.push_back((factory->Create("circle", ilrd::Point(0, 0))));
    shapes.push_back((factory->Create("square", ilrd::Point(1, 2))));
    shapes.push_back((factory->Create("rectangle", ilrd::Point(3, 4))));
    shapes.push_back((factory->Create("Line", ilrd::Point(5, 6))));

    for (auto shape : shapes)
    {
        shape->Draw();
        delete shape;
    }
}

void TestMultiThreadSingleton(void)
{
    std::vector<std::future<ilrd::Factory<ilrd::Shape, std::string, 
                                    ilrd::Point> *>> futures;
    std::vector<ilrd::Factory<ilrd::Shape, std::string, 
                                    ilrd::Point> *> factories;

    std::cout << "\nMulti Thread Singleton:" << std::endl;

    for (std::size_t i = 0; i < 32; ++i)
    {
        futures.push_back(std::async(
        []()
        {
            return ilrd::Singleton<ilrd::Factory<ilrd::Shape, std::string,
                                    ilrd::Point>>::GetInstance();
        }));
    }

    for (auto &future : futures)
    {
        factories.push_back(future.get());
    }

    bool status = true;
    for (std::size_t i = 0; (i < factories.size() - 1) && status; ++i)
    {
        status = (factories[i] == factories[i + 1]);
    }

    status ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";
}