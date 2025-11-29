#pragma once

#include <kmpkg/base/fwd/fmt.h>

#include <string>

namespace kmpkg
{
    struct LineInfo
    {
        int line_number;
        const char* file_name;
        const char* function_name;

        std::string to_string() const;
    };
}

#define KMPKG_LINE_INFO                                                                                                \
    kmpkg::LineInfo { __LINE__, __FILE__, __func__ }

KMPKG_FORMAT_WITH_TO_STRING(kmpkg::LineInfo);
