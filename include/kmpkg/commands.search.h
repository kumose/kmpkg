#pragma once

#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    extern const CommandMetadata CommandSearchMetadata;

    void command_search_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}
