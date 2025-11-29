#if defined(_WIN32)
#pragma once

#include <kmpkg/base/fwd/files.h>

#include <kmpkg/fwd/kmpkgcmdarguments.h>

namespace kmpkg
{
    extern const CommandMetadata CommandZApplocalMetadata;
    void command_z_applocal_and_exit(const KmpkgCmdArguments& args, const Filesystem& fs);
}
#endif
