#include <kmpkg/base/checks.h>
#include <kmpkg/base/contractual-constants.h>
#include <kmpkg/base/stringview.h>
#include <kmpkg/base/util.h>

#include <kmpkg/commands.regenerate.h>
#include <kmpkg/configure-environment.h>
#include <kmpkg/kmpkgcmdarguments.h>

#include <string>
#include <vector>

using namespace kmpkg;

namespace
{
    constexpr CommandSwitch command_switches[] = {
        {SwitchForce, msgCmdRegenerateOptForce},
        {SwitchDryRun, msgCmdRegenerateOptDryRun},
        {SwitchNormalize, msgCmdRegenerateOptNormalize},
    };
} // unnamed namespace

namespace kmpkg
{
    constexpr CommandMetadata CommandRegenerateMetadata{
        "x-regenerate",
        msgRegeneratesArtifactRegistry,
        {"kmpkg x-regenerate"},
        Undocumented,
        AutocompletePriority::Public,
        1,
        1,
        {command_switches},
        nullptr,
    };

    void command_regenerate_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths)
    {
        std::vector<std::string> forwarded_args;
        forwarded_args.emplace_back("regenerate");
        const auto parsed = args.parse_arguments(CommandRegenerateMetadata);
        forwarded_args.push_back(parsed.command_arguments[0]);

        if (Util::Sets::contains(parsed.switches, SwitchForce))
        {
            forwarded_args.emplace_back("--force");
        }

        if (Util::Sets::contains(parsed.switches, SwitchDryRun))
        {
            forwarded_args.emplace_back("--what-if");
        }

        if (Util::Sets::contains(parsed.switches, SwitchNormalize))
        {
            forwarded_args.emplace_back("--normalize");
        }

        Checks::exit_with_code(KMPKG_LINE_INFO, run_configure_environment_command(paths, forwarded_args));
    }
} // namespace kmpkg
