/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 16/5/2023	               */
/* WORKSHEET NAME: bit array      	      */
/* REVIEWER: Noy		                 */
/*--------------------------------------*/

#include <iostream>

#include "bit_array.hpp"

void Test1(void);
void Test2(void);

int main(void)
{
    Test1();
    Test2();

    return 0;
}

void Test1(void)
{
    ilrd::BitArray<120> b1;
    ilrd::BitArray<120> b2;
    std::size_t i = 100;

    if (b1[i])
    {
        std::cout << "b1[i] is true" << std::endl;
    }

    std::cout << "Test1:    ";

    b1[i] = true;
    (b1[i]) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,"; 
    b1[i] = b2[i];

    (!b1[i]) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,"; 
    (!b2[i]) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n"; 
}

void Test2(void)
{
    ilrd::BitArray<120> b1;
    ilrd::BitArray<120> b2;

    b1[100] = true;
    b2[100] = true;

    std::cout << "\n== test:    ";
    (b1 == b2) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    b1[1] = true;
    !(b1 == b2) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";

    std::cout << "\n!= test:    ";
    (b1 != b2) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    b2[1] = true;
    !(b1 != b2) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";

    std::cout << "\n|= test:    ";
    b1[1] = false;
    b2[1] = true;
    b1 |= b2;
    (b1[100]) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (!b1[10]) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (b1[1]) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";

    std::cout << "\n&= test:    ";
    b1[3] = true;
    b1[3] = false;
    b1 &= b2;
    (b1[1]) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (!b1[0]) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (!b1[3]) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";

    std::cout << "\n^= test:    ";
    b1[1] = true;
    b2[1] = true;
    b1[101] = true;
    b2[101] = false;
    b1[10] = false;
    b2[10] = true;
    b1[64] = false;
    b2[64] = false;
    b1 ^= b2;

    (!b1[1]) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (b1[101]) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (b1[10]) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (!b1[64]) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";

    std::cout << "\ncount test:    ";
    ilrd::BitArray<112> b3;
    (0 == b3.Count()) ? 
    std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    b3[0] = true;
    b3[1] = true;
    b3[23] = true;
    b3[100] = true;
    b3[64] = true;
    b3[111] = true;
    (2 == b1.Count()) ? 
    std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (6 == b3.Count()) ? 
    std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (3 == b2.Count()) ?
    std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";

    std::cout << "\n ToString test:    \n";
    std::cout << b1.ToString() << std::endl;
    std::cout << b2.ToString() << std::endl;
    std::cout << b3.ToString() << std::endl;

    std::cout << "\nSetAll test:    ";
    b2.Set();
    (120 == b2.Count()) ? 
    std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";
    std::cout << b2.ToString() << std::endl;

    std::cout << "\nSet test:    ";
    b2.Set(101, false);
    b2.Set(10, false);
    b2.Set(63, false);
    (117 == b2.Count()) ?
    std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    b2.Set(10, true);
    (118 == b2.Count()) ?
    std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";

    std::cout << "\nGet test:    ";
    (b2.Get(10)) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (!b2.Get(63)) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (!b2.Get(101)) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (b2.Get(119)) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (b2.Get(100)) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";

    std::cout << "\nFlip all test:    ";
    b2.Flip();
    (2 == b2.Count()) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    b3.Flip();
    (106 == b3.Count()) ? 
    std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";
    std::cout << b2.ToString() << std::endl;
    std::cout << b3.ToString() << std::endl;

    std::cout << "\nFlip test:    ";
    b2[10] = false;
    b2.Flip(10);
    (3 == b2.Count()) ?
    std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (b2[10]) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";

    std::cout << "\nshift right test:    ";
    ilrd::BitArray<121> b4;
    b4[0] = true;
    b4[63] = true;
    b4[64] = true;
    b4[120] = true;
    std::cout << '\n' << b4.ToString() << std::endl;
    b4 >>= 64;
    std::cout << '\n' << b4.ToString() << std::endl;
    (b4[56]) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";

    ilrd::BitArray<400> b5;
    ilrd::BitArray<400> b6;
    b5[0] = true;
    b6[300] = true;

    b6 >>= 300;
    (b5 == b6) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";

    std::cout << "\nshift left test:    ";
    b4 <<= 120;
    (b4[120]) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";
    std::cout << '\n' << b4.ToString() << std::endl;
    b6 <<= 300;
    b6[360] = true;
    b5[60] = true;
    b5 <<= 300;
    (b5 == b6) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";

    bool status = 1;
    ilrd::BitArray<1000> b7;
    b7[0] = true;

    for (std::size_t i = 0; (i < 1000) && status; ++i)
    {
        status = (b7[i]);
        b7 <<= 1;
    }

    std::cout << "\ncheck shift left loop test:    ";
    (0 == b7.Count()) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (status) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";

    b7[999] = true;
    for (std::size_t i = 0; (i < 1000) && status; ++i)
    {
        status = (b7[1000 - i - 1]);
        b7 >>= 1;
    }

    std::cout << "\ncheck shift right loop test:    ";
    (0 == b7.Count()) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (status) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";

    b7[0] = true;

    for (std::size_t i = 0; (i < 1000) && status; i+= 65)
    {
        status = (b7[i]);
        b7 <<= 65;
    }

    std::cout << "\ncheck shift left loop test:    ";
    (0 == b7.Count()) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (status) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";

    b7[999] = true;
    for (std::size_t i = 0; (i < 1000) && status; i+= 65)
    {
        status = (b7[1000 - i - 1]);
        b7 >>= 65;
    }

    std::cout << "\ncheck shift right loop test:    ";
    (0 == b7.Count()) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (status) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";

    b7[0] = true;

    for (std::size_t i = 0; (i < 1000) && status; i+= 122)
    {
        status = (b7[i]);
        b7 <<= 122;
    }

    std::cout << "\ncheck shift left loop test:    ";
    (0 == b7.Count()) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (status) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";

    b7[999] = true;
    for (std::size_t i = 0; (i < 1000) && status; i+= 122)
    {
        status = (b7[1000 - i - 1]);
        b7 >>= 122;
    }
    
    std::cout << "\ncheck shift right loop test:    ";
    (0 == b7.Count()) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (status) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";

    b7[0] = true;

    for (std::size_t i = 0; (i < 1000) && status; i+= 256)
    {
        status = (b7[i]);
        b7 <<= 256;
    }

    std::cout << "\ncheck shift left loop test:    ";
    (0 == b7.Count()) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (status) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";

    b7[999] = true;
    for (std::size_t i = 0; (i < 1000) && status; i+= 256)
    {
        status = (b7[1000 - i - 1]);
        b7 >>= 256;
    }
    
    std::cout << "\ncheck shift right loop test:    ";
    (0 == b7.Count()) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (status) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";


}
