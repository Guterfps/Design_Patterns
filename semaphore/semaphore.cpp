
#include "semaphore.hpp"

namespace ilrd
{

Semaphore::Semaphore(std::size_t count) : m_count(count)
{}

void Semaphore::Wait()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cond.wait(lock, [this]
    { return (m_count.load(std::memory_order_relaxed) > 0); });
    m_count.fetch_sub(1);
}


bool Semaphore::WaitFor(std::chrono::milliseconds timeout)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    auto ret = m_cond.wait_for(lock, timeout, 
    [this]{ return (m_count.load(std::memory_order_relaxed) > 0); });
    if (ret)
    {
        m_count.fetch_sub(1);
    }

    return ret;
}

void Semaphore::Post()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_count.fetch_add(1);
    m_cond.notify_one();
}

} // ilrd