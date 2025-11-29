#pragma once

#include <kmpkg/base/fwd/json.h>
#include <kmpkg/base/fwd/optional.h>

#include <kmpkg/fwd/build.h>
#include <kmpkg/fwd/cmakevars.h>
#include <kmpkg/fwd/commands.install.h>
#include <kmpkg/fwd/dependencies.h>
#include <kmpkg/fwd/statusparagraphs.h>
#include <kmpkg/fwd/triplet.h>
#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

#include <set>

namespace kmpkg
{
    enum class DryRun : bool
    {
        No,
        Yes
    };

    extern const CommandMetadata CommandSetInstalledMetadata;

    /**
     * @brief adjust_action_plan_to_status_db creates an action plan that installs only the requested ports.
     * All ABIs must be computed first.
     * @param action_plan An action plan that was created with an empty status db
     * @param status_db The status db of the installed folder
     * @returns A set of PackageSpec's that are already installed
     */
    std::set<PackageSpec> adjust_action_plan_to_status_db(ActionPlan& action_plan, const StatusParagraphs& status_db);

    void command_set_installed_and_exit_ex(const KmpkgCmdArguments& args,
                                           const KmpkgPaths& paths,
                                           Triplet host_triplet,
                                           const BuildPackageOptions& build_options,
                                           const CMakeVars::CMakeVarProvider& cmake_vars,
                                           ActionPlan action_plan,
                                           DryRun dry_run,
                                           PrintUsage print_usage,
                                           const Optional<Path>& maybe_pkgconfig,
                                           bool include_manifest_in_github_issue);
    void command_set_installed_and_exit(const KmpkgCmdArguments& args,
                                        const KmpkgPaths& paths,
                                        Triplet default_triplet,
                                        Triplet host_triplet);

    Optional<Json::Object> create_dependency_graph_snapshot(const KmpkgCmdArguments& args,
                                                            const ActionPlan& action_plan);
}
