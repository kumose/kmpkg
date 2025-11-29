#pragma once

#include <kmpkg/base/fwd/files.h>
#include <kmpkg/base/fwd/optional.h>
#include <kmpkg/base/fwd/stringview.h>

#include <kmpkg/fwd/portfileprovider.h>
#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

#include <string>

namespace kmpkg
{
    void perform_find_port_and_exit(const KmpkgPaths& paths,
                                    bool full_description,
                                    bool enable_json,
                                    Optional<StringView> filter,
                                    const OverlayPortPaths& overlay_ports);
    extern const CommandMetadata CommandFindMetadata;
    void command_find_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
}
