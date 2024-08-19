
#ifndef ILRD_RD136_7_PQ_HPP
#define ILRD_RD136_7_PQ_HPP

#include <queue>
#include <vector>
#include <functional>

namespace ilrd
{

template <class T,
class Container = std::vector<T>,
class Compare = std::less<typename Container::value_type>>
class PriorityQueue : private std::priority_queue<T, Container, Compare>
{
    typedef typename std::priority_queue<T, Container, Compare> pq_t;
public: 
    typedef typename pq_t::value_type value_type_t;
    typedef typename pq_t::reference reference_t;

    using pq_t::push;
    using pq_t::pop;
    using pq_t::empty;
    const value_type_t& Front() const;
    
};

template <class T, class Container, class Compare>
inline const typename PriorityQueue<T, Container, Compare>::value_type_t& 
PriorityQueue<T, Container, Compare>::Front() const
{
    return this->top();
}

} // namespace ilrd

#endif //ILRD_RD136_7_PQ_HPP