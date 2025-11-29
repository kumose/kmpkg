#pragma once

#include <kmpkg/base/fwd/files.h>

#include <kmpkg/fwd/kmpkgcmdarguments.h>

namespace kmpkg
{
    extern const CommandMetadata CommandFormatFeatureBaselineMetadata;
    void command_format_feature_baseline_and_exit(const KmpkgCmdArguments& args, const Filesystem& fs);
}
