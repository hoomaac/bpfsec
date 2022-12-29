#include "vmlinux.h"

#include <libbpf/bpf_core_read.h>
#include <libbpf/bpf_helpers.h>
#include <libbpf/bpf_tracing.h>

#include "common_types.h"

char LICENSE[] SEC("license") = "GPL";

enum
{
    RING_BUFFER_FLAGS = 0,
    TASK_COMM_LEN     = 16,
    FILE_NAME_LEN     = 32,
};

struct
{
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 1 << 12);
} RingBuffer SEC(".maps");

SEC("lsm/bprm_committed_creds")
void BPF_PROG(bpfsec_process_mon, struct linux_binprm *bprm)
{
    long pid_tgid;
    struct ProcessInfo* process_info = NULL;
    struct task_struct* current_task = NULL;

    process_info = bpf_ringbuf_reserve(&RingBuffer, sizeof(*process_info), RING_BUFFER_FLAGS);
    if (!process_info)
        return;

    pid_tgid           = bpf_get_current_pid_tgid();
    process_info->pid  = pid_tgid;
    process_info->tgid = pid_tgid >> 32;

    current_task             = (struct task_struct *)bpf_get_current_task();
    process_info->parent_pid = BPF_CORE_READ(current_task, real_parent, pid);

    bpf_get_current_comm(&process_info->name, sizeof(process_info->name));

    bpf_ringbuf_submit(process_info, RING_BUFFER_FLAGS);
}