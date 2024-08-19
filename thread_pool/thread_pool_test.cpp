/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 30/5/2023	               */
/* WORKSHEET NAME: Thread Pool            */
/* REVIEWER: Or 		                 */
/*--------------------------------------*/

#include <iostream>
#include <vector>
#include <atomic>
#include <chrono>
#include <future>

#include "thread_pool.hpp"

void TestThreadPool(void);
void BenchMarkThreadPool(unsigned int n);
void BenchMark(void);
void TestBenchMark(void);

int main(void)
{
    for (int i = 0; i < 100; ++i)
    TestThreadPool();
    TestBenchMark();
    
    return 0;
}

void TestThreadPool(void)
{
    ilrd::ThreadPool pool(8);
    std::vector<ilrd::ThreadPool::TaskFuture> futures;
    std::atomic<std::size_t> taskCounter(0);

    std::cout << "TestThreadPool: " << std::endl;

    std::cout << "test puse: \n";
    pool.Pause();

    for (std::size_t i = 0; i < 100; ++i)
    {
        futures.push_back(pool.AddTask(std::make_shared<std::function<int()>>(
        [i, &taskCounter]
        { 
            std::cout << "Hello from task " << i  << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            ++taskCounter;
            //throw std::runtime_error("test");
            return 1;
        }), static_cast<ilrd::ThreadPool::Priority>(i % 3)));
    }

    /* std::cout << "press enter to resume: ";
    std::cin.get(); */
    pool.Resume();

    pool.SetNThreads(16);
    pool.SetNThreads(12);
    
    pool.Pause();
    for (auto& future : futures)
    {
        std::cout << "status: ";
        switch (future.GetStatus())
        {
            case ilrd::ThreadPool::WAIT:
                std::cout << "WAIT" << '\n';
                break;
            case ilrd::ThreadPool::RUN:
                std::cout << "RUN" << '\n';
                break;
            case ilrd::ThreadPool::DONE:
                std::cout << "DONE" << '\n';
                break;
            default:
                std::cout << "UNKNOWN" << '\n';
                break;
        }
    }
    pool.Resume();
    
    pool.SetNThreads(4);
    pool.SetNThreads(16);

    std::size_t counter = 0;
    
    pool.ShutDown();
    pool.WaitTermination();
    for (auto& future : futures)
    {
        try
        {
        counter += ((0 == future.WaitResult()) ?  1 : 0);
        }
        catch (std::exception& e)
        {
            std::cout << "test: " << e.what() << '\n';
        }
    }

    std::cout << "test: ";
    (100 == taskCounter) ? 
    std::cout << "SUCCESS ✅, " : std::cout << "FAIL ❌, ";
    (100 == counter) ? std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";

}

const std::size_t NumberOftasks = 30000;

void BenchMarkThreadPool(unsigned int n)
{
    ilrd::ThreadPool pool(n);
    std::vector<ilrd::ThreadPool::TaskFuture> futures;
    std::atomic<std::size_t> taskCounter(0);

    for (std::size_t i = 0; i < NumberOftasks; ++i)
    {
        futures.push_back(pool.AddTask(std::make_shared<std::function<int()>>(
        [&taskCounter]
        {
            taskCounter.fetch_add(1, std::memory_order_relaxed);
            return 0;
        }), ilrd::ThreadPool::HIGH));
    }

    for (auto& future : futures)
    {
        future.WaitResult();
    }

    std::cout << "test resualt: ";
    (NumberOftasks == taskCounter) ? 
    std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";
}

void BenchMark(void)
{
    std::vector<std::future<int>> futures;
    std::atomic<std::size_t> taskCounter(0);

    for (std::size_t i = 0; i < NumberOftasks; ++i)
    {
        futures.push_back(std::async(std::launch::async,
        [&taskCounter]
        {
            taskCounter.fetch_add(1, std::memory_order_relaxed);
            return 0;
        }));
    }

    for (auto& future : futures)
    {
        future.get();
    }

    std::cout << "test resualt: ";
    (NumberOftasks == taskCounter) ? 
    std::cout << "SUCCESS ✅\n" : std::cout << "FAIL ❌\n";
}

void TestBenchMark(void)
{
    std::cout << "BenchMarkThreadPool: " << std::endl;
    for (unsigned int i = 1; i < 256; i <<= 1)
    {
        auto start = std::chrono::high_resolution_clock::now();
        BenchMarkThreadPool(i);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> elapsed = end - start;
        std::cout << "number of threads = " << i << 
        " time: " << elapsed.count() << " ms" << std::endl;
    }

    std::cout << "same work without threads: " << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    BenchMark();
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "no thread pool: " <<
        " time: " << elapsed.count() << " ms" << std::endl;
}