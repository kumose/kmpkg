#pragma once

#include <kmpkg/base/fwd/messages.h>

#include <kmpkg/fwd/kmpkgpaths.h>

#include <kmpkg/base/files.h>

#include <string>
#include <vector>

namespace kmpkg
{
    struct ToolsetsInformation
    {
        std::vector<Toolset> toolsets;

#if defined(_WIN32)
        std::vector<Path> paths_examined;
        std::vector<Toolset> excluded_toolsets;
        LocalizedString get_localized_debug_info() const;
#endif
    };
}

#if defined(_WIN32)

namespace kmpkg::VisualStudio
{
    std::vector<std::string> get_visual_studio_instances(const ReadOnlyFilesystem& fs);

    ToolsetsInformation find_toolset_instances_preferred_first(const ReadOnlyFilesystem& fs);
}

#endif
