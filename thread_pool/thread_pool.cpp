/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 30/5/2023	               */
/* WORKSHEET NAME: Thread Pool            */
/* REVIEWER: Or 		                 */
/*--------------------------------------*/

#include <algorithm> // for_each
#include <iostream>  // cout

#include "thread_pool.hpp"

namespace ilrd
{

// ThreadPool class
ThreadPool::ThreadPool(unsigned int num_threads) : m_sync(new SyncAccess())
{
    MakeThreads(num_threads);
}

ThreadPool::~ThreadPool()
{
    if (!m_shutDown)
    {
        ShutDown();
    }
    
    if (!m_termination)
    {
        WaitTermination();
    }
}

ThreadPool::TaskFuture ThreadPool::
AddTask(std::shared_ptr<std::function<int(void)>> func, Priority priority)
{
    if (m_shutDown)
    {
        throw std::runtime_error("AddTask: ShutDown already called");
    }
    std::shared_ptr<Task> newTask(new Task(func, priority));
    
    m_tasksQueue.Push(newTask);

    return TaskFuture(newTask);
}

void ThreadPool::Pause()
{
    if (m_shutDown)
    {
        throw std::runtime_error("Pause: ShutDown already called");
    }
    if (m_pause)
    {
        throw std::runtime_error("Pause: ThreadPool already paused");
    }

    m_pause = true;

    for (std::size_t i = 0; i < m_threadsNum; ++i)
    {
        AddTask(std::make_shared<std::function<int(void)>>(
        [this](void)
        {
           m_stopSem.Wait();

           return 0;
        }), Priority(HIGH - 1));
    }
}

void ThreadPool::Resume()
{
    if (!m_pause)
    {
        throw std::runtime_error("Resume: ThreadPool not paused");
    }

    for (std::size_t i = 0; i < m_threadsNum; ++i)
    {
        m_stopSem.Post();
    }
    m_pause = false;
}

void ThreadPool::ShutDown()
{
    if (m_shutDown)
    {
        throw std::runtime_error("Pause: ShutDown already called");
    }

    for (std::size_t i = 0; i < m_threadsNum; ++i)
    {
        AddTask(std::make_shared<std::function<int(void)>>
        ([this](void)
        {
            std::thread::id id = std::this_thread::get_id();
            m_sync->StartRead();
            m_threads.at(id).second->Stop();
            m_sync->EndRead();

            return 0;
        }), static_cast<Priority>(LOW + 1));
    }

    m_shutDown = true;
}

void ThreadPool::SetNThreads(unsigned int num_threads)
{
    if (m_shutDown)
    {
        throw std::runtime_error("SetNTheads: ShutDown already called");
    }
    if (m_pause)
    {
        throw std::runtime_error("SetNTheads: ThreadPool is paused");
    }
    if (num_threads < 1)
    {
        throw std::runtime_error("SetNTheads: num_threads < 1");
    }
    
    if (num_threads >= m_threadsNum)
    {
        MakeThreads(num_threads - m_threadsNum);
    }
    else
    {
        std::size_t numThreadsToRemove = m_threadsNum - num_threads;

        m_detachNum.fetch_add(numThreadsToRemove, std::memory_order_acquire);
        for (std::size_t i = 0; i < numThreadsToRemove; ++i)
        {
            AddTask(std::make_shared<std::function<int(void)>>
            ([this](void)
            {
                std::thread::id id = std::this_thread::get_id();
                m_sync->StartRead();
                m_threads.at(id).second->Stop();
                m_threads.at(id).first->detach();
                m_threads.at(id).second->Detach();
                m_sync->EndRead();

                m_detachNum.fetch_sub(1, std::memory_order_release);
                m_barrier.notify_one();

                return 0;
            }), static_cast<Priority>(HIGH - 1));
        }
        m_threadsNum = num_threads;
    }
}

void ThreadPool::WaitTermination()
{
    if (!m_shutDown)
    {
        throw std::runtime_error("WaitTermination: ShutDown not called");
    }

    std::mutex mutex;
    std::unique_lock<std::mutex> lock(mutex);
    m_barrier.wait(lock, [this] 
    { 
        return (m_detachNum.load(std::memory_order_relaxed) == 0); 
    });

    m_sync->StartRead();
    auto copy = m_threads;
    m_sync->EndRead();

    for (auto& thread : copy)
    {
        if (thread.second.first->joinable())
        {
            thread.second.first->join();
        }
    }

    m_termination = true;
}

void ThreadPool::MakeThreads(unsigned int NumOfThreads)
{
    for (std::size_t i = 0; i < NumOfThreads; ++i)
    {
        std::shared_ptr<ThreadFunc> threadFunc(new ThreadFunc(*this));
        std::shared_ptr<std::thread> thread(new std::thread(std::ref(*threadFunc)));
        std::thread::id id = thread->get_id();
        m_sync->StartWrite();
        m_threads.emplace(id, std::make_pair(thread, threadFunc));
        m_sync->EndWrite();
        threadFunc->Start();
    }

    m_threadsNum += NumOfThreads;
}

// class FunctorLess
bool ThreadPool::FunctorLess::operator()(const std::shared_ptr<Task>& lhs, 
                        const std::shared_ptr<Task>& rhs) const
{
    return (*lhs < *rhs);
}

// Task class
ThreadPool::Task::Task(std::shared_ptr<std::function<int(void)>> func, 
Priority priority) : m_func(func), m_priority(priority)
{}

bool ThreadPool::Task::operator<(const Task& rhs) const
{
    return (m_priority > rhs.m_priority);
}


void ThreadPool::Task::Run()
{
    m_status = RUN;
    try
    {
      (*m_func)();
    }
    catch(std::exception& e) 
    {
        m_exeption = e;
        m_isExeption = true;
    }

    m_status = DONE;
    m_sem.Post();
}

int ThreadPool::Task::WaitResult(std::chrono::milliseconds timeout) const
{
    if (m_isExeption)
    {
        throw m_exeption;
    }
    return (!m_sem.WaitFor(timeout));
}

int ThreadPool::Task::GetStatus() const
{
    return m_status;
}

// TaskFuture class
ThreadPool::TaskFuture::TaskFuture(std::shared_ptr<Task> task) : m_task(task)
{
}

int ThreadPool::TaskFuture::WaitResult(std::chrono::milliseconds timeout) const
{
    return (m_task->WaitResult(timeout));
}

int ThreadPool::TaskFuture::GetStatus() const
{
    return (m_task->GetStatus());
}

// ThreadFunc class
ThreadPool::ThreadFunc::ThreadFunc(ThreadPool& threadPool) : 
m_pool(threadPool), m_psem(new Semaphore(0))
{}

void ThreadPool::ThreadFunc::operator()(void)
{
    m_psem->Wait();

    while (m_run)
    {
        std::shared_ptr<Task> task(nullptr);
        m_pool.m_tasksQueue.Pop(task);
        task->Run();
    }

    if (m_detached)
    {
        std::shared_ptr<SyncAccess> sync(m_pool.m_sync);
        sync->StartWrite();
        m_pool.m_threads.erase(std::this_thread::get_id());
        sync->EndWrite();
    }
}

void ThreadPool::ThreadFunc::Stop()
{
    m_run = false;
}

void ThreadPool::ThreadFunc::Start()
{
    m_psem->Post();
}

void ThreadPool::ThreadFunc::Detach()
{
    m_detached = true;
}

// SyncAccess class
void ThreadPool::SyncAccess::StartRead()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_readers.fetch_add(1, std::memory_order_acquire);

}

void ThreadPool::SyncAccess::EndRead()
{
    m_readers.fetch_sub(1, std::memory_order_release);
}

void ThreadPool::SyncAccess::StartWrite()
{
    m_mutex.lock();
    while (m_readers.load(std::memory_order_relaxed) > 0);
}

void ThreadPool::SyncAccess::EndWrite()
{
    m_mutex.unlock();
}



} // namespace ilrd