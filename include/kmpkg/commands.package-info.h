#pragma once

#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    extern const CommandMetadata CommandPackageInfoMetadata;

    void command_package_info_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}
