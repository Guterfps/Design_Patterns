
#include <iostream>
#include <future>
#include <vector>
#include <thread>

#include "semaphore.hpp"

void TestSemaphore(void);

int main(void)
{
    TestSemaphore();

    return 0;
}

void TestSemaphore(void)
{
    ilrd::Semaphore sem(1);
    int resorce = 0;
    std::vector<std::future<void>> futures;

    for (std::size_t i = 0; i < 10; ++i)
    {
        futures.emplace_back(std::async(std::launch::async,
         [&resorce, &sem, i]()
         {
            sem.Wait();
            resorce += i;
            //std::this_thread::sleep_for(std::chrono::milliseconds(100));

            sem.Post();
         }));
    }

    for (auto& future : futures)
    {
        future.wait();
    }

    std::cout << "resorce: " << resorce;
    (45 == resorce) ? std::cout << " SUCCESS ✅\n" : std::cout << " FAIL ❌\n";

    std::vector<std::future<void>> futures2;
    resorce = 0;

    for (std::size_t i = 0; i < 5; ++i)
    {
        futures2.emplace_back(std::async(std::launch::async,
        [&resorce, &sem, i]()
        {
            if (sem.WaitFor(std::chrono::milliseconds(250)))
            {
                ++resorce;
                std::this_thread::sleep_for(std::chrono::
                                            milliseconds(100 * resorce));
                std::cout << "thread " << i << " is done\n";
                sem.Post();
            }
            else
            {
                std::cout << "thread " << i << " timed out\n";
            }
        }));
    }
}