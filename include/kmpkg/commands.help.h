#pragma once

#include <kmpkg/base/fwd/files.h>

#include <kmpkg/fwd/triplet.h>
#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    extern const CommandMetadata CommandHelpMetadata;

    void command_help_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);

    void append_help_topic_valid_triplet(LocalizedString& result, const TripletDatabase& database);
}
