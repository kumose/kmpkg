#pragma once

#include <kmpkg/base/fwd/files.h>

#include <kmpkg/fwd/kmpkgcmdarguments.h>

namespace kmpkg
{
    extern const CommandMetadata CommandInitRegistryMetadata;
    void command_init_registry_and_exit(const KmpkgCmdArguments& args, const Filesystem& fs);
}
