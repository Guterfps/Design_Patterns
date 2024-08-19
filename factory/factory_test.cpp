/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 1/6/2023	               */
/* WORKSHEET NAME: Factory                */
/* REVIEWER: Or 		                 */
/*--------------------------------------*/

#include <iostream>
#include <string>
#include <vector>

#include "shapes.hpp"
#include "factory.hpp"

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

void TestFactory(void);

int main(void)
{
    TestFactory();

    return 0;
}

void TestFactory(void)
{
    ilrd::Factory<ilrd::Shape, std::string, ilrd::Point> factory;
    std::vector<ilrd::Shape *> shapes;
    CircleCreator circleCreator(10);
    SquareCreator squareCreator(5);
    RectangleCreator rectangleCreator(1, 15);
    LineCreator lineCreator(ilrd::Point(1, 2));

    factory.Add("circle", circleCreator);
    factory.Add("square", squareCreator);
    factory.Add("rectangle", rectangleCreator);
    factory.Add("Line", lineCreator);
    //factory.Add("composit", CreateShape<ilrd::ShapeComposite>);

    shapes.push_back((factory.Create("circle", ilrd::Point(0, 0))));
    shapes.push_back((factory.Create("square", ilrd::Point(1, 2))));
    shapes.push_back((factory.Create("rectangle", ilrd::Point(3, 4))));
    shapes.push_back((factory.Create("Line", ilrd::Point(5, 6))));

    for (auto shape : shapes)
    {
        shape->Draw();
        delete shape;
    }

}