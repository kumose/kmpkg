#include <kmpkg/base/checks.h>
#include <kmpkg/base/contractual-constants.h>
#include <kmpkg/base/util.h>

#include <kmpkg/commands.acquire.h>
#include <kmpkg/configure-environment.h>
#include <kmpkg/kmpkgcmdarguments.h>

using namespace kmpkg;

namespace
{
    constexpr CommandMultiSetting AcquireMultiOptions[] = {
        {SwitchVersion, msgArtifactsOptionVersion},
    };
} // unnamed namespace

namespace kmpkg
{
    constexpr CommandMetadata CommandAcquireMetadata{
        "acquire",
        msgCmdAcquireSynopsis,
        {msgCmdAcquireExample1, "kmpkg acquire cmake"},
        Undocumented,
        AutocompletePriority::Public,
        1,
        SIZE_MAX,
        {CommonAcquireArtifactSwitches, {}, AcquireMultiOptions},
        nullptr,
    };

    void command_acquire_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths)
    {
        auto parsed = args.parse_arguments(CommandAcquireMetadata);
        std::vector<std::string> ecmascript_args;
        ecmascript_args.emplace_back("acquire");
        forward_common_artifacts_arguments(ecmascript_args, parsed);
        auto maybe_versions = Util::lookup_value(parsed.multisettings, SwitchVersion);
        if (auto versions = maybe_versions.get())
        {
            if (versions->size() != parsed.command_arguments.size())
            {
                Checks::msg_exit_with_error(KMPKG_LINE_INFO, msgArtifactsOptionVersionMismatch);
            }

            for (auto&& version : *versions)
            {
                ecmascript_args.push_back("--version");
                ecmascript_args.push_back(version);
            }
        }

        ecmascript_args.insert(ecmascript_args.end(),
                               std::make_move_iterator(parsed.command_arguments.begin()),
                               std::make_move_iterator(parsed.command_arguments.end()));

        Checks::exit_with_code(KMPKG_LINE_INFO, run_configure_environment_command(paths, ecmascript_args));
    }
}
