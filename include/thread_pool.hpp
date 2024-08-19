/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 30/5/2023	               */
/* WORKSHEET NAME: Thread Pool            */
/* REVIEWER: Or 		                 */
/*--------------------------------------*/

#ifndef ILRD_RD136_7_THREAD_POOL_HPP
#define ILRD_RD136_7_THREAD_POOL_HPP

#include <thread>   // hardware_concurrency
#include <functional> // function
#include <unordered_map> // unordered_map
#include <atomic> // atomic
#include <condition_variable> // condition_variable

#include "semaphore.hpp"      // Semaphore
#include "pq.hpp" // PriorityQueue
#include "waitable_queue.hpp" // WaitableQueue

namespace ilrd
{
class ThreadPool
{
public:
    class TaskFuture;

    enum Status : int
    {
        WAIT = 0,
        RUN,
        DONE
    };

    enum Priority : int
    {
        HIGH = 0,
        MID,
        LOW
    };


    explicit ThreadPool(unsigned int threadsNum = 
                        std::thread::hardware_concurrency());
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ~ThreadPool();

    TaskFuture AddTask(std::shared_ptr<std::function<int(void)>> func, 
                        Priority priority);  
    void Pause();
    void Resume();
    void ShutDown();
    void SetNThreads(unsigned int threadsNum);
    void WaitTermination();

private:
    class Task;
    class ThreadFunc;
    class SyncAccess;

    class FunctorLess
    {
    public:
        bool operator()(const std::shared_ptr<Task>& lhs, 
                        const std::shared_ptr<Task>& rhs) const;
    };

    std::unordered_map<std::thread::id, 
                std::pair<std::shared_ptr<std::thread>, 
                std::shared_ptr<ThreadFunc>>> m_threads;
    WaitableQueue<PriorityQueue<std::shared_ptr<Task>, 
                std::vector<std::shared_ptr<Task>>,
                FunctorLess>> m_tasksQueue;
    
    Semaphore m_stopSem{0};
    bool m_shutDown{false};
    bool m_pause{false};
    bool m_termination{false};
    std::size_t m_threadsNum{0};
    std::shared_ptr<SyncAccess> m_sync;
    std::atomic<std::size_t> m_detachNum{0};
    std::condition_variable m_barrier;

    void MakeThreads(unsigned int NumOfThreads);
};

class ThreadPool::TaskFuture
{
public:
    explicit TaskFuture(std::shared_ptr<Task> task);

    int WaitResult(std::chrono::milliseconds timeout = 
                  std::chrono::milliseconds(INT32_MAX)) const;
    int GetStatus() const;

private:
    std::shared_ptr<Task> m_task;
};

class ThreadPool::Task
{
public:
    explicit Task(std::shared_ptr<std::function<int(void)>> func, 
                    Priority priority);

    bool operator<(const Task& rhs) const;
    void Run();
    int WaitResult(std::chrono::milliseconds timeout = 
                  std::chrono::milliseconds(INT32_MAX)) const; 
    int GetStatus() const;

private:
    std::shared_ptr<std::function<int(void)>> m_func;
    mutable Semaphore m_sem{0};
    int m_priority;
    int m_status{WAIT};
    bool m_isExeption{false};
    std::exception m_exeption;
};

class ThreadPool::ThreadFunc
{
public:
    ThreadFunc(ThreadPool& threadPool);

    void operator()(void);
    void Stop();
    void Start();
    void Detach();

private:
    ThreadPool& m_pool;
    bool m_run{true};
    bool m_detached{false};
    std::shared_ptr<Semaphore> m_psem;
};

class ThreadPool::SyncAccess
{
public:
    SyncAccess() = default;
    void StartRead();
    void EndRead();
    void StartWrite();
    void EndWrite();

private:
    std::atomic<std::size_t> m_readers{0};
    std::mutex m_mutex;

};

} //namespace ilrd

#endif /* ILRD_RD136_7_THREAD_POOL_HPP */