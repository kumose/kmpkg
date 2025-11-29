#include <kmpkg/base/checks.h>

#include <kmpkg/commands.z-ce.h>
#include <kmpkg/configure-environment.h>
#include <kmpkg/kmpkgcmdarguments.h>

namespace kmpkg
{
    constexpr CommandMetadata CommandZCEMetadata{
        "z-ce",
        {/*intentionally undocumented*/},
        {},
        Undocumented,
        AutocompletePriority::Never,
        0,
        SIZE_MAX,
        {},
        nullptr,
    };

    void command_z_ce_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths)
    {
        Checks::exit_with_code(KMPKG_LINE_INFO,
                               run_configure_environment_command(paths, args.get_forwardable_arguments()));
    }
}
