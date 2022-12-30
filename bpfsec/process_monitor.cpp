#include "process_monitor.h"

extern "C"
{
#include <libbpf/libbpf.h>
}

#include <bpfkern/bpfsec_process_mon_skel.h>
#include <bpfkern/common_types.h>

#include <iostream>
#include <memory>

using namespace bpfsec;

struct BpfsecDeleter
{
    void operator()(bpfsec_process_mon_c* ptr)
    {
        bpfsec_process_mon_c__destroy(ptr);
    }
};

struct RingBufferDeleter
{
    void operator()(ring_buffer* ptr)
    {
        ring_buffer__free(ptr);
    }
};

ProcessMonitor::ProcessMonitor()
    : m_stop{false}
{
    m_thread = std::thread(&ProcessMonitor::start, this);
}

ProcessMonitor::~ProcessMonitor()
{
    m_stop = true;
    if (m_thread.joinable())
        m_thread.join();
}

void ProcessMonitor::start()
{
    constexpr int8_t RING_BUFFER_TIME_OUT = -1;
    std::unique_ptr<bpfsec_process_mon_c, BpfsecDeleter> process_monitor{bpfsec_process_mon_c__open_and_load()};

    if (!process_monitor)
    {
        std::cerr << "couldn't load the BPF program!" << std::endl;
        return;
    }

    if (bpfsec_process_mon_c__attach(process_monitor.get()) != 0)
    {
        std::cerr << "couldn't attach to BPF program!";
        return;
    }

    int ring_buf_fd = bpf_map__fd(process_monitor->maps.RingBuffer);
    if (ring_buf_fd < 0)
    {
        std::cerr << "couldn't get the file descriptor of the ring buffer!";
        return;
    }

    std::unique_ptr<ring_buffer, RingBufferDeleter> ring_buf{
        ring_buffer__new(ring_buf_fd, &ProcessMonitor::event_handler, nullptr, nullptr)};

    if (!ring_buf)
    {
        std::cerr << "couldn't create ring buffer!";
        return;
    }

    while (!m_stop)
    {
        int ret = ring_buffer__poll(ring_buf.get(), RING_BUFFER_TIME_OUT);
        if (ret < 0)
        {
            std::cerr << "bpf ring buffer returned error: " << ret << std::endl;
            break;
        }
    }
}

int ProcessMonitor::event_handler(void* contex, void* data, size_t size)
{
    ProcessInfo* process_info = reinterpret_cast<ProcessInfo*>(data);
    std::cout << process_info->name << "\t" << process_info->pid << "\t" << process_info->parent_pid << "\t"
              << process_info->tgid;

    return 0;
}
