#pragma once

#include <kmpkg/base/fwd/stringview.h>

namespace kmpkg
{
#if defined(_WIN32)
    enum class Color : unsigned short
    {
        none = 0,
        success = 0x0A, // FOREGROUND_GREEN | FOREGROUND_INTENSITY
        error = 0xC,    // FOREGROUND_RED | FOREGROUND_INTENSITY
        warning = 0xE,  // FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
    };
#else
    enum class Color : char
    {
        none = 0,
        success = '2', // [with 9] bright green
        error = '1',   // [with 9] bright red
        warning = '3', // [with 9] bright yellow
    };
#endif

    enum class OutputStream
    {
        StdOut,
        StdErr,
    };

    struct LocalizedString;
    struct MessageSink;

    namespace msg
    {
        template<class... Tags>
        struct MessageT;

        template<class Tag, class Type>
        struct TagArg;
    }
}

namespace kmpkg::msg
{
    extern OutputStream default_output_stream;
    void write_unlocalized_text(Color c, kmpkg::StringView sv);
    void write_unlocalized_text_to_stdout(Color c, kmpkg::StringView sv);
    void write_unlocalized_text_to_stderr(Color c, kmpkg::StringView sv);
}
