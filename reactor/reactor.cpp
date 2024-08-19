/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 15/6/2023	               */
/* WORKSHEET NAME: Reactor                */
/* REVIEWER: Gali 		                 */
/*--------------------------------------*/

#include <algorithm> // std::find
#include <stdexcept> // std::runtime_error
#include <sys/select.h> // select

#include "reactor.hpp"

namespace ilrd
{

// Reactor class
Reactor::Reactor() : m_fdListener(TIMEOUT_SEC), m_isRunning{false}
{}

void Reactor::Add(int fd, Mode mode, const std::function<void(void)>& callback)
{
    if (0 > fd)
    {
        throw std::runtime_error("Invalid file descriptor");
    }

    if (!callback)
    {
        throw std::runtime_error("callback is empty");
    }

    if (m_callbacks[mode].find(fd) == m_callbacks[mode].end())
    {  
        m_fds.push_back(std::make_pair(fd, mode));
    }
    m_callbacks[mode][fd] = callback;
}

void Reactor::Remove(int fd, Mode mode)
{
    if (m_callbacks[mode].erase(fd))
    {
        auto it = std::find(m_fds.begin(), m_fds.end(), 
                            std::make_pair(fd, mode));
        m_fds.erase(it);
    }
}

void Reactor::Run()
{
    if (m_isRunning)
    {
        throw std::runtime_error("Reactor already running");
    }

    m_isRunning = true;

    while (m_isRunning)
    {
        std::vector<FDListener::fd_mode_t> readyFds = 
                                            std::move(m_fdListener.Wait(m_fds));
        auto vecEnd = readyFds.end();
        for (auto fd = readyFds.begin(); m_isRunning && (fd != vecEnd); ++fd)
        {
            std::unordered_map<int, std::function<void(void)>>& callbackMap =
            m_callbacks[fd->second];
            if (callbackMap.find(fd->first) != callbackMap.end())
            {
                try
                {
                    callbackMap.at(fd->first)();
                } 
                catch (...) {}
            }
        }
    } 
}

void Reactor::Stop()
{
    m_isRunning = false;
}

// FDListener class
Reactor::FDListener::FDListener(int timeoutSec) : m_timeoutSec(timeoutSec)
{}

std::vector<Reactor::FDListener::fd_mode_t> 
Reactor::FDListener::Wait(const std::vector<FDListener::fd_mode_t>& fd)
{
    std::vector<fd_mode_t> readyFds;
    fd_set fdSets[NUM_OF_MODES];
    struct timeval timeout{m_timeoutSec, 0};
    int maxFd = 0;

    FillFdSets(fd, fdSets, maxFd);
    
    int ret = select(++maxFd, &fdSets[READ], &fdSets[WRITE], 
                        &fdSets[EXCEPT], &timeout);
    if (-1 == ret)
    {
        throw std::runtime_error("select failed");
    }

    FillReadyFds(fd, readyFds, fdSets);
    
    return readyFds;
}

inline void Reactor::FDListener::FillFdSets(const std::vector<fd_mode_t>& fds, 
                                fd_set *fdSets, int& maxFd)
{
    FD_ZERO(&fdSets[READ]);
    FD_ZERO(&fdSets[WRITE]);
    FD_ZERO(&fdSets[EXCEPT]);

    for (auto& f : fds)
    {
        FD_SET(f.first, &fdSets[f.second]);
        maxFd = std::max(maxFd, f.first);
    }
}

inline void Reactor::FDListener::FillReadyFds(const std::vector<fd_mode_t>& fds,
                                        std::vector<fd_mode_t>& readyFds,
                                        fd_set *fdSets)
{
    for (auto& f : fds)
    {
        if (FD_ISSET(f.first, &fdSets[f.second]))
        {
            readyFds.push_back(f);
        }
    }
}

} // ilrd namespace