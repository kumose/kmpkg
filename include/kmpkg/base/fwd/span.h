#pragma once

namespace kmpkg
{
    template<class T>
    struct Span;

    template<class T>
    using View = Span<const T>;
}
