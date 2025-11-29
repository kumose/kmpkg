#pragma once

#include <kmpkg/base/fwd/files.h>

#include <kmpkg/fwd/kmpkgcmdarguments.h>

namespace kmpkg
{
    extern const CommandMetadata CommandDownloadMetadata;
    void command_download_and_exit(const KmpkgCmdArguments& args, const Filesystem& fs);
}
