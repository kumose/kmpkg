#include <kmpkg/base/checks.h>
#include <kmpkg/base/contractual-constants.h>

#include <kmpkg/commands.activate.h>
#include <kmpkg/configure-environment.h>
#include <kmpkg/kmpkgcmdarguments.h>

using namespace kmpkg;

namespace
{
    constexpr CommandSetting ActivateOptions[] = {
        {SwitchMSBuildProps, msgArtifactsOptionMSBuildProps},
        {SwitchJson, msgArtifactsOptionJson},
    };
} // unnamed namespace

namespace kmpkg
{
    constexpr CommandMetadata CommandActivateMetadata{
        "activate",
        msgCmdActivateSynopsis,
        {"kmpkg-shell activate", "kmpkg activate --msbuild-props file.targets"},
        Undocumented,
        AutocompletePriority::Public,
        0,
        0,
        {CommonAcquireArtifactSwitches, ActivateOptions},
        nullptr,
    };

    void command_activate_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths)
    {
        auto parsed = args.parse_arguments(CommandActivateMetadata);
        std::vector<std::string> ecmascript_args;
        ecmascript_args.emplace_back("activate");
        forward_common_artifacts_arguments(ecmascript_args, parsed);
        Checks::exit_with_code(KMPKG_LINE_INFO, run_configure_environment_command(paths, ecmascript_args));
    }
} // namespace kmpkg
