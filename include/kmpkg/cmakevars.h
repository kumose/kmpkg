#pragma once

#include <kmpkg/base/fwd/files.h>
#include <kmpkg/base/fwd/optional.h>
#include <kmpkg/base/fwd/span.h>

#include <kmpkg/fwd/dependencies.h>
#include <kmpkg/fwd/packagespec.h>
#include <kmpkg/fwd/portfileprovider.h>
#include <kmpkg/fwd/triplet.h>
#include <kmpkg/fwd/kmpkgpaths.h>

#include <memory>
#include <string>
#include <unordered_map>

namespace kmpkg::CMakeVars
{
    using CMakeVars = std::unordered_map<std::string, std::string>;

    struct CMakeVarProvider
    {
        virtual ~CMakeVarProvider() = default;

        virtual Optional<const CMakeVars&> get_generic_triplet_vars(Triplet triplet) const = 0;

        virtual Optional<const CMakeVars&> get_dep_info_vars(const PackageSpec& spec) const = 0;

        const CMakeVars& get_or_load_dep_info_vars(const PackageSpec& spec, Triplet host_triplet) const;

        virtual Optional<const CMakeVars&> get_tag_vars(const PackageSpec& spec) const = 0;

        virtual void load_generic_triplet_vars(Triplet triplet) const = 0;

        virtual void load_dep_info_vars(View<PackageSpec> specs, Triplet host_triplet) const = 0;

        virtual void load_tag_vars(View<FullPackageSpec> specs,
                                   View<Path> port_locations,
                                   Triplet host_triplet) const = 0;

        void load_tag_vars(const ActionPlan& action_plan, Triplet host_triplet) const;
    };

    std::unique_ptr<CMakeVarProvider> make_triplet_cmake_var_provider(const KmpkgPaths& paths);
}
