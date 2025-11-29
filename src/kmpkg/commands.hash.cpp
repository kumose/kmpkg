#include <kmpkg/base/files.h>
#include <kmpkg/base/hash.h>

#include <kmpkg/commands.hash.h>
#include <kmpkg/kmpkgcmdarguments.h>

namespace kmpkg
{
    constexpr CommandMetadata CommandHashMetadata{
        "hash",
        msgCmdHashSynopsis,
        {msgCmdHashExample1, msgCmdHashExample2, "kmpkg hash boost_1_62_0.tar.bz2"},
        Undocumented,
        AutocompletePriority::Public,
        1,
        2,
        {},
        nullptr,
    };

    void command_hash_and_exit(const KmpkgCmdArguments& args, const Filesystem& fs)
    {
        const auto parsed = args.parse_arguments(CommandHashMetadata);

        const auto file_to_hash = (fs.current_path(KMPKG_LINE_INFO) / parsed.command_arguments[0]).lexically_normal();

        auto algorithm = kmpkg::Hash::Algorithm::Sha512;
        if (parsed.command_arguments.size() == 2)
        {
            algorithm = kmpkg::Hash::algorithm_from_string(parsed.command_arguments[1]).value_or_exit(KMPKG_LINE_INFO);
        }

        const std::string hash = kmpkg::Hash::get_file_hash(fs, file_to_hash, algorithm).value_or_exit(KMPKG_LINE_INFO);
        msg::write_unlocalized_text(Color::none, hash + '\n');
        Checks::exit_success(KMPKG_LINE_INFO);
    }
}
