#include <kmpkg/base/checks.h>
#include <kmpkg/base/contractual-constants.h>
#include <kmpkg/base/util.h>

#include <kmpkg/commands.generate-msbuild-props.h>
#include <kmpkg/configure-environment.h>
#include <kmpkg/kmpkgcmdarguments.h>

using namespace kmpkg;

namespace
{
    constexpr CommandSetting GenerateMSBuildPropsOptions[] = {
        {SwitchMSBuildProps, msgArtifactsOptionMSBuildProps},
    };
} // unnamed namespace

namespace kmpkg
{
    constexpr CommandMetadata CommandGenerateMsbuildPropsMetadata{
        "generate-msbuild-props",
        msgCmdGenerateMSBuildPropsSynopsis,
        {msgCmdGenerateMSBuildPropsExample1, msgCmdGenerateMSBuildPropsExample2},
        Undocumented,
        AutocompletePriority::Internal,
        0,
        0,
        {CommonAcquireArtifactSwitches, GenerateMSBuildPropsOptions},
        nullptr,
    };

    void command_generate_msbuild_props_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths)
    {
        auto parsed = args.parse_arguments(CommandGenerateMsbuildPropsMetadata);
        std::vector<std::string> ecmascript_args;
        ecmascript_args.emplace_back("generate-msbuild-props");

        forward_common_artifacts_arguments(ecmascript_args, parsed);

        if (!Util::Maps::contains(parsed.settings, SwitchMSBuildProps))
        {
            Checks::msg_exit_with_error(KMPKG_LINE_INFO, msgOptionRequiresAValue, msg::option = SwitchMSBuildProps);
        }

        Checks::exit_with_code(KMPKG_LINE_INFO, run_configure_environment_command(paths, ecmascript_args));
    }
} // namespace kmpkg
