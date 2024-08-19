/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 4/6/2023	               */
/* WORKSHEET NAME: Singleton              */
/* REVIEWER: Eitan 		                 */
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
    Factory() = default;
    std::unordered_map<KEY, std::function<BASE *(ARGS...)>> m_map;
    
    friend Singleton<Factory<BASE, KEY, ARGS...>>;
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