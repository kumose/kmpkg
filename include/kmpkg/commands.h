#pragma once

#include <kmpkg/base/fwd/files.h>
#include <kmpkg/base/fwd/span.h>
#include <kmpkg/base/fwd/stringview.h>

#include <kmpkg/fwd/triplet.h>
#include <kmpkg/fwd/kmpkgcmdarguments.h>
#include <kmpkg/fwd/kmpkgpaths.h>

#include <string>

namespace kmpkg
{
    using BasicCommandFn = void (*)(const KmpkgCmdArguments& args, const Filesystem& fs);
    using PathsCommandFn = void (*)(const KmpkgCmdArguments& args, const KmpkgPaths& paths);
    using TripletCommandFn = void (*)(const KmpkgCmdArguments& args,
                                      const KmpkgPaths& paths,
                                      Triplet default_triplet,
                                      Triplet host_triplet);

    template<class T>
    struct CommandRegistration
    {
        const CommandMetadata& metadata;
        T function;
    };

    extern const View<CommandRegistration<BasicCommandFn>> basic_commands;
    extern const View<CommandRegistration<PathsCommandFn>> paths_commands;
    extern const View<CommandRegistration<TripletCommandFn>> triplet_commands;

    std::vector<const CommandMetadata*> get_all_commands_metadata();

    std::string get_zero_args_usage();
    void print_full_command_list();
}
