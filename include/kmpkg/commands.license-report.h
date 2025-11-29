#pragma once

#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    extern const CommandMetadata CommandLicenseReportMetadata;
    void command_license_report_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}
