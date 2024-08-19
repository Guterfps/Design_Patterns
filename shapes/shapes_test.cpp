/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 10/4/2023	               */
/* WORKSHEET NAME: Shapes                 */
/* REVIEWER: Moriah		                 */
/*--------------------------------------*/

#include <iostream>

#include "shapes.hpp"

int main(void)
{
    ilrd::Point p1(1,2);
    ilrd::Point p2(2,1);
    ilrd::Line l1(p1, p2);
    ilrd::Line l2(p2, p1);
    ilrd::Rectangle rec(p1, 2, 3.1);
    ilrd::Rectangle rec2(p2, 2.1, 3);
    ilrd::Square sqr(p2, 5);
    ilrd::Square sqr2(p1, 3.14);
    ilrd::Circle circl(p1, 3.14);
    ilrd::Circle circl2(p2, 3.1415);
    
    l1.Draw();
    rec.Draw();
    sqr.Draw();
    circl.Draw();
    
    std::cout << "test line '==':  ";
    (l1 == l1) ? std::cout << "SUCCESS ✅️," : std::cout << "FAIL ❌️,";
    !(l1 == l2) ? std::cout << "SUCCESS ✅️\n" : std::cout << "FAIL ❌️\n";
    
    std::cout << "test rec '==':  ";
    (rec == rec) ? std::cout << "SUCCESS ✅️," : std::cout << "FAIL ❌️,";
    !(rec == rec2) ? std::cout << "SUCCESS ✅️\n" : std::cout << "FAIL ❌️\n";
    
    std::cout << "test sqr '==':  ";
    (sqr == sqr) ? std::cout << "SUCCESS ✅️," : std::cout << "FAIL ❌️,";
    !(sqr2 == sqr) ? std::cout << "SUCCESS ✅️\n" : std::cout << "FAIL ❌️\n";
    
    std::cout << "test circl '==':  ";
    (circl == circl) ? std::cout << "SUCCESS ✅️," : std::cout << "FAIL ❌️,";
    !(circl == circl2) ? std::cout << "SUCCESS ✅️\n" : std::cout << "FAIL ❌️\n";
    
    std::cout << "test different types '==':  ";
    !(circl == sqr2) ? std::cout << "SUCCESS ✅️," : std::cout << "FAIL ❌️,";
    !(rec == sqr) ? std::cout << "SUCCESS ✅️\n" : std::cout << "FAIL ❌️\n";
    
    std::cout << std::endl << "test composite:  \n";
    ilrd::ShapeComposite comp2(&l2);
    ilrd::ShapeComposite comp1(&comp2);
    ilrd::ShapeComposite comp3(&l1);
    ilrd::ShapeComposite comp4(&comp2);
    //comp1.AddShape(&comp2);
    comp1.AddShape(&circl);
    comp1.AddShape(&l2);
    comp1.AddShape(&sqr);
    comp1.AddShape(&rec);
    comp2.AddShape(&circl2);
    comp2.AddShape(&rec2);
    comp1.Draw();
    
    std::cout << "\nadd comp to comp: \n";
    comp1.AddShape(&comp3);
    comp1.Draw();
    
    ilrd::Point p3(123, 0.123);
    comp1.Move(p3);
    std::cout << std::endl << "test composite after move:  \n";
    comp1.Draw();
    
    std::cout << "test comps '==':  ";
    (comp1 == comp1) ? std::cout << "SUCCESS ✅️," : std::cout << "FAIL ❌️,";
    !(comp1 == comp2) ? std::cout << "SUCCESS ✅️\n" : std::cout << "FAIL ❌️\n";
    
    //comp4.AddShape(&comp2);
    comp4.AddShape(&circl);
    comp4.AddShape(&l2);
    comp4.AddShape(&sqr);
    comp4.AddShape(&rec);
    comp4.AddShape(&comp3);
    
    std::cout << "test comps '==':  ";
    (comp1 == comp4) ? std::cout << "SUCCESS ✅️\n" : std::cout << "FAIL ❌️\n";
    
    std::cout << std::endl << "\ntest composite after remove:  \n";
    comp1.RemoveShape(&l2);
    comp1.RemoveShape(&comp2);
    comp1.RemoveShape(&l1);
    comp1.Draw();
    
    std::cout << "test comps '==':  ";
    !(comp1 == comp4) ? std::cout << "SUCCESS ✅️\n" : std::cout << "FAIL ❌️\n";
    
    return 0;
}
