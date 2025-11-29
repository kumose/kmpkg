#pragma once

namespace kmpkg
{
    struct Configuration;
    struct ConfigurationAndSource;
    struct RegistryConfig;
    struct ManifestConfiguration;

    enum class ConfigurationSource
    {
        None,
        KmpkgConfigurationFile,
        ManifestFileKmpkgConfiguration,
        ManifestFileConfiguration
    };
}
