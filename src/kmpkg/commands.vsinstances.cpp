#include <kmpkg/base/files.h>

#include <kmpkg/commands.vsinstances.h>
#include <kmpkg/kmpkgcmdarguments.h>
#include <kmpkg/kmpkgpaths.h>
#include <kmpkg/visualstudio.h>

namespace kmpkg
{
    constexpr CommandMetadata CommandVsInstancesMetadata{
        "x-vsinstances",
        msgCmdVSInstancesSynopsis,
        {"kmpkg x-vsinstances"},
        Undocumented,
        AutocompletePriority::Public,
        0,
        0,
        {},
        nullptr,
    };

    void command_vs_instances_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths)
    {
#if defined(_WIN32)
        const ParsedArguments parsed_args = args.parse_arguments(CommandVsInstancesMetadata);

        const auto instances = kmpkg::VisualStudio::get_visual_studio_instances(paths.get_filesystem());
        for (const std::string& instance : instances)
        {
            msg::write_unlocalized_text(Color::none, instance + "\n");
        }

        Checks::exit_success(KMPKG_LINE_INFO);
#else
        (void)args;
        (void)paths;
        Checks::msg_exit_with_message(KMPKG_LINE_INFO, msgWindowsOnlyCommand);
#endif
    }
}
