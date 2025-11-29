#pragma once

#include <kmpkg/base/fwd/files.h>

#include <kmpkg/fwd/kmpkgcmdarguments.h>

namespace kmpkg
{
    extern const CommandMetadata CommandCheckToolsShaMetadata;
    void command_check_tools_sha_and_exit(const KmpkgCmdArguments& args, const Filesystem& paths);
}
