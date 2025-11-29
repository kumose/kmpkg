#pragma once

#include <kmpkg/base/fwd/files.h>
#include <kmpkg/base/fwd/messages.h>

#include <kmpkg/fwd/build.h>
#include <kmpkg/fwd/dependencies.h>
#include <kmpkg/fwd/packagespec.h>
#include <kmpkg/fwd/kmpkgpaths.h>

#include <stddef.h>

namespace kmpkg
{
    size_t perform_post_build_lint_checks(const InstallPlanAction& action,
                                          const KmpkgPaths& paths,
                                          const PreBuildInfo& pre_build_info,
                                          const BuildInfo& build_info,
                                          MessageSink& msg_sink);
}
