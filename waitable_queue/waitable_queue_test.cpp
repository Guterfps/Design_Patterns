/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 22/5/2023	               */
/* WORKSHEET NAME: waitable queueu        */
/* REVIEWER: Gal		                 */
/*--------------------------------------*/

#include <iostream>
#include <future>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>

#include "pq.hpp" // priority queue
#include "waitable_queue.hpp"

void TestWaitableQueue(void);

int main(void)
{
    TestWaitableQueue();

    return 0;
}

void TestWaitableQueue(void)
{
    std::vector<std::future<void>> futuresPush;
    std::vector<std::future<void>> futuresPop;
    ilrd::WaitableQueue<ilrd::PriorityQueue<int>> queue;
    std::vector<int> resualts;
    std::mutex mtx;

    srand(time(NULL));

    std::cout << "TestWaitableQueue:" << std::endl;

    std::cout << "IsEmpty: ";
    (queue.IsEmpty()) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";

    for (std::size_t i = 0; i < 1000; ++i)
    {
        futuresPop.emplace_back(std::async(std::launch::async,
        [&queue, &resualts, &mtx]
        {
            int temp = 0;
            if (queue.Pop(temp))
            {
                std::lock_guard<std::mutex> guard(mtx);
                resualts.push_back(temp);
            }
        }));
        futuresPush.emplace_back(std::async(std::launch::async,
        [&queue]
        {
            queue.Push(rand() % 1000);
        }));
    }

    for (auto &f : futuresPush)
    {
        f.wait();
    }
    for (auto &f : futuresPop)
    {
        f.wait();
    }

    std::cout << "IsEmpty: ";
    (queue.IsEmpty()) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";
    std::cout << "Size: " << resualts.size();
    (1000 ==resualts.size()) ? 
    std::cout << " SUCCESS ✅\n" : std::cout << " FAIL ❌\n";

    std::cout << "timeout test:\n";

    futuresPush.emplace_back(std::async(std::launch::async,
    [&queue]
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        queue.Push(rand() % 1000);
    }));

    futuresPop.emplace_back(std::async(std::launch::async,
    [&queue]
    {
        int temp = 0;
        if (queue.Pop(temp, std::chrono::milliseconds(2000)))
        {
            std::cout << "patient Pop: SUCCESS ✅\n";
        }
        else
        {
            std::cout << "patient Pop: FAIL ❌\n";
        }

    }));

    futuresPop.push_back(std::async(std::launch::async,
    [&queue]
    {
        int temp = 0;
        if (queue.Pop(temp, std::chrono::milliseconds(500)))
        {
            std::cout << "not patient Pop: FAIL ❌\n";
        }
        else
        {
            std::cout << "not patient Pop: SUCCESS ✅\n";
        }

    }));

    for (auto &f : futuresPush)
    {
        f.wait();
    }
    for (auto &f : futuresPop)
    {
        f.wait();
    }
}