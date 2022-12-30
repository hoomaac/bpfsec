#ifndef BPFSEC_BPFSEC_H_
#define BPFSEC_BPFSEC_H_

#include <string>
#include <memory>
#include <filesystem>
#include <filesystem>
#include <fstream>

#include <yaml-cpp/yaml.h>

#include "process_monitor.h"
#include "config.h"

namespace bpfsec
{

class Bpfsec
{
public:
    explicit Bpfsec(const Config &config);

    void run();
    std::string read_boot_params() const;
    bool has_btf() const;
    std::string read_kernel_config() const;
    bool is_bpf_lsm_enabled(const std::string &config, const std::string &cmdline) const;

private:
    /// @brief Path of BTF Vmlinux
    constexpr static char BTF_PATH[] = "/sys/kernel/btf/vmlinux";

    /// @brief Bpfsec Config
    Config m_config;

    /// @brief ProcessMonitor instance
    std::unique_ptr<ProcessMonitor> m_process_monitor;
};

inline bool Bpfsec::has_btf() const
{
    return std::filesystem::exists(BTF_PATH);
}

inline std::string Bpfsec::read_boot_params() const
{
    std::ifstream cmdline{"/proc/cmdline"};
    if (!cmdline.good())
        return {};

    std::stringstream ss;
    ss << cmdline.rdbuf();

    return ss.str();
}

}  // namespace bpfsec

#endif  // BPFSEC_BPFSEC_H_