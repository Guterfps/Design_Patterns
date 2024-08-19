/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 10/4/2023	               */
/* WORKSHEET NAME: Shapes                 */
/* REVIEWER: Moriah		                 */
/*--------------------------------------*/

#include <iostream> // cout
#include <algorithm> // for_each

#include "shapes.hpp"

namespace ilrd
{
// Point class
Point::Point(double x, double y) : m_x(x), m_y(y)
{
}

double Point::GetX() const
{
    return m_x;
}

double Point::GetY() const
{
    return m_y;
}

void Point::SetX(double newPosition)
{
    m_x = newPosition;
}

void Point::SetY(double newPosition)
{
    m_y = newPosition;
}

// Line class
Line::Line(Point start, Point end) : m_start(start), m_end(end)
{
}

void Line::Draw() const
{
    std::cout << "Line start at: (" << m_start.GetX() << " , " 
              << m_start.GetY() << ") " << " and ends at: ("
              << m_end.GetX() << " , " << m_end.GetY() << ")" << '\n';
}

void Line::Move(Point newPoint)
{
    double deltaX = newPoint.GetX() - m_start.GetX();
    double deltaY = newPoint.GetY() - m_start.GetY();
    m_start = newPoint;
    m_end.SetX(m_end.GetX() + deltaX);
    m_end.SetY(m_end.GetY() + deltaY);
}

bool Line::operator==(const Shape& sh) const
{
    bool status = false; 
    const Line *psh = dynamic_cast<const Line *>(&sh);
    
    if (0 != psh)
    {
        status = (m_start.GetX() == psh->m_start.GetX()) && 
                 (m_start.GetY() == psh->m_start.GetY()) &&
                 (m_end.GetX() == psh->m_end.GetX()) && 
                 (m_end.GetY() == psh->m_end.GetY());
    }
    
    return status;
}

// Rectangle class
Rectangle::Rectangle(Point left_up, double length, double width) : 
m_left_up(left_up), m_length(length), m_width(width)
{
}

void Rectangle::Draw() const
{
    std::cout << "Rectangle top left corner is: (" << m_left_up.GetX() 
              << " , " << m_left_up.GetY() << ")" 
              << ", width: " << m_width << ", length: " << m_length << '\n';
}

void Rectangle::Move(Point newPoint)
{
    m_left_up = newPoint;
}

bool Rectangle::operator==(const Shape& sh) const
{
    bool status = false; 
    const Rectangle *psh = dynamic_cast<const Rectangle *>(&sh);
    
    if (0 != psh)
    {
        status = (m_left_up.GetX() == psh->m_left_up.GetX()) && 
                 (m_left_up.GetY() == psh->m_left_up.GetY()) &&
                 (m_length == psh->m_length) && 
                 (m_width == psh->m_width);
    }
    
    return status;
}

// Square class
Square::Square(Point left_up, double width) : m_left_up(left_up), m_width(width)
{
}

void Square::Draw() const
{
    std::cout << "Square top left corner is: (" << m_left_up.GetX() 
              << " , " << m_left_up.GetY() << ")" 
              << ", width: " << m_width << '\n';
}

void Square::Move(Point newPoint)
{
    m_left_up = newPoint;
}

bool Square::operator==(const Shape& sh) const
{
    bool status = false; 
    const Square *psh = dynamic_cast<const Square *>(&sh);
    
    if (0 != psh)
    {
        status = (m_left_up.GetX() == psh->m_left_up.GetX()) && 
                 (m_left_up.GetY() == psh->m_left_up.GetY()) && 
                 (m_width == psh->m_width);
    }
    
    return status;
}

// Circle class
Circle::Circle(Point centerPoint, double radius) : 
m_center(centerPoint), m_radius(radius)
{
}

void Circle::Draw() const
{
    std::cout << "Circle center is: (" << m_center.GetX() 
              << " , " << m_center.GetY() << ")" 
              << ", radius: " << m_radius << '\n';
}

void Circle::Move(Point newPoint)
{
    m_center = newPoint;
}

bool Circle::operator==(const Shape& sh) const
{
    bool status = false; 
    const Circle *psh = dynamic_cast<const Circle *>(&sh);
    
    if (0 != psh)
    {
        status = (m_center.GetX() == psh->m_center.GetX()) && 
                 (m_center.GetY() == psh->m_center.GetY()) && 
                 (m_radius == psh->m_radius);
    }
    
    return status;
}

// ShapeComposite class
ShapeComposite::ShapeComposite(Shape *sh) : m_shapes(1, sh)
{
}

void ShapeComposite::AddShape(Shape *sh)
{
    m_shapes.push_back(sh);
}

void ShapeComposite::RemoveShape(Shape *sh)
{
    bool status = false;
    
    if (1 < m_shapes.size())
    {
        std::size_t i = 0;
        for (;(i < m_shapes.size()) && (!status); ++i)
        {
            status = *(m_shapes[i]) == *sh;
        }
        
        if (status)
        {
            m_shapes.erase(m_shapes.begin() + i - 1);
        }
    }
}

bool ShapeComposite::operator==(const Shape& sh) const
{
    bool status = false; 
    const ShapeComposite *psh = dynamic_cast<const ShapeComposite *>(&sh);
    
    if ((0 != psh) && (m_shapes.size() == psh->m_shapes.size()))
    {
        status = true;
        for (std::size_t i = 0; (i < m_shapes.size()) && (status); ++i)
        {
            status = *(m_shapes[i]) == *(psh->m_shapes[i]);
        }
    }
    
    return status;
}

void ShapeComposite::Draw() const
{
    std::for_each(m_shapes.begin(), m_shapes.end(), std::mem_fun(&Shape::Draw));
}

void ShapeComposite::Move(Point newPoint)
{
    std::for_each(m_shapes.begin(), m_shapes.end(),
              std::bind2nd(std::mem_fun(&Shape::Move), newPoint));
}

} // ilrd

