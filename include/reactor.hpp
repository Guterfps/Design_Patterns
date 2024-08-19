/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 15/6/2023	               */
/* WORKSHEET NAME: Reactor                */
/* REVIEWER: Gali 		                 */
/*--------------------------------------*/

#ifndef ILRD_RD136_7_REACTOR_HPP
#define ILRD_RD136_7_REACTOR_HPP

#include <vector>           // std::vector
#include <functional>       // std::function<>
#include <unordered_map>     // std::unordered_map
#include <sys/select.h>      // fd_set

namespace ilrd
{

class Reactor
{
public:
    enum Mode : int
    {
        READ,
        WRITE,
        EXCEPT,
        NUM_OF_MODES
    };

    Reactor();
    ~Reactor() = default;

    Reactor(const Reactor&) = delete;
    Reactor& operator=(const Reactor&) = delete;

    void Add(int fd, Mode mode, const std::function<void(void)>& callback);
    void Remove(int fd, Mode mode);

    void Run(); // blocking
    void Stop();

private:

    class FDListener
    {
    public:
        typedef std::pair<int, Mode> fd_mode_t;

        FDListener(int timeoutSec);
        ~FDListener() = default;
        FDListener(const FDListener&) = delete;
        FDListener& operator=(const FDListener&) = delete;
        
        std::vector<fd_mode_t> Wait(const std::vector<fd_mode_t>& fd);
    private:
        int m_timeoutSec;

        static void FillFdSets(const std::vector<fd_mode_t>& fds, 
                                fd_set *fdSets, int& maxFd);
        static void FillReadyFds(const std::vector<fd_mode_t>& fds, 
                                std::vector<fd_mode_t>& readyFds,
                                fd_set *fdSets);
    };

    static constexpr int TIMEOUT_SEC = 1;
    std::vector<FDListener::fd_mode_t> m_fds;
    std::unordered_map<int, std::function<void(void)>> m_callbacks[NUM_OF_MODES];
    FDListener m_fdListener;
    bool m_isRunning;
};

}   // namespace ilrd

#endif /* ILRD_RD136_7_REACTOR_HPP */