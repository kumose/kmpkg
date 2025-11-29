#include <kmpkg/commands.z-upload-metrics.h>

#if defined(_WIN32)
#include <kmpkg/base/checks.h>
#include <kmpkg/base/files.h>

#include <kmpkg/metrics.h>
#include <kmpkg/kmpkgcmdarguments.h>

namespace kmpkg
{
    constexpr CommandMetadata CommandZUploadMetricsMetadata{
        "z-upload-metrics",
        {/*intentionally undocumented*/},
        {},
        Undocumented,
        AutocompletePriority::Never,
        1,
        1,
        {},
        nullptr,
    };

    void command_z_upload_metrics_and_exit(const KmpkgCmdArguments& args, const Filesystem& fs)
    {
        const auto parsed = args.parse_arguments(CommandZUploadMetricsMetadata);
        const auto& payload_path = parsed.command_arguments[0];
        auto payload = fs.read_contents(payload_path, KMPKG_LINE_INFO);
        winhttp_upload_metrics(payload);
        Checks::exit_success(KMPKG_LINE_INFO);
    }
}
#endif // defined(_WIN32)
