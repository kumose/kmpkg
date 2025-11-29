#pragma once

#include <kmpkg/base/fwd/files.h>

#include <kmpkg/fwd/dependencies.h>
#include <kmpkg/fwd/packagespec.h>
#include <kmpkg/fwd/kmpkgpaths.h>

#include <kmpkg/base/strings.h>

#include <kmpkg/binarycaching.h>

namespace kmpkg
{
    // Turns:
    // - <XXXX>-<YY>-<ZZ><whatever> -> <X>.<Y>.<Z>-kmpkg<abitag>
    // - v?<X> -> <X>.0.0-kmpkg<abitag>
    //   - this avoids turning 20-01-01 into 20.0.0-kmpkg<abitag>
    // - v?<X>.<Y><whatever> -> <X>.<Y>.0-kmpkg<abitag>
    // - v?<X>.<Y>.<Z><whatever> -> <X>.<Y>.<Z>-kmpkg<abitag>
    // - anything else -> 0.0.0-kmpkg<abitag>
    std::string format_version_for_feedref(StringView version_text, StringView abi_tag);

    struct FeedReference
    {
        FeedReference(std::string id, std::string version) : id(std::move(id)), version(std::move(version)) { }

        std::string id;
        std::string version;

        std::string nupkg_filename() const { return Strings::concat(id, '.', version, ".nupkg"); }
    };

    FeedReference make_nugetref(const InstallPlanAction& action, StringView prefix);

    std::string generate_nuspec(const Path& package_dir,
                                const InstallPlanAction& action,
                                StringView id_prefix,
                                const NuGetRepoInfo& repo_info);
}
