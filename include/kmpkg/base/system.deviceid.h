#pragma once

#include <kmpkg/base/fwd/files.h>
#include <kmpkg/base/fwd/stringview.h>

#include <string>

namespace kmpkg
{
    bool validate_device_id(StringView uuid);

    std::string get_device_id(const kmpkg::Filesystem& fs);
}
