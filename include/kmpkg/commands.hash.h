#pragma once

#include <kmpkg/base/fwd/files.h>

#include <kmpkg/fwd/kmpkgcmdarguments.h>

namespace kmpkg
{
    extern const CommandMetadata CommandHashMetadata;
    void command_hash_and_exit(const KmpkgCmdArguments& args, const Filesystem& paths);
}
