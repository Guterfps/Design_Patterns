/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 10/4/2023	               */
/* WORKSHEET NAME: Shapes                 */
/* REVIEWER: Moriah		                 */
/*--------------------------------------*/

#ifndef ILRD_RD136_7_SHAPES_HPP
#define ILRD_RD136_7_SHAPES_HPP

#include <vector> // vector

namespace ilrd
{
class Point
{
public:
	explicit Point(double x = 0, double y = 0);
	
	double GetX() const;
	double GetY() const;
	void SetX(double newPosition);
	void SetY(double newPosition);
	
private:
	double m_x;
	double m_y;
};

class Shape
{
public:
	virtual void Draw()const =0;
	virtual void Move(Point newPoint)=0;
	virtual bool operator==(const Shape& sh)const =0;
	virtual ~Shape() {};
};

class Line: public Shape
{
public:
	explicit Line(Point start, Point end);
	void Draw() const;
	void Move(Point newPoint);
	bool operator==(const Shape& sh) const;
	
private:
    Point m_start;
    Point m_end;
};

class Rectangle: public Shape
{
public:
	explicit Rectangle(Point left_up, double length, double width);
	void Draw() const;
	void Move(Point newPoint);
	bool operator==(const Shape& sh) const;
	
private:
    Point m_left_up;
    double m_length;
    double m_width;
};

class Square: public Shape
{
public:
	explicit Square(Point left_up, double width);
	void Draw() const;
	void Move(Point newPoint);
	bool operator==(const Shape& sh) const;
	
private:
    Point m_left_up;
    double m_width;
};

class Circle: public Shape
{
public:
	explicit Circle(Point centerPoint, double radius);
	void Draw() const;
	void Move(Point newPoint);
	bool operator==(const Shape& sh) const;
	
private:
    Point m_center;
    double m_radius;
};

class ShapeComposite: public Shape
{
public:
	explicit ShapeComposite(Shape *sh);
	void AddShape(Shape *sh);
	void RemoveShape(Shape *sh);
	bool operator==(const Shape& sh) const;
	
	void Draw() const;
	void Move(Point newPoint);
	
private:
    std::vector<Shape *> m_shapes;
};
} // ilrd

#endif	// ILRD_RD136_7_SHAPES_HPP

