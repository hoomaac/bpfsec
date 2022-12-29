extern "C"
{
#include <sys/utsname.h>
}

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

#include "process_monitor.hpp"

using bpfsec::ProcessMonitor;

std::string read_kernel_config()
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

std::string read_boot_params()
{
    std::ifstream cmdline{"/proc/cmdline"};
    if (!cmdline.good())
        return {};

    std::stringstream ss;
    ss << cmdline.rdbuf();

    return ss.str();
}

bool is_bpf_lsm_enabled(const std::string &config, const std::string &cmdline)
{
    auto contains_bpf = [](const std::string &input) { return input.find("apparmor") != std::string::npos; };

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

bool has_btf()
{
    constexpr char BTF_PATH[] = "/sys/kernel/btf/vmlinux";
    return std::filesystem::exists(BTF_PATH);
}

int main(int argc, char const *argv[])
{
    std::string config      = read_kernel_config();
    std::string boot_params = read_boot_params();
    if (!is_bpf_lsm_enabled(config, boot_params))
    {
        std::cerr << "bpf should added to boot params!" << std::endl;
        return 1;
    }

    if (geteuid() != 0)
    {
        std::cerr << "run as root!" << std::endl;
        return 1;
    }

    ProcessMonitor process_monitor;

    return 0;
}
