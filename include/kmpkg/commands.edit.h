#pragma once

#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    extern const CommandMetadata CommandEditMetadata;
    void command_edit_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}
