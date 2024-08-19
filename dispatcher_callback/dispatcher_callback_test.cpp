/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 11/6/2023	               */
/* WORKSHEET NAME: Dispatcher Callback    */
/* REVIEWER: Aviv    	                 */
/*--------------------------------------*/

#include <iostream>
#include <vector>
#include <memory>

#include "dispatcher_callback.hpp"

struct Obs
{
    Obs() : cb(this, &Obs::NotifyFunc, &Obs::CloseFunc) {}

    Obs(const Obs& other) = delete;
    Obs& operator=(const Obs& other) = delete;

    void NotifyFunc(int i)
    {
        std::cout << "notify " << i << std::endl;
    }
    void CloseFunc()
    {
        std::cout << "close" << std::endl;
    }
    ilrd::Callback<Obs,int> cb;

};

class DataModel
{
public:

private:
    
};

class ViewWindow
{
public:
    ViewWindow() : m_Callback(this, &ViewWindow::Notify, &ViewWindow::Close) {}
    
    virtual void Notify(DataModel) { std::cout << "notify window" << std::endl;}
    virtual void Close() { std::cout << "close window" << std::endl; }
    ilrd::Callback<ViewWindow, DataModel> *GetCallBack() { return &m_Callback; }

private:
    ilrd::Callback<ViewWindow, DataModel> m_Callback;
};

class ControlWindow;

class SilyWindow : public ViewWindow
{
public:
    SilyWindow(ControlWindow *ctl) : ViewWindow(), m_controler(ctl) {}
    
    void Notify(DataModel);
    void Close();

private:
    ControlWindow *m_controler;
};


class ControlWindow
{
public:
    void AddObserver(ViewWindow *viewWindow)
    {
        m_Dispatcher.Register(viewWindow->GetCallBack());
    }
    void RemoveObserver(ViewWindow *viewWindow)
    {
        m_Dispatcher.Unregister(viewWindow->GetCallBack());
    }
    void Notify()
    {
        m_Dispatcher.NotifyAll(m_Data);
    }
    void Close()
    {
        m_Dispatcher.CloseAll();
    }

private:
    ilrd::Dispatcher<DataModel> m_Dispatcher;
    DataModel m_Data;
};

void TestDispatcherCallback();
void TestMVC();
void TestSelf();

int main()
{
    TestDispatcherCallback();
    TestMVC();
    TestSelf();

    return 0;
}

void TestDispatcherCallback()
{
    ilrd::Dispatcher<int> disp;
    std::vector<std::unique_ptr<Obs>> observers;
    const std::size_t numObservers = 10;

    std::cout << "test dispatcher callback" << std::endl;
    for (std::size_t i = 0; i < numObservers; ++i)
    {
        observers.emplace_back(new Obs());
        disp.Register(&(observers[i]->cb));
    }

    std::cout << "notify all:" << std::endl;
    disp.NotifyAll(1);

    std::cout << "unregister half :" << std::endl;
    for (std::size_t i = 0; i < numObservers; i += 2)
    {
        observers[i]->cb.Unregister();
    }

    std::cout << "notify all:" << std::endl;
    disp.NotifyAll(2);

    std::cout << "close all:" << std::endl;
    disp.CloseAll();

    for (std::size_t i = 0; i < numObservers; ++i)
    {
        disp.Register(&(observers[i]->cb));
    }

    std::cout << "notify all:" << std::endl;
    disp.NotifyAll(3);

}

void TestMVC()
{
    ControlWindow controlWindow;
    ViewWindow viewWindows1;
    ViewWindow viewWindows2;
    ViewWindow viewWindows3;

    std::cout << "test MVC: " << std::endl;
    std::cout << "add observers: " << std::endl;
    controlWindow.AddObserver(&viewWindows1);
    controlWindow.Notify();
    controlWindow.AddObserver(&viewWindows2);
    controlWindow.Notify();
    controlWindow.AddObserver(&viewWindows3);
    controlWindow.Notify();

    std::cout << "remove observers:" << std::endl;
    controlWindow.Notify();
    controlWindow.RemoveObserver(&viewWindows1);

    controlWindow.Notify();
    controlWindow.RemoveObserver(&viewWindows2);
    controlWindow.RemoveObserver(&viewWindows3);   
    controlWindow.Notify();

    controlWindow.Close();
}

void SilyWindow::Notify(DataModel)
{ 
    m_controler->AddObserver(this);
    std::cout << "notify window add itself" << std::endl;
}

void SilyWindow::Close()
{
    m_controler->RemoveObserver(this);
    std::cout << "close window remove itself" << std::endl; 
}

void TestSelf()
{
    ControlWindow controlWindow;
    SilyWindow viewWindows1(&controlWindow);
    SilyWindow viewWindows2(&controlWindow);
    SilyWindow viewWindows3(&controlWindow);

    std::cout << "test MVC: " << std::endl;
    controlWindow.AddObserver(&viewWindows1);
    controlWindow.AddObserver(&viewWindows2);
    controlWindow.AddObserver(&viewWindows3);
    
    std::cout << "add self in broadcast:" << std::endl;
    controlWindow.Notify();

    std::cout << "remove self in broadcast:" << std::endl;
    controlWindow.Close();

}