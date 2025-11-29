#pragma once

#include <kmpkg/base/fwd/files.h>

#include <kmpkg/fwd/kmpkgcmdarguments.h>

namespace kmpkg
{
    extern const CommandMetadata CommandZPreregisterTelemetryMetadata;
    void command_z_preregister_telemetry_and_exit(const KmpkgCmdArguments& args, const Filesystem& fs);
}
