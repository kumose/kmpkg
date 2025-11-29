#include <kmpkg/commands.build-external.h>
#include <kmpkg/commands.build.h>
#include <kmpkg/input.h>
#include <kmpkg/portfileprovider.h>
#include <kmpkg/registries.h>
#include <kmpkg/kmpkgcmdarguments.h>
#include <kmpkg/kmpkgpaths.h>

namespace kmpkg
{
    constexpr CommandMetadata CommandBuildExternalMetadata{
        "build-external",
        msgCmdBuildExternalSynopsis,
        {msgCmdBuildExternalExample1, msgCmdBuildExternalExample2},
        Undocumented,
        AutocompletePriority::Internal,
        2,
        2,
        {},
        nullptr,
    };

    void command_build_external_and_exit(const KmpkgCmdArguments& args,
                                         const KmpkgPaths& paths,
                                         Triplet default_triplet,
                                         Triplet host_triplet)
    {
        const ParsedArguments options = args.parse_arguments(CommandBuildExternalMetadata);

        static constexpr BuildPackageOptions build_options{
            BuildMissing::Yes,
            AllowDownloads::Yes,
            OnlyDownloads::No,
            CleanBuildtrees::Yes,
            CleanPackages::Yes,
            CleanDownloads::No,
            BackcompatFeatures::Allow,
        };

        const FullPackageSpec spec =
            check_and_get_full_package_spec(options.command_arguments[0], default_triplet, paths.get_triplet_db())
                .value_or_exit(KMPKG_LINE_INFO);

        auto overlays = paths.overlay_ports;
        overlays.overlay_ports.insert(overlays.overlay_ports.begin(), options.command_arguments[1]);

        auto& fs = paths.get_filesystem();
        auto registry_set = paths.make_registry_set();
        PathsPortFileProvider provider(*registry_set, make_overlay_provider(fs, overlays));
        command_build_and_exit_ex(args, paths, host_triplet, build_options, spec, provider, null_build_logs_recorder);
    }
}
