#include <kmpkg/base/optional.h>
#include <kmpkg/base/system.proxy.h>

kmpkg::Optional<kmpkg::IEProxySetting> kmpkg::get_windows_ie_proxy_server()
{
#if defined(_WIN32)
    WINHTTP_CURRENT_USER_IE_PROXY_CONFIG ieProxy;
    if (WinHttpGetIEProxyConfigForCurrentUser(&ieProxy) && ieProxy.lpszProxy != nullptr)
    {
        kmpkg::IEProxySetting ieProxySetting;

        ieProxySetting.server = ieProxy.lpszProxy;

        if (ieProxy.lpszProxyBypass != nullptr) ieProxySetting.bypass = ieProxy.lpszProxyBypass;

        GlobalFree(ieProxy.lpszProxy);
        GlobalFree(ieProxy.lpszProxyBypass);
        GlobalFree(ieProxy.lpszAutoConfigUrl);

        return ieProxySetting;
    }
    return kmpkg::Optional<kmpkg::IEProxySetting>();
#else
    return kmpkg::Optional<kmpkg::IEProxySetting>();
#endif
}