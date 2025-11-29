#pragma once

#include <kmpkg/fwd/triplet.h>
#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    extern const CommandMetadata CommandTestFeaturesMetadata;
    void command_test_features_and_exit(const KmpkgCmdArguments& args,
                                        const KmpkgPaths& paths,
                                        Triplet default_triplet,
                                        Triplet host_triplet);
}
