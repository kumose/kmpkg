#include <kmpkg/base/fwd/message_sinks.h>

#include <kmpkg/base/checks.h>
#include <kmpkg/base/files.h>
#include <kmpkg/base/hash.h>
#include <kmpkg/base/messages.h>
#include <kmpkg/base/strings.h>
#include <kmpkg/base/util.h>

#include <kmpkg/commands.add.h>
#include <kmpkg/configure-environment.h>
#include <kmpkg/documentation.h>
#include <kmpkg/metrics.h>
#include <kmpkg/paragraphs.h>
#include <kmpkg/kmpkgcmdarguments.h>
#include <kmpkg/kmpkgpaths.h>

#include <limits.h>

namespace kmpkg
{
    constexpr CommandMetadata CommandAddMetadata{
        "add",
        msgCmdAddSynopsis,
        {msgCmdAddExample1, "kmpkg add port png", msgCmdAddExample2, "kmpkg add artifact cmake"},
        Undocumented,
        AutocompletePriority::Public,
        2,
        SIZE_MAX,
        {{}, CommonSelectArtifactVersionSettings},
        nullptr,
    };

    void command_add_and_exit(const KmpkgCmdArguments& args, const KmpkgPaths& paths)
    {
        MetricsSubmission metrics;
        auto parsed = args.parse_arguments(CommandAddMetadata);
        auto&& selector = parsed.command_arguments[0];

        if (selector == "artifact")
        {
            Checks::msg_check_exit(KMPKG_LINE_INFO,
                                   parsed.command_arguments.size() <= 2,
                                   msgAddArtifactOnlyOne,
                                   msg::command_line = "kmpkg add artifact");

            auto& artifact_name = parsed.command_arguments[1];
            auto artifact_hash = Hash::get_string_hash(artifact_name, Hash::Algorithm::Sha256);
            metrics.track_string(StringMetric::CommandContext, "artifact");
            metrics.track_string(StringMetric::CommandArgs, artifact_hash);
            get_global_metrics_collector().track_submission(std::move(metrics));

            std::vector<std::string> ecmascript_args;
            ecmascript_args.emplace_back("add");
            ecmascript_args.emplace_back(artifact_name);
            auto maybe_version = Util::lookup_value(parsed.settings, SwitchVersion);
            if (auto version = maybe_version.get())
            {
                ecmascript_args.emplace_back("--version");
                ecmascript_args.emplace_back(*version);
            }

            Checks::exit_with_code(KMPKG_LINE_INFO, run_configure_environment_command(paths, ecmascript_args));
        }

        if (selector == "port")
        {
            auto manifest = paths.get_manifest().get();
            if (!manifest)
            {
                Checks::msg_exit_with_message(
                    KMPKG_LINE_INFO,
                    msg::format(msgAddPortRequiresManifest, msg::command_line = "kmpkg add port")
                        .append_raw('\n')
                        .append(msgSeeURL, msg::url = docs::add_command_url));
            }

            if (Util::Maps::contains(parsed.settings, SwitchVersion))
            {
                Checks::msg_exit_with_error(KMPKG_LINE_INFO, msgAddVersionArtifactsOnly);
            }

            std::vector<ParsedQualifiedSpecifier> specs;
            specs.reserve(parsed.command_arguments.size() - 1);
            for (std::size_t idx = 1; idx < parsed.command_arguments.size(); ++idx)
            {
                specs.push_back(parse_qualified_specifier(parsed.command_arguments[idx],
                                                          AllowFeatures::Yes,
                                                          ParseExplicitTriplet::Forbid,
                                                          AllowPlatformSpec::No)
                                    .value_or_exit(KMPKG_LINE_INFO));
            }

            auto maybe_manifest_scf =
                SourceControlFile::parse_project_manifest_object(manifest->path, manifest->manifest, out_sink);
            auto pmanifest_scf = maybe_manifest_scf.get();
            if (!pmanifest_scf)
            {
                msg::print(Color::error,
                           std::move(maybe_manifest_scf)
                               .error()
                               .append_raw('\n')
                               .append_raw(NotePrefix)
                               .append(msgSeeURL, msg::url = docs::manifests_url)
                               .append_raw('\n'));
                Checks::exit_fail(KMPKG_LINE_INFO);
            }

            auto& manifest_scf = **pmanifest_scf;
            for (const auto& spec : specs)
            {
                auto dep = Util::find_if(manifest_scf.core_paragraph->dependencies, [&spec](Dependency& dep) {
                    return dep.name == spec.name.value && !dep.host &&
                           structurally_equal(spec.platform_or_always_true(), dep.platform);
                });

                std::vector<std::string> feature_names;
                bool is_core = false;
                if (auto pspec_features = spec.features.get())
                {
                    for (auto&& feature_name : *pspec_features)
                    {
                        if (feature_name.value == "core")
                        {
                            is_core = true;
                        }
                        else
                        {
                            feature_names.push_back(feature_name.value);
                        }
                    }
                }

                const auto features = Util::fmap(feature_names, [](const std::string& feature) {
                    Checks::check_exit(KMPKG_LINE_INFO,
                                       !feature.empty() && feature != FeatureNameCore && feature != FeatureNameDefault);
                    return DependencyRequestedFeature{feature};
                });
                if (dep == manifest_scf.core_paragraph->dependencies.end())
                {
                    auto& new_dep = manifest_scf.core_paragraph->dependencies.emplace_back(
                        Dependency{spec.name.value, features, spec.platform_or_always_true()});
                    if (is_core)
                    {
                        new_dep.default_features = false;
                    }
                }
                else
                {
                    for (const auto& feature : features)
                    {
                        if (!Util::Vectors::contains(dep->features, feature))
                        {
                            dep->features.push_back(feature);
                        }
                    }
                    if (!is_core)
                    {
                        dep->default_features = true;
                    }
                }
            }

            paths.get_filesystem().write_contents(
                manifest->path, Json::stringify(serialize_manifest(manifest_scf)), KMPKG_LINE_INFO);
            msg::println(msgAddPortSucceeded);

            auto command_args_hash =
                Strings::join(" ", Util::fmap(specs, [](const ParsedQualifiedSpecifier& spec) -> std::string {
                                  return Hash::get_string_hash(spec.name.value, Hash::Algorithm::Sha256);
                              }));
            metrics.track_string(StringMetric::CommandContext, "port");
            metrics.track_string(StringMetric::CommandArgs, command_args_hash);
            get_global_metrics_collector().track_submission(std::move(metrics));

            Checks::exit_success(KMPKG_LINE_INFO);
        }

        Checks::msg_exit_with_message(
            KMPKG_LINE_INFO,
            msg::format(msgAddCommandFirstArg).append_raw('\n').append(msgSeeURL, msg::url = docs::add_command_url));
    }
}
