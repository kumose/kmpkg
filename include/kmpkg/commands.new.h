#pragma once

#include <kmpkg/base/fwd/expected.h>
#include <kmpkg/base/fwd/json.h>

#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

namespace kmpkg
{
    ExpectedL<Json::Object> build_prototype_manifest(const std::string* name,
                                                     const std::string* version,
                                                     bool option_application,
                                                     bool option_version_relaxed,
                                                     bool option_version_date,
                                                     bool option_version_string);

    extern const CommandMetadata CommandNewMetadata;
    void command_new_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}
