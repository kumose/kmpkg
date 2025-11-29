#pragma once

#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    extern const CommandMetadata CommandFormatManifestMetadata;
    void command_format_manifest_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}
