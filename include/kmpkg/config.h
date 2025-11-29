#pragma once

#include <string>
#include <string_view>
#include <exception>
#include <optional>
#include <kmpkg/base/toml.hpp>

namespace kmpkg  {
    

class KmpkgConfig
{
public:
    static constexpr std::string_view kConfigPath = "~/.config/kmpkg.toml";


static std::optional<std::string> get_root_from_config()
{
    // Construct config file path: ~/.config/kmpkg.toml
    const char* home_dir = std::getenv("HOME");
    if (!home_dir || home_dir[0] == '\0') {
        return std::nullopt;
    }

    std::string config_path(home_dir);
    config_path += "/.config/kmpkg.toml";

    // Check if config file exists (using standard C I/O)
    FILE* file = std::fopen(config_path.c_str(), "r");
    if (!file) {
        return std::nullopt;
    }
    std::fclose(file);

    // Parse TOML file
    toml::table config;
    try {
        config = toml::parse_file(config_path);
    } catch (const toml::parse_error&) {
        return std::nullopt;
    }

    // Extract "root" field (must be string type)
    auto root_node = config["root"];
    if (!root_node.is_string()) {
        return std::nullopt;
    }

    // Return parsed root path as optional
    return root_node.value<std::string>().value();
}


static std::optional<std::string> get_current_repo_from_config()
{
    // Construct config file path: ~/.config/kmpkg.toml
    const char* home_dir = std::getenv("HOME");
    if (!home_dir || home_dir[0] == '\0') {
        return std::nullopt;
    }

    std::string config_path(home_dir);
    config_path += "/.config/kmpkg.toml";

    // Check if config file exists (C-style I/O)
    FILE* file = std::fopen(config_path.c_str(), "r");
    if (!file) {
        return std::nullopt;
    }
    std::fclose(file);

    // Parse TOML file (catch parse errors)
    toml::table config;
    try {
        config = toml::parse_file(config_path);
    } catch (const toml::parse_error&) {
        return std::nullopt;
    }

    // Get [current] table first
    auto current_table = config["current"].as_table();
    if (!current_table) {
        return std::nullopt;
    }

    // Extract "repo" field from [current] (must be string)
    auto repo_node = (*current_table)["repo"];
    if (!repo_node.is_string()) {
        return std::nullopt;
    }

    // Return repo path as optional
    return repo_node.value<std::string>().value();
}

static std::optional<std::string> get_current_remote_from_config()
{
    // Construct config file path: ~/.config/kmpkg.toml
    const char* home_dir = std::getenv("HOME");
    if (!home_dir || home_dir[0] == '\0') {
        return std::nullopt;
    }

    std::string config_path(home_dir);
    config_path += "/.config/kmpkg.toml";

    // Check if config file exists (C-style I/O)
    FILE* file = std::fopen(config_path.c_str(), "r");
    if (!file) {
        return std::nullopt;
    }
    std::fclose(file);

    // Parse TOML file (catch parse errors)
    toml::table config;
    try {
        config = toml::parse_file(config_path);
    } catch (const toml::parse_error&) {
        return std::nullopt;
    }

    // Get [current] table first
    auto current_table = config["current"].as_table();
    if (!current_table) {
        return std::nullopt;
    }

    // Extract "repo" field from [current] (must be string)
    auto repo_node = (*current_table)["remote"];
    if (!repo_node.is_string()) {
        return std::nullopt;
    }

    // Return repo path as optional
    return repo_node.value<std::string>().value();
}
};

} // namespace kmpkg 
