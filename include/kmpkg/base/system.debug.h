#pragma once

#include <kmpkg/base/fwd/messages.h>

#include <kmpkg/base/lineinfo.h>
#include <kmpkg/base/strings.h>

#include <atomic>

namespace kmpkg::Debug
{
    extern std::atomic<bool> g_debugging;

    template<class... Args>
    void print(const Args&... args)
    {
        if (g_debugging) msg::write_unlocalized_text(Color::none, Strings::concat("[DEBUG] ", args...));
    }
    template<class... Args>
    void println(const Args&... args)
    {
        if (g_debugging) msg::write_unlocalized_text(Color::none, Strings::concat("[DEBUG] ", args..., '\n'));
    }
}
