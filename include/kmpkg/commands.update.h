#pragma once

#include <kmpkg/fwd/portfileprovider.h>
#include <kmpkg/fwd/statusparagraphs.h>
#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

#include <kmpkg/packagespec.h>
#include <kmpkg/versions.h>

#include <vector>

namespace kmpkg
{
    struct OutdatedPackage
    {
        static bool compare_by_name(const OutdatedPackage& left, const OutdatedPackage& right);

        PackageSpec spec;
        VersionDiff version_diff;
    };

    std::vector<OutdatedPackage> find_outdated_packages(const PortFileProvider& provider,
                                                        const StatusParagraphs& status_db);

    extern const CommandMetadata CommandUpdateMetadata;
    void command_update_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}
