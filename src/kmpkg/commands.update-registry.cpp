#include <kmpkg/base/checks.h>
#include <kmpkg/base/contractual-constants.h>
#include <kmpkg/base/util.h>

#include <kmpkg/commands.update-registry.h>
#include <kmpkg/configure-environment.h>
#include <kmpkg/kmpkgcmdarguments.h>

using namespace kmpkg;

namespace
{
    constexpr CommandSwitch UpdateRegistrySwitches[] = {
        {SwitchAll, msgCmdUpdateRegistryAll},
    };
} // unnamed namespace

namespace kmpkg
{
    constexpr CommandMetadata CommandUpdateRegistryMetadata{
        "x-update-registry",
        msgCmdUpdateRegistrySynopsis,
        {
            "kmpkg x-update-registry <uri>",
            "kmpkg x-update-registry https://example.com",
            msgCmdUpdateRegistryExample3,
            "kmpkg x-update-registry microsoft",
        },
        Undocumented,
        AutocompletePriority::Public,
        0,
        SIZE_MAX,
        {UpdateRegistrySwitches},
        nullptr,
    };

    void command_update_registry_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths)
    {
        auto parsed = args.parse_arguments(CommandUpdateRegistryMetadata);
        const bool all = Util::Sets::contains(parsed.switches, SwitchAll);
        auto&& command_arguments = parsed.command_arguments;
        if (all)
        {
            if (command_arguments.empty())
            {
                command_arguments.emplace_back("update");
                command_arguments.emplace_back("--all");
            }
            else
            {
                Checks::msg_exit_with_error(KMPKG_LINE_INFO, msgCmdUpdateRegistryAllExcludesTargets);
            }
        }
        else
        {
            if (command_arguments.empty())
            {
                Checks::msg_exit_with_error(KMPKG_LINE_INFO, msgCmdUpdateRegistryAllOrTargets);
            }
            else
            {
                command_arguments.emplace(command_arguments.begin(), "update");
            }
        }

        Checks::exit_with_code(KMPKG_LINE_INFO, run_configure_environment_command(paths, command_arguments));
    }
} // namespace kmpkg
