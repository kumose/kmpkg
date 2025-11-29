#pragma once

#include <kmpkg/base/fwd/message_sinks.h>

#include <kmpkg/fwd/configuration.h>
#include <kmpkg/fwd/kmpkgcmdarguments.h>

#include <kmpkg/base/json.h>
#include <kmpkg/base/optional.h>
#include <kmpkg/base/path.h>

#include <kmpkg/registries.h>

#include <memory>
#include <string>
#include <vector>

namespace kmpkg
{
    /// <summary>
    /// A registry package pattern (e.g.: boost*) and the in-file location where it was declared.
    /// </summary>
    struct PackagePatternDeclaration
    {
        std::string pattern;
        std::string location;
    };

    struct RegistryConfig
    {
        // Missing kind means "null"
        Optional<std::string> kind;
        Optional<std::string> baseline;
        Optional<std::string> location;
        Optional<std::string> name;
        Optional<Path> path;
        Optional<std::string> reference;
        Optional<std::string> repo;
        Optional<std::vector<std::string>> packages;
        Optional<std::vector<PackagePatternDeclaration>> package_declarations;

        Json::Value serialize() const;

        ExpectedL<Optional<std::string>> get_latest_baseline(const KmpkgPaths& paths) const;
        StringView pretty_location() const;
    };

    struct Configuration
    {
        Optional<RegistryConfig> default_reg;
        std::vector<RegistryConfig> registries;
        Json::Object ce_metadata;
        Json::Object extra_info;
        std::vector<std::string> overlay_ports;
        std::vector<std::string> overlay_triplets;

        Json::Object serialize() const;
        void validate_as_active() const;

        std::unique_ptr<RegistrySet> instantiate_registry_set(const KmpkgPaths& paths, const Path& config_dir) const;

        bool requests_ce() const;

        static View<StringView> known_fields();
    };

    StringLiteral configuration_source_file_name(ConfigurationSource source);
    StringLiteral configuration_source_field(ConfigurationSource source);

    struct ConfigurationAndSource
    {
        Configuration config;
        Path directory;
        ConfigurationSource source = ConfigurationSource::None;

        std::unique_ptr<RegistrySet> instantiate_registry_set(const KmpkgPaths& paths) const
        {
            return config.instantiate_registry_set(paths, directory);
        }
    };

    struct ManifestConfiguration
    {
        Optional<std::string> builtin_baseline;
        Optional<Configuration> config;
        ConfigurationSource config_source = ConfigurationSource::None;
    };

    extern const Json::IDeserializer<Configuration>& configuration_deserializer;
    // Parse configuration from a file containing a valid kmpkg-configuration.json file
    Optional<Configuration> parse_configuration(StringView contents,
                                                StringView origin,
                                                kmpkg::MessageSink& messageSink);
    // Parse a configuration JSON object
    Optional<Configuration> parse_configuration(const Json::Object& object,
                                                StringView origin,
                                                kmpkg::MessageSink& messageSink);

    std::vector<std::string> find_unknown_fields(const Configuration& config);

    // Exposed for testing
    bool is_package_pattern(StringView sv);
}
