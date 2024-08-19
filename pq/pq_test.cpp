
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "pq.hpp"

void TestPQ(void);

int main(void)
{
    TestPQ();

    return 0;
}

void TestPQ(void)
{
    ilrd::PriorityQueue<int> pq;

    std::cout << "TestPQ:" << std::endl;

    (pq.empty()) ? 
    std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";
    srand(time(NULL));

    bool status = true;
    for (std::size_t i = 0; (i < 10000) && status; ++i)
    {
        int rand_num = rand() % 10000;
        pq.push(rand_num);
        status = (rand_num <= pq.Front());
    }

    std::cout << "TestPQ push: ";
    (!pq.empty()) ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (status) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";

    for (std::size_t i = 0; (i < 10000) && status; ++i)
    {
        int temp = pq.Front();
        pq.pop();
        status = (temp >= pq.Front());
    }

    std::cout << "TestPQ pop: ";
    status ? std::cout << "SUCCESS ✅," : std::cout << "FAIL ❌,";
    (pq.empty()) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";
}