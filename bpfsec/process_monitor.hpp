#ifndef BPFSEC_PROCESS_MONITOR_HPP_
#define BPFSEC_PROCESS_MONITOR_HPP_

#include <thread>
#include <functional>

namespace bpfsec
{

/// @brief Monitor all processes using eBPF and LSM Hooks
class ProcessMonitor
{
public:
    ProcessMonitor();
    ~ProcessMonitor();

    void start();

private:
    static int event_handler(void* contex, void* data, size_t size);

    std::thread m_thread;
    bool m_stop;
};

}  // namespace bpfsec

#endif  // BPFSEC_PROCESS_MONITOR_HPP_