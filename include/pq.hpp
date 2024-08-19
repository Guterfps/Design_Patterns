
#ifndef ILRD_RD136_7_PQ_HPP
#define ILRD_RD136_7_PQ_HPP

#include <queue>
#include <vector>
#include <functional>

namespace ilrd
{
/* 
T	-	The type of the stored elements. The behavior is undefined if T is not 
        the same type as Container::value_type.

Container	-	The type of the underlying container to use to store the 
                elements. The container must satisfy the requirements of 
                SequenceContainer, and its iterators must satisfy the 
                requirements of LegacyRandomAccessIterator. Additionally, 
                it must provide the following functions with the usual semantics:
                front()
                push_back()
                pop_back().
                The standard containers std::vector (including std::vector<bool>) 
                and std::deque satisfy these requirements.
 
Compare	-	A Compare type providing a strict weak ordering.
            Note that the Compare parameter is defined such that it returns true 
            if its first argument comes before its second argument in a weak 
            ordering. But because the priority queue outputs largest elements 
            first, the elements that "come before" are actually output last. 
            That is, the front of the queue contains the "last" element 
            according to the weak ordering imposed by Compare.
 
 */

template <class T,
class Container = std::vector<T>,
class Compare = std::less<typename Container::value_type>>
class PriorityQueue : private std::priority_queue<T, Container, Compare>
{
    typedef typename std::priority_queue<T, Container, Compare> pq_t;
public: 
    typedef typename pq_t::value_type value_type;
    typedef typename pq_t::reference reference;

    using pq_t::pq_t;
    using pq_t::push;
    using pq_t::pop;
    using pq_t::empty;
    const value_type& front() const;
    
};

template <class T, class Container, class Compare>
inline const typename PriorityQueue<T, Container, Compare>::value_type& 
PriorityQueue<T, Container, Compare>::front() const
{
    return this->top();
}

} // namespace ilrd

#endif //ILRD_RD136_7_PQ_HPP