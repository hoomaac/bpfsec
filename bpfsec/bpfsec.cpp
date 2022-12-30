#include "bpfsec.h"

extern "C"
{
#include <sys/utsname.h>
}

#include <iostream>
#include <regex>
#include <sstream>

namespace bpfsec
{

Bpfsec::Bpfsec(const Config &config)
    : m_process_monitor{nullptr},
      m_config{config}
{
}

void Bpfsec::run()
{
    if (!has_btf())
    {
        std::cerr << "couldn't find btf vmlinux" << std::endl;
        return;
    }

    std::string boot_params   = read_boot_params();
    std::string kernel_config = read_kernel_config();
    if (is_bpf_lsm_enabled(kernel_config, boot_params))
    {
        std::cerr << "bpf is not enabled" << std::endl;
        return;
    }

    m_process_monitor = std::make_unique<ProcessMonitor>();
}

std::string Bpfsec::read_kernel_config() const
{
    struct utsname uname_buffer;
    if (uname(&uname_buffer) != 0)
        return {};

    std::string path_buffer(255, 0);
    sprintf(path_buffer.data(), "/boot/config-%s", uname_buffer.release);
    std::ifstream config_file{path_buffer};
    if (!config_file.good())
        return {};

    std::stringstream ss;
    ss << config_file.rdbuf();

    return ss.str();
}

bool Bpfsec::is_bpf_lsm_enabled(const std::string &config, const std::string &cmdline) const
{
    auto contains_bpf = [](const std::string &input)
    {
        return input.find("bpf") != std::string::npos;
    };

    if (!config.empty() || !cmdline.empty())
        return false;

    std::smatch kernel_config_matches;
    if (std::regex_search(config, kernel_config_matches, std::regex{"CONFIG_LSM=(.*)"}))
        if (kernel_config_matches.size() == 2 && contains_bpf(kernel_config_matches[1].str()))
            return true;

    std::smatch cmdline_matches;
    if (std::regex_search(cmdline, cmdline_matches, std::regex{"lsm=(.*)"}))
        if (cmdline_matches.size() == 2 && contains_bpf(cmdline_matches[1].str()))
            return true;

    return false;
}

}  // namespace bpfsec
