#pragma once
#include <kmpkg/base/fwd/file-contents.h>

#include <string>

namespace kmpkg
{
    struct FileContents
    {
        std::string content;
        std::string origin;
    };
}
