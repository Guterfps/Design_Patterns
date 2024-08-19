/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 22/5/2023	               */
/* WORKSHEET NAME: waitable queueu        */
/* REVIEWER: Gal		                 */
/*--------------------------------------*/

#ifndef ILRD_RD136_7_WAITABLE_QUEUE_HPP
#define ILRD_RD136_7_WAITABLE_QUEUE_HPP

#include <mutex> // mutex
#include "semaphore.hpp" // semaphore

namespace ilrd
{
/* 
Container	-	The type of the underlying container to use to store the 
                elements. The container must satisfy the requirements of 
                SequenceContainer, and its iterators must satisfy the 
                requirements of LegacyRandomAccessIterator. Additionally, 
                it must provide the following functions with the usual semantics:
                front()
                push()
                pop()
                empty().
                The standard containers std::vector (including std::vector<bool>) 
                and std::deque satisfy these requirements.
 */

template<class Container>
class WaitableQueue
{
public:
    WaitableQueue() = default;
    WaitableQueue(const WaitableQueue&) = delete;
    WaitableQueue& operator=(const WaitableQueue&) = delete;

    void Push(const typename Container::value_type& data);
    bool Pop(typename Container::reference);
    bool Pop(typename Container::reference, std::chrono::milliseconds timeout);
    bool IsEmpty() const;                           //not thread safe

private:
    Container m_container;
    Semaphore m_sem{0};
    mutable std::mutex m_mutex;
};

template <class Container>
void WaitableQueue<Container>::Push(const typename Container::value_type& data)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_container.push(data);
    m_sem.Post();
}

template <class Container>
bool WaitableQueue<Container>::Pop(typename Container::reference data)
{
    return (Pop(data, std::chrono::hours(INT32_MAX)));
}

template <class Container>
bool WaitableQueue<Container>::Pop(typename Container::reference data,
                                    std::chrono::milliseconds timeout)
{
    bool isWaitSuccessful = m_sem.WaitFor(timeout);

    if (isWaitSuccessful)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        data = m_container.front();
        m_container.pop();
    }

    return isWaitSuccessful;
}

template <class Container>
bool WaitableQueue<Container>::IsEmpty() const
{
    return (m_container.empty());
}

}//ilrd

#endif    // ILRD_RD136_7_WAITABLE_QUEUE_HPP 