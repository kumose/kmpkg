#pragma once

#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    extern const CommandMetadata CommandZChangelogMetadata;
    void command_z_changelog_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}
