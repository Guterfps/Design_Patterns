/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 1/6/2023	               */
/* WORKSHEET NAME: Factory                */
/* REVIEWER: Or 		                 */
/*--------------------------------------*/

#ifndef ILRD_RD136_7_FACTORY_HPP
#define ILRD_RD136_7_FACTORY_HPP

#include <unordered_map>
#include <functional>

namespace ilrd
{

template <typename BASE, typename KEY, typename... ARGS>
class Factory
{
public:
    void Add(KEY key, std::function<BASE *(ARGS...)> creator);
    BASE *Create(KEY key, ARGS... args);

private:
    std::unordered_map<KEY, std::function<BASE *(ARGS...)>> m_map;
    
};

template <typename BASE, typename KEY, typename... ARGS>
void Factory<BASE, KEY, ARGS...>::Add(KEY key, 
                                    std::function<BASE *(ARGS...)> creator)
{
    m_map[key] = creator;
}

template <typename BASE, typename KEY, typename... ARGS>
BASE *Factory<BASE, KEY, ARGS...>::Create(KEY key, ARGS... args)
{
    return m_map.at(key)(args...);
}

} //namespace ilrd

#endif /* ILRD_RD136_7_FACTORY_HPP */