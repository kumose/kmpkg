#pragma once

#include <kmpkg/base/lineinfo.h>
#include <kmpkg/base/stringview.h>

namespace kmpkg::Util
{
    std::string nullvalue_to_string(const ZStringView enum_name);

    [[noreturn]] void nullvalue_used(const LineInfo& line_info, const ZStringView enum_name);
}
