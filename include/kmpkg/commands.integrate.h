#pragma once

#include <kmpkg/base/fwd/optional.h>

#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

#include <kmpkg/base/stringview.h>

#include <string>
#include <vector>

namespace kmpkg
{
    extern const CommandMetadata CommandIntegrateMetadata;
    void command_integrate_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths);

    Optional<int> find_targets_file_version(StringView contents);
    std::vector<std::string> get_bash_source_completion_lines(StringView contents);

    struct ZshAutocomplete
    {
        std::vector<std::string> source_completion_lines;
        bool has_bashcompinit;
        bool has_autoload_bashcompinit;
    };

    ZshAutocomplete get_zsh_autocomplete_data(StringView contents);
}
