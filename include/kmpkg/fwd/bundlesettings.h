#pragma once

namespace kmpkg
{
    enum class DeploymentKind
    {
        Git,         // kmpkg was deployed with "git clone" or similar
        OneLiner,    // kmpkg was deployed with the "one liner" installer
        VisualStudio // kmpkg was deployed by the Visual Studio installer
    };

    struct BundleSettings;
}
