#include <kmpkg/base/checks.h>

#include <kmpkg/commands.deactivate.h>
#include <kmpkg/configure-environment.h>
#include <kmpkg/kmpkgcmdarguments.h>

namespace kmpkg
{
    constexpr CommandMetadata CommandDeactivateMetadata{
        "deactivate",
        msgCmdDeactivateSynopsis,
        {"kmpkg deactivate"},
        Undocumented,
        AutocompletePriority::Public,
        0,
        0,
        {},
        nullptr,
    };

    void command_deactivate_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths)
    {
        (void)args.parse_arguments(CommandDeactivateMetadata);
        const std::string deactivate = "deactivate";
        Checks::exit_with_code(KMPKG_LINE_INFO,
                               run_configure_environment_command(paths, View<std::string>{&deactivate, 1}));
    }
}
