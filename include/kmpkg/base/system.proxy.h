#pragma once

#include <kmpkg/base/fwd/optional.h>

#include <string>

/*
 * This is a helper class. It reads IE Proxy settings.
 * For coherence of kmpkg design "using HTTP(S)_PROXY not WPAD", it is a user-friendly design to manually
 * read the IE Proxy settings on Windows, and set the environment variables HTTP(S)_PROXY to the same
 * when starting an external program (like CMake, which file(DOWNLOAD) only accept proxy settings
 * by HTTP(S)_PROXY variables).
 */

namespace kmpkg
{
    struct IEProxySetting
    {
        std::wstring server;
        std::wstring bypass;
    };

    kmpkg::Optional<IEProxySetting> get_windows_ie_proxy_server();
}