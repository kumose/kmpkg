#include <kmpkg/base/checks.h>

#include <kmpkg/commands.contact.h>
#include <kmpkg/kmpkgcmdarguments.h>

namespace kmpkg
{
    constexpr CommandMetadata CommandContactMetadata{
        "contact",
        msgHelpContactCommand,
        {"kmpkg contact"},
        Undocumented,
        AutocompletePriority::Internal,
        0,
        0,
        {},
        nullptr,
    };

    void command_contact_and_exit(const KmpkgCmdArguments& args, const Filesystem&)
    {
        (void)args.parse_arguments(CommandContactMetadata);
        msg::println(msgEmailKmpkgTeam, msg::url = "kumo-pub@outlook.com");
        Checks::exit_success(KMPKG_LINE_INFO);
    }
}
