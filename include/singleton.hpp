/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 4/6/2023	               */
/* WORKSHEET NAME: Singleton              */
/* REVIEWER: Eitan 		                 */
/*--------------------------------------*/

#ifndef ILRD_RD136_7_SINGLETON_HPP
#define ILRD_RD136_7_SINGLETON_HPP

#include <atomic> // atomic
#include <mutex> // mutex
#include <cstdlib> // atexit

namespace ilrd
{

template <typename T, typename... Args>
class Singleton
{
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static T *GetInstance(Args... args);

private:
    Singleton() = default;

    static std::mutex m_mutex;
};

template <typename T, typename... Args>
std::mutex Singleton<T, Args...>::m_mutex;

template <typename T, typename... Args>
T *Singleton<T, Args...>::GetInstance(Args... args)
{
    static std::atomic<T *> m_instance(nullptr);
    T *temp = m_instance.load(std::memory_order_acquire);

    if (nullptr == temp)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        temp = m_instance.load(std::memory_order_relaxed);
        
        if (nullptr == temp)
        {
            temp = new T(args...);
            m_instance.store(temp, std::memory_order_release);
            std::atexit([]()
            {
                delete m_instance.load();
                m_instance.store(nullptr);
            });
        }
    }

    return temp;
}

} // namespace ilrd

#endif // ILRD_RD136_7_SINGLETON_HPP