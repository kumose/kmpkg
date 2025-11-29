#pragma once

#include <kmpkg/base/fwd/stringview.h>

#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

#include <kmpkg/base/diagnostics.h>
#include <kmpkg/base/optional.h>

#include <kmpkg/versions.h>

#include <string>
#include <vector>

namespace kmpkg
{
    struct UpdatedPort
    {
        std::string port_name;
        VersionDiff version_diff;
    };

    struct PortsDiff
    {
        std::vector<VersionSpec> added_ports;
        std::vector<UpdatedPort> updated_ports;
        std::vector<std::string> removed_ports;
    };

    Optional<PortsDiff> find_portsdiff(DiagnosticContext& context,
                                       const KmpkgPaths& paths,
                                       StringView git_commit_id_for_previous_snapshot,
                                       StringView git_commit_id_for_current_snapshot);

    extern const CommandMetadata CommandPortsdiffMetadata;
    void command_portsdiff_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}
