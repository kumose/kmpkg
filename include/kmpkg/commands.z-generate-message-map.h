#pragma once

#include <kmpkg/base/fwd/files.h>
#include <kmpkg/base/fwd/messages.h>

#include <kmpkg/fwd/kmpkgcmdarguments.h>

#include <kmpkg/base/stringview.h>

#include <vector>

namespace kmpkg
{
    struct FormatArgMismatches
    {
        std::vector<StringView> arguments_without_comment;
        std::vector<StringView> comments_without_argument;
    };

    std::vector<StringView> get_all_format_args(StringView fstring, LocalizedString& error);
    FormatArgMismatches get_format_arg_mismatches(StringView value, StringView comment, LocalizedString& error);

    extern const CommandMetadata CommandZGenerateDefaultMessageMapMetadata;
    void command_z_generate_default_message_map_and_exit(const KmpkgCmdArguments&, const Filesystem&);
}
