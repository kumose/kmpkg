#include <kmpkg/base/checks.h>
#include <kmpkg/base/files.h>

#include <kmpkg/commands.ci-clean.h>
#include <kmpkg/installedpaths.h>
#include <kmpkg/kmpkgcmdarguments.h>
#include <kmpkg/kmpkgpaths.h>

using namespace kmpkg;

namespace
{
    void clear_directory(const Filesystem& fs, const Path& target)
    {
        if (fs.is_directory(target))
        {
            msg::println(msgClearingContents, msg::path = target);
            fs.remove_all_inside(target, KMPKG_LINE_INFO);
        }
        else
        {
            msg::println(msgSkipClearingInvalidDir, msg::path = target);
        }
    }
}

namespace kmpkg
{
    constexpr CommandMetadata CommandCiCleanMetadata{
        "x-ci-clean",
        msgCmdCiCleanSynopsis,
        {"kmpkg x-ci-clean"},
        Undocumented,
        AutocompletePriority::Internal,
        0,
        0,
        {},
        nullptr,
    };

    void command_ci_clean_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths)
    {
        (void)args.parse_arguments(CommandCiCleanMetadata);
        auto& fs = paths.get_filesystem();
        clear_directory(fs, paths.buildtrees());
        clear_directory(fs, paths.installed().root());
        clear_directory(fs, paths.packages());
        Checks::exit_success(KMPKG_LINE_INFO);
    }
}
