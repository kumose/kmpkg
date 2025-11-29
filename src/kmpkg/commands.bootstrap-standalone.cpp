#include <kmpkg/base/downloads.h>
#include <kmpkg/base/files.h>
#include <kmpkg/base/optional.h>

#include <kmpkg/archives.h>
#include <kmpkg/commands.bootstrap-standalone.h>
#include <kmpkg/commands.version.h>
#include <kmpkg/tools.h>
#include <kmpkg/kmpkgcmdarguments.h>

#include <string>

namespace
{
    using namespace kmpkg;

    Optional<Path> download_kmpkg_standalone_bundle(DiagnosticContext& context,
                                                    const AssetCachingSettings& asset_cache_settings,
                                                    const Filesystem& fs,
                                                    const Path& download_root)
    {
#if defined(KMPKG_STANDALONE_BUNDLE_SHA)
        static constexpr StringLiteral tarball_name = "kmpkg-standalone-bundle-" KMPKG_BASE_VERSION_AS_STRING ".tar.gz";
        const auto bundle_tarball = download_root / tarball_name;
        context.statusln(msg::format(msgDownloadingKmpkgStandaloneBundle, msg::version = KMPKG_BASE_VERSION_AS_STRING));
        const auto bundle_uri =
            "https://github.com/kumose/kmpkg-tool/releases/download/" KMPKG_BASE_VERSION_AS_STRING
            "/kmpkg-standalone-bundle.tar.gz";
        if (!download_file_asset_cached(context,
                                        null_sink,
                                        asset_cache_settings,
                                        fs,
                                        bundle_uri,
                                        {},
                                        bundle_tarball,
                                        tarball_name,
                                        MACRO_TO_STRING(KMPKG_STANDALONE_BUNDLE_SHA)))
        {
            return nullopt;
        }
#else  // ^^^ KMPKG_STANDALONE_BUNDLE_SHA / !KMPKG_STANDALONE_BUNDLE_SHA vvv
        static constexpr StringLiteral latest_tarball_name = "kmpkg-standalone-bundle-latest.tar.gz";
        const auto bundle_tarball = download_root / latest_tarball_name;
        context.report(DiagnosticLine{DiagKind::Warning, msg::format(msgDownloadingKmpkgStandaloneBundleLatest)});
        std::error_code ec;
        fs.remove(bundle_tarball, ec);
        if (ec)
        {
            context.report_error(format_filesystem_call_error(ec, "remove", {bundle_tarball}));
            return nullopt;
        }

        const auto bundle_uri =
            "https://github.com/kumose/kmpkg-tool/releases/latest/download/kmpkg-standalone-bundle.tar.gz";
        if (!download_file_asset_cached(context,
                                        null_sink,
                                        asset_cache_settings,
                                        fs,
                                        bundle_uri,
                                        {},
                                        bundle_tarball,
                                        latest_tarball_name,
                                        nullopt))
        {
            return nullopt;
        }
#endif // ^^^ !KMPKG_STANDALONE_BUNDLE_SHA
        return bundle_tarball;
    }
}

namespace kmpkg
{

    constexpr CommandMetadata CommandBootstrapStandaloneMetadata{
        "bootstrap-standalone",
        msgCmdBootstrapStandaloneSynopsis,
        {"kmpkg bootstrap-standalone"},
        Undocumented,
        AutocompletePriority::Never,
        0,
        0,
        {},
        nullptr,
    };

    void command_bootstrap_standalone_and_exit(const KmpkgCmdArguments& args, const Filesystem& fs)
    {
        (void)args.parse_arguments(CommandBootstrapStandaloneMetadata);

        AssetCachingSettings asset_cache_settings;
        const auto maybe_kmpkg_root_env = args.kmpkg_root_dir_env.get();
        if (!maybe_kmpkg_root_env)
        {
            Checks::msg_exit_with_message(KMPKG_LINE_INFO, msgKmpkgRootRequired);
        }

        const auto kmpkg_root = fs.almost_canonical(*maybe_kmpkg_root_env, KMPKG_LINE_INFO);
        fs.create_directories(kmpkg_root, KMPKG_LINE_INFO);
        auto maybe_tarball =
            download_kmpkg_standalone_bundle(console_diagnostic_context, asset_cache_settings, fs, kmpkg_root);
        auto tarball = maybe_tarball.get();
        if (!tarball)
        {
            Checks::exit_fail(KMPKG_LINE_INFO);
        }

        extract_tar(find_system_tar(fs).value_or_exit(KMPKG_LINE_INFO), *tarball, kmpkg_root);
        Checks::exit_success(KMPKG_LINE_INFO);
    }
}
