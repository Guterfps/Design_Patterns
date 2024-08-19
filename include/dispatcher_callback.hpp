/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 11/6/2023	               */
/* WORKSHEET NAME: Dispatcher Callback    */
/* REVIEWER: Aviv    	                 */
/*--------------------------------------*/

#ifndef ILRD_RD136_7_DISPATCHER_CALLBACK_HPP
#define ILRD_RD136_7_DISPATCHER_CALLBACK_HPP

#include <functional>       // std::function
#include <unordered_set>    // std::unordered_set
#include <algorithm>        // std::for_each

namespace ilrd
{

// EVENT must be copyable

template <class EVENT>
class Dispatcher;

template <class EVENT>
class CallbackBase
{
public:
    CallbackBase() = default;
    virtual ~CallbackBase() = default;
    CallbackBase(const CallbackBase&) = delete;
    CallbackBase& operator=(const CallbackBase&) = delete;
    
    virtual void Notify(EVENT event) =0;
    virtual void Close() =0;
    void Unregister();

private:
    void SetDispatcher(Dispatcher<EVENT> *dispatcher);
    friend Dispatcher<EVENT>;

    Dispatcher<EVENT> *m_dispatcher;
};

template <class OBS, class EVENT>
class Callback : public CallbackBase<EVENT>
{
    typedef void (OBS::*NotifyFunc_t)(EVENT event);
    typedef void (OBS::*CloseFunc_t)();

public:
    Callback(OBS *observer, NotifyFunc_t notify, CloseFunc_t close);
    ~Callback();
    void Notify(EVENT event);
    void Close();

private:
    OBS *m_observer;
    NotifyFunc_t m_notify;
    CloseFunc_t m_close;
};

template <class EVENT>
class Dispatcher
{
public:
    Dispatcher() = default;
    ~Dispatcher() = default;
    Dispatcher(const Dispatcher&) = delete;
    Dispatcher& operator=(const Dispatcher&) = delete;

    void Register(CallbackBase<EVENT> *callback);
    void Unregister(CallbackBase<EVENT> *callback);
    void NotifyAll(EVENT event);
    void CloseAll();

private:
    std::unordered_set<CallbackBase<EVENT> *> m_callbacks;
};

template <class EVENT>
void CallbackBase<EVENT>::SetDispatcher(Dispatcher<EVENT> *dispatcher)
{
    m_dispatcher = dispatcher;
}

template <class EVENT>
void CallbackBase<EVENT>::Unregister()
{
    if (m_dispatcher)
    {
        m_dispatcher->Unregister(this);
    }
}

template <class OBS, class EVENT>
Callback<OBS, EVENT>::Callback(OBS *observer, NotifyFunc_t notify, 
                                            CloseFunc_t close) :
m_observer(observer), m_notify(notify), m_close(close)
{
    if (nullptr == m_observer)
    {
        throw std::invalid_argument("Callback::Ctor: Observer is null");
    }
    if (nullptr == m_notify)
    {
        throw std::invalid_argument("Callback::Ctor: Notify is null");
    }
    if (nullptr == m_close)
    {
        throw std::invalid_argument("Callback::Ctor: Close is null");
    }
}

template <class OBS, class EVENT>
Callback<OBS, EVENT>::~Callback()
{
    this->Unregister();
}

template <class OBS, class EVENT>
void Callback<OBS, EVENT>::Notify(EVENT event)
{
    (m_observer->*m_notify)(event);
}

template <class OBS, class EVENT>
void Callback<OBS, EVENT>::Close()
{
    (m_observer->*m_close)();
}

template <class EVENT>
void Dispatcher<EVENT>::Register(CallbackBase<EVENT> *callback)
{
    if (nullptr == callback)
    {
        throw std::invalid_argument("Dispatcher::Register: Callback is null");
    }
    m_callbacks.insert(callback);
    callback->SetDispatcher(this);
}

template <class EVENT>
void Dispatcher<EVENT>::Unregister(CallbackBase<EVENT> *callback)
{
    auto it = m_callbacks.find(callback);
    if (it != m_callbacks.end())
    {
        (*it)->SetDispatcher(nullptr);
        m_callbacks.erase(it);
    }
}

template <class EVENT>
void Dispatcher<EVENT>::NotifyAll(EVENT event)
{
    auto copy = m_callbacks;
    std::for_each(copy.begin(), copy.end(),
    [event](CallbackBase<EVENT> *cb)
    {
        cb->Notify(event);
    });
}

template <class EVENT>
void Dispatcher<EVENT>::CloseAll()
{
    auto copy = m_callbacks;
    std::for_each(copy.begin(), copy.end(),
    [](CallbackBase<EVENT> *cb)
    {
        cb->Close();
    });
    m_callbacks.clear();
}

}

#endif // ILRD_RD136_7_DISPATCHER_CALLBACK_HPP 