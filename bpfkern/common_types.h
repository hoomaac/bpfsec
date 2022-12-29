#ifndef BPFKERN_COMMON_TYPES_H_
#define BPFKERN_COMMON_TYPES_H_

#include <linux/limits.h>

struct ProcessInfo
{
    uint pid;
    uint parent_pid;
    uint tgid;
    char name[PATH_MAX];
};

#endif  // BPFKERN_COMMON_TYPES_H_