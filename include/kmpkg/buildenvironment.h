#pragma once

#include <kmpkg/fwd/kmpkgpaths.h>

#include <kmpkg/base/system.process.h>

#include <vector>

namespace kmpkg
{
    Command make_cmake_cmd(const KmpkgPaths& paths,
                           const Path& cmake_script,
                           std::vector<CMakeVariable>&& pass_variables);
}
