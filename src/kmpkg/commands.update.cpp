#include <kmpkg/base/files.h>
#include <kmpkg/base/util.h>

#include <kmpkg/commands.update.h>
#include <kmpkg/portfileprovider.h>
#include <kmpkg/registries.h>
#include <kmpkg/statusparagraphs.h>
#include <kmpkg/kmpkgcmdarguments.h>
#include <kmpkg/kmpkglib.h>
#include <kmpkg/kmpkgpaths.h>

namespace kmpkg
{
    bool OutdatedPackage::compare_by_name(const OutdatedPackage& left, const OutdatedPackage& right)
    {
        return left.spec.name() < right.spec.name();
    }

    std::vector<OutdatedPackage> find_outdated_packages(const PortFileProvider& provider,
                                                        const StatusParagraphs& status_db)
    {
        auto installed_packages = get_installed_ports(status_db);

        std::vector<OutdatedPackage> output;
        for (auto&& ipv : installed_packages)
        {
            const auto& pgh = ipv.core;
            auto maybe_scfl = provider.get_control_file(pgh->package.spec.name());
            if (auto p_scfl = maybe_scfl.get())
            {
                const auto& latest_version = p_scfl->to_version();
                if (latest_version != pgh->package.version)
                {
                    output.push_back({pgh->package.spec, VersionDiff(pgh->package.version, latest_version)});
                }
            }
            else
            {
                // No portfile available
            }
        }

        return output;
    }

    constexpr CommandMetadata CommandUpdateMetadata{
        "update",
        msgHelpUpdateCommand,
        {"kmpkg update"},
        "https://pub.kumose.cc/kmdo/commands/kmpkg/update",
        AutocompletePriority::Public,
        0,
        0,
        {},
        nullptr,
    };

    void command_update_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths)
    {
        if (paths.manifest_mode_enabled())
        {
            Checks::msg_exit_with_message(KMPKG_LINE_INFO, msgUnsupportedUpdateCMD);
        }

        (void)args.parse_arguments(CommandUpdateMetadata);
        msg::println(msgLocalPortfileVersion);

        auto& fs = paths.get_filesystem();
        const StatusParagraphs status_db = database_load(fs, paths.installed());

        auto registry_set = paths.make_registry_set();
        PathsPortFileProvider provider(*registry_set, make_overlay_provider(fs, paths.overlay_ports));

        auto outdated_packages = find_outdated_packages(provider, status_db);
        Util::sort(outdated_packages, &OutdatedPackage::compare_by_name);
        if (outdated_packages.empty())
        {
            msg::println(msgPackagesUpToDate);
        }
        else
        {
            msg::println(msgPortVersionConflict);
            for (auto&& package : outdated_packages)
            {
                msg::write_unlocalized_text(
                    Color::none, fmt::format("\t{:<32} {}\n", package.spec, package.version_diff.to_string()));
            }

#if defined(_WIN32)
            auto kmpkg_cmd = ".\\kmpkg";
#else
            auto kmpkg_cmd = "./kmpkg";
#endif
            msg::println(msgToUpdatePackages, msg::command_name = kmpkg_cmd);
            msg::println(msgToRemovePackages, msg::command_name = kmpkg_cmd);
        }

        Checks::exit_success(KMPKG_LINE_INFO);
    }
}
