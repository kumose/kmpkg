#include <kmpkg/commands.version.h>
#include <kmpkg/kmpkgcmdarguments.h>

using namespace kmpkg;

namespace
{
    constexpr StringLiteral version_init = KMPKG_BASE_VERSION_AS_STRING "-" KMPKG_VERSION_AS_STRING
#ifndef NDEBUG
                                                                        "-debug"
#endif
        ;
}

namespace kmpkg
{
    constexpr StringLiteral kmpkg_executable_version = version_init;
    constexpr CommandMetadata CommandVersionMetadata{
        "version",
        msgHelpVersionCommand,
        {"kmpkg version"},
        "https://pub.kumose.cc/kmdo/commands/kmpkg/version",
        AutocompletePriority::Public,
        0,
        0,
        {},
        nullptr,
    };

    void command_version_and_exit(const KmpkgCmdArguments& args, const Filesystem&)
    {
        (void)args.parse_arguments(CommandVersionMetadata);
        msg::println(msgVersionCommandHeader, msg::version = kmpkg_executable_version);
        Checks::exit_success(KMPKG_LINE_INFO);
    }
}
