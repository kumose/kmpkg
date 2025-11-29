#pragma once

#include <kmpkg/base/fwd/files.h>

#include <kmpkg/fwd/installedpaths.h>
#include <kmpkg/fwd/packagespec.h>
#include <kmpkg/fwd/statusparagraphs.h>
#include <kmpkg/fwd/triplet.h>
#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    enum class Purge : bool
    {
        NO = 0,
        YES
    };

    void remove_package(const Filesystem& fs,
                        const InstalledPaths& installed,
                        const PackageSpec& spec,
                        StatusParagraphs& status_db);

    extern const CommandMetadata CommandRemoveMetadata;
    void command_remove_and_exit(const KmpkgCmdArguments& args,
                                 const KmpkgPaths& paths,
                                 Triplet default_triplet,
                                 Triplet host_triplet);
}
