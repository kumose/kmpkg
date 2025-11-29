#include <kmpkg/base/contractual-constants.h>
#include <kmpkg/base/util.h>

#include <kmpkg/commands.find.h>
#include <kmpkg/commands.search.h>
#include <kmpkg/kmpkgcmdarguments.h>
#include <kmpkg/kmpkgpaths.h>

using namespace kmpkg;

namespace
{
    constexpr CommandSwitch SearchSwitches[] = {
        {SwitchXFullDesc, msgHelpTextOptFullDesc},
        {SwitchXJson, msgJsonSwitch},
    };
} // unnamed namespace

namespace kmpkg
{
    constexpr CommandMetadata CommandSearchMetadata = {
        "search",
        msgHelpSearchCommand,
        {msgCmdSearchExample1, "kmpkg search png"},
        "https://pub.kumose.cc/kmdo/commands/kmpkg/search",
        AutocompletePriority::Public,
        0,
        1,
        {SearchSwitches, {}},
        nullptr,
    };

    void command_search_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths)
    {
        msg::default_output_stream = OutputStream::StdErr;
        const ParsedArguments options = args.parse_arguments(CommandSearchMetadata);
        const bool full_description = Util::Sets::contains(options.switches, SwitchXFullDesc);
        Optional<StringView> filter;
        if (!options.command_arguments.empty())
        {
            filter = StringView{options.command_arguments[0]};
        }

        perform_find_port_and_exit(
            paths, full_description, Util::Sets::contains(options.switches, SwitchXJson), filter, paths.overlay_ports);
    }
} // namespace kmpkg
