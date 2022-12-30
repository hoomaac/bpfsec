#ifndef BPFSEC_CONFIG_H_
#define BPFSEC_CONFIG_H_

#include <string>
#include <filesystem>
#include <map>
#include <vector>

#include <yaml-cpp/yaml.h>

namespace bpfsec
{

using ProcessConfigType = std::map<std::string, std::string>;

struct ProcessConfig
{
    uint pid;
    uint parent_pid;
    std::string name;
    std::string action;
};

struct Config
{
    std::vector<ProcessConfig> process_configs;
};

inline std::vector<ProcessConfig> get_process_config(const YAML::Node& config)
{
    std::vector<ProcessConfigType> process_config_yaml = config["process"].as<std::vector<ProcessConfigType>>();
    std::vector<ProcessConfig> process_configs;
    std::for_each(process_config_yaml.begin(), process_config_yaml.end(),
                  [&](const ProcessConfigType& p)
                  {
                      ProcessConfig process_config;
                      process_config.pid        = atoi(p.at("pid").c_str());
                      process_config.parent_pid = atoi(p.at("parent_pid").c_str());
                      process_config.name       = p.at("name");
                      process_config.action     = p.at("action");
                      process_configs.push_back(process_config);
                  });

    return process_configs;
}

inline Config get_config(const std::string& config_path)
{
    if (!std::filesystem::exists(config_path))
        return {};

    YAML::Node yaml_config = YAML::LoadFile(config_path);

    return Config{.process_configs = get_process_config(yaml_config)};
}

}  // namespace bpfsec

#endif  // BPFSEC_CONFIG_H_