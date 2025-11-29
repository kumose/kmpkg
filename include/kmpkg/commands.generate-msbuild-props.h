#pragma once

#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    extern const CommandMetadata CommandGenerateMsbuildPropsMetadata;
    void command_generate_msbuild_props_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}
