#include <kmpkg/base/fwd/message_sinks.h>

#include <kmpkg/buildenvironment.h>
#include <kmpkg/installedpaths.h>
#include <kmpkg/tools.h>
#include <kmpkg/kmpkgpaths.h>

namespace kmpkg
{
    Command make_cmake_cmd(const KmpkgPaths& paths,
                           const Path& cmake_script,
                           std::vector<CMakeVariable>&& pass_variables)
    {
        auto local_variables = std::move(pass_variables);
        local_variables.emplace_back("KMPKG_ROOT_DIR", paths.root);
        local_variables.emplace_back("PACKAGES_DIR", paths.packages());
        local_variables.emplace_back("BUILDTREES_DIR", paths.buildtrees());
        local_variables.emplace_back("_KMPKG_INSTALLED_DIR", paths.installed().root());
        local_variables.emplace_back("DOWNLOADS", paths.downloads);
        local_variables.emplace_back("KMPKG_MANIFEST_INSTALL", "OFF");
        return make_basic_cmake_cmd(paths.get_tool_exe(Tools::CMAKE, out_sink), cmake_script, local_variables);
    }
}
