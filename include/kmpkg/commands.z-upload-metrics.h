#pragma once

#include <kmpkg/base/fwd/files.h>

#include <kmpkg/fwd/kmpkgcmdarguments.h>

namespace kmpkg
{
    extern const CommandMetadata CommandZUploadMetricsMetadata;
    void command_z_upload_metrics_and_exit(const KmpkgCmdArguments& args, const Filesystem& fs);
}
