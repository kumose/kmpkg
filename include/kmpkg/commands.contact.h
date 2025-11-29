#pragma once

#include <kmpkg/base/fwd/files.h>

#include <kmpkg/fwd/kmpkgcmdarguments.h>

namespace kmpkg
{
    extern const CommandMetadata CommandContactMetadata;
    void command_contact_and_exit(const KmpkgCmdArguments& args, const Filesystem& fs);
}
