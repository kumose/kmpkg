#pragma once

#include <kmpkg/base/files.h>
#include <kmpkg/base/message_sinks.h>

namespace kmpkg
{
    struct FileSink : MessageSink
    {
        Path m_log_file;
        WriteFilePointer m_out_file;

        FileSink(const Filesystem& fs, StringView log_file, Append append_to_file)
            : m_log_file(log_file), m_out_file(fs.open_for_write(m_log_file, append_to_file, KMPKG_LINE_INFO))
        {
        }
        void println(const MessageLine& line) override;
        void println(MessageLine&& line) override;
    };
}
