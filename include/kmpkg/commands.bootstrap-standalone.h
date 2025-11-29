#pragma once

#include <kmpkg/base/fwd/files.h>

#include <kmpkg/fwd/kmpkgcmdarguments.h>

namespace kmpkg
{
    extern const CommandMetadata CommandBootstrapStandaloneMetadata;
    void command_bootstrap_standalone_and_exit(const KmpkgCmdArguments& args, const Filesystem& fs);
}
