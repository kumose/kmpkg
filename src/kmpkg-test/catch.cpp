#define CATCH_CONFIG_RUNNER
#include <kmpkg-test/util.h>

#include <kmpkg/base/system.debug.h>
#include <kmpkg/base/system.h>

namespace kmpkg::Checks
{
    void on_final_cleanup_and_exit() { }
}

int main(int argc, char** argv)
{
    if (kmpkg::get_environment_variable("KMPKG_DEBUG").value_or("") == "1") kmpkg::Debug::g_debugging = true;
    // We set KMPKG_ROOT to an invalid value to ensure unit tests do not attempt to instantiate KmpkgRoot
    kmpkg::set_environment_variable("KMPKG_ROOT", "KMPKG_TESTS_SHOULD_NOT_USE_KMPKG_ROOT");

    return Catch::Session().run(argc, argv);
}
