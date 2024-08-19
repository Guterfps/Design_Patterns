/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 4/6/2023	               */
/* WORKSHEET NAME: Singleton              */
/* REVIEWER: Eitan 		                 */
/*--------------------------------------*/

#ifndef ILRD_RD136_7_SIMPLE_SINGLETON_HPP
#define ILRD_RD136_7_SIMPLE_SINGLETON_HPP

#include <functional> // std::function
#include <unordered_map> // unordered_map
#include <cstdlib> // atexit

namespace ilrd
{

template <typename BASE, typename KEY, typename... ARGS>
class FactorySingleton
{
public:
    FactorySingleton(const FactorySingleton &) = delete;
    FactorySingleton &operator=(const FactorySingleton &) = delete;

    static FactorySingleton *GetInstance();
    void Add(KEY key, std::function<BASE *(ARGS...)> creator);
    BASE *Create(KEY key, ARGS... args);

private:
    FactorySingleton();
    std::unordered_map<KEY, std::function<BASE *(ARGS...)>> m_map;
    static FactorySingleton<BASE, KEY, ARGS...>* m_instance;

};

template <typename BASE, typename KEY, typename... ARGS>
FactorySingleton<BASE, KEY, ARGS...> *
FactorySingleton<BASE, KEY, ARGS...>::m_instance = 
new FactorySingleton<BASE, KEY, ARGS...>();

template <typename BASE, typename KEY, typename... ARGS>
FactorySingleton<BASE, KEY, ARGS...>::FactorySingleton()
{
    std::atexit([](){delete m_instance;});
}

template <typename BASE, typename KEY, typename... ARGS>
FactorySingleton<BASE, KEY, ARGS...> *
FactorySingleton<BASE, KEY, ARGS...>::GetInstance()
{
    return m_instance;
}

template <typename BASE, typename KEY, typename... ARGS>
void FactorySingleton<BASE, KEY, ARGS...>::Add(KEY key, 
                                    std::function<BASE *(ARGS...)> creator)
{
    m_map[key] = creator;
}

template <typename BASE, typename KEY, typename... ARGS>
BASE *FactorySingleton<BASE, KEY, ARGS...>::Create(KEY key, ARGS... args)
{
    return m_map.at(key)(args...);
}

} //namespace ilrd

#endif // ILRD_RD136_7_SIMPLE_SINGLETON_HPP