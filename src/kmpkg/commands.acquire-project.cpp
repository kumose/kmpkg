#include <kmpkg/base/checks.h>

#include <kmpkg/commands.acquire-project.h>
#include <kmpkg/configure-environment.h>
#include <kmpkg/kmpkgcmdarguments.h>

namespace kmpkg
{
    constexpr CommandMetadata CommandAcquireProjectMetadata{
        "acquire_project",
        msgCmdAcquireProjectSynopsis,
        {"kmpkg acquire-project"},
        Undocumented,
        AutocompletePriority::Public,
        0,
        0,
        {CommonAcquireArtifactSwitches},
        nullptr,
    };

    void command_acquire_project_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths)
    {
        auto parsed = args.parse_arguments(CommandAcquireProjectMetadata);
        std::vector<std::string> ecmascript_args;
        ecmascript_args.emplace_back("acquire-project");
        forward_common_artifacts_arguments(ecmascript_args, parsed);
        Checks::exit_with_code(KMPKG_LINE_INFO, run_configure_environment_command(paths, ecmascript_args));
    }
} // namespace kmpkg
