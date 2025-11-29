#include <kmpkg/base/checks.h>
#include <kmpkg/base/files.h>

#include <kmpkg/buildenvironment.h>
#include <kmpkg/commands.create.h>
#include <kmpkg/commands.version.h>
#include <kmpkg/kmpkgcmdarguments.h>
#include <kmpkg/kmpkgpaths.h>

#include <algorithm>

using namespace kmpkg;

namespace
{
    std::string remove_trailing_url_slashes(std::string argument)
    {
        argument.erase(std::find_if_not(argument.rbegin(), argument.rend(), [](char c) { return c == '/'; }).base(),
                       argument.end());
        return argument;
    }
}

namespace kmpkg
{
    constexpr CommandMetadata CommandCreateMetadata{
        "create",
        msgHelpCreateCommand,
        {
            msgCmdCreateExample1,
            msgCmdCreateExample2,
            msgCmdCreateExample3,
            "kmpkg create zlib2 http://zlib.net/zlib1211.zip zlib1211-2.zip",
        },
        Undocumented,
        AutocompletePriority::Public,
        2,
        3,
        {},
        nullptr,
    };

    int command_create(const KmpkgCmdArguments& args, const KmpkgPaths& paths)
    {
        auto parsed = args.parse_arguments(CommandCreateMetadata);
        const std::string& port_name = parsed.command_arguments[0];
        std::string url = remove_trailing_url_slashes(parsed.command_arguments[1]);

        std::vector<CMakeVariable> cmake_args{
            {"CMD", "CREATE"},
            {"PORT", port_name},
            {"PORT_PATH", (paths.builtin_ports_directory() / port_name).generic_u8string()},
            {"URL", std::move(url)},
            {"KMPKG_BASE_VERSION", KMPKG_BASE_VERSION_AS_STRING},
        };

        if (parsed.command_arguments.size() >= 3)
        {
            std::string& zip_file_name = parsed.command_arguments[2];
            Checks::msg_check_exit(KMPKG_LINE_INFO,
                                   !has_invalid_chars_for_filesystem(zip_file_name),
                                   msgInvalidFilename,
                                   msg::value = FILESYSTEM_INVALID_CHARACTERS,
                                   msg::path = zip_file_name);
            cmake_args.emplace_back("FILENAME", zip_file_name);
        }

        ProcessLaunchSettings settings;
        settings.environment = get_clean_environment();
        return static_cast<int>(cmd_execute(make_cmake_cmd(paths, paths.ports_cmake, std::move(cmake_args)), settings)
                                    .value_or_exit(KMPKG_LINE_INFO));
    }

    void command_create_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths)
    {
        Checks::exit_with_code(KMPKG_LINE_INFO, command_create(args, paths));
    }
}
