#pragma once

#include <kmpkg/base/system-headers.h>

#include <catch2/catch.hpp>

#include <kmpkg/base/fwd/files.h>

#include <kmpkg/fwd/tools.h>

#include <kmpkg/base/files.h>
#include <kmpkg/base/fmt.h>
#include <kmpkg/base/messages.h>
#include <kmpkg/base/pragmas.h>
#include <kmpkg/base/strings.h>

#include <kmpkg/packagespec.h>
#include <kmpkg/statusparagraph.h>

#include <iomanip>
#include <memory>

#define CHECK_EC(ec)                                                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
        if (ec)                                                                                                        \
        {                                                                                                              \
            FAIL(ec.message());                                                                                        \
        }                                                                                                              \
    } while (0)

namespace Catch
{
    template<>
    struct StringMaker<kmpkg::FullPackageSpec>
    {
        static std::string convert(kmpkg::FullPackageSpec const& value)
        {
            return fmt::format("{}[{}]:{}",
                               value.package_spec.name(),
                               kmpkg::Strings::join(",", value.features),
                               value.package_spec.triplet());
        }
    };

    template<>
    struct StringMaker<kmpkg::FeatureSpec>
    {
        static std::string convert(kmpkg::FeatureSpec const& value)
        {
            return fmt::format("{}[{}]:{}", value.port(), value.feature(), value.triplet());
        }
    };

    template<>
    struct StringMaker<kmpkg::Triplet>
    {
        static const std::string& convert(const kmpkg::Triplet& triplet) { return triplet.canonical_name(); }
    };

    template<>
    struct StringMaker<kmpkg::LocalizedString>
    {
        static const std::string convert(const kmpkg::LocalizedString& value) { return "LL\"" + value.data() + "\""; }
    };

    template<>
    struct StringMaker<kmpkg::PackageSpec>
    {
        static const std::string convert(const kmpkg::PackageSpec& value) { return value.to_string(); }
    };

    template<>
    struct StringMaker<kmpkg::Path>
    {
        static const std::string convert(const kmpkg::Path& value) { return "\"" + value.native() + "\""; }
    };

    template<>
    struct StringMaker<std::pair<kmpkg::Path, kmpkg::Path>>
    {
        static const std::string convert(const std::pair<kmpkg::Path, kmpkg::Path>& value)
        {
            return "{\"" + value.first.native() + "\", \"" + value.second.native() + "\"}";
        }
    };

    template<>
    struct StringMaker<kmpkg::Version>
    {
        static const std::string convert(const kmpkg::Version& value) { return value.to_string(); }
    };
}

namespace kmpkg
{
    inline std::ostream& operator<<(std::ostream& os, const PackageSpec& value) { return os << value.to_string(); }

    inline std::ostream& operator<<(std::ostream& os, const LocalizedString& value)
    {
        return os << "LL" << std::quoted(value.data());
    }

    inline std::ostream& operator<<(std::ostream& os, const Path& value) { return os << value.native(); }

    template<class T>
    inline auto operator<<(std::ostream& os, const Optional<T>& value) -> decltype(os << *(value.get()))
    {
        if (auto v = value.get())
        {
            return os << *v;
        }
        else
        {
            return os << "nullopt";
        }
    }
}

namespace kmpkg::Test
{
    std::unique_ptr<SourceControlFile> make_control_file(
        const char* name,
        const char* depends,
        const std::vector<std::pair<const char*, const char*>>& features = {},
        const std::vector<const char*>& default_features = {});

    ExpectedL<std::unique_ptr<SourceControlFile>> test_parse_control_file(
        const std::vector<std::unordered_map<std::string, std::string>>& v);

    std::unique_ptr<kmpkg::StatusParagraph> make_status_pgh(const char* name,
                                                            const char* depends = "",
                                                            const char* default_features = "",
                                                            const char* triplet = "x86-windows");

    std::unique_ptr<kmpkg::StatusParagraph> make_status_feature_pgh(const char* name,
                                                                    const char* feature,
                                                                    const char* depends = "",
                                                                    const char* triplet = "x86-windows");

    extern const Triplet X86_WINDOWS;
    extern const Triplet X64_WINDOWS;
    extern const Triplet X64_WINDOWS_STATIC;
    extern const Triplet X64_WINDOWS_STATIC_MD;
    extern const Triplet ARM64_WINDOWS;
    extern const Triplet X86_UWP;
    extern const Triplet X64_UWP;
    extern const Triplet ARM_UWP;
    extern const Triplet X64_ANDROID;
    extern const Triplet X64_OSX;
    extern const Triplet X64_LINUX;

    /// <summary>
    /// Map of source control files by their package name.
    /// </summary>
    struct PackageSpecMap
    {
        std::unordered_map<std::string, SourceControlFileAndLocation> map;
        Triplet triplet;
        PackageSpecMap(Triplet t = X86_WINDOWS) noexcept : triplet(t) { }

        PackageSpec emplace(const char* name,
                            const char* depends = "",
                            const std::vector<std::pair<const char*, const char*>>& features = {},
                            const std::vector<const char*>& default_features = {});

        PackageSpec emplace(kmpkg::SourceControlFileAndLocation&& scfl);
    };

    std::vector<FullPackageSpec> parse_test_fspecs(StringView sv);

    template<class R1, class R2>
    void check_ranges(const R1& r1, const R2& r2)
    {
        CHECK(r1.size() == r2.size());
        auto it1 = r1.begin();
        auto e1 = r1.end();
        auto it2 = r2.begin();
        auto e2 = r2.end();
        for (; it1 != e1 && it2 != e2; ++it1, ++it2)
        {
            CHECK(*it1 == *it2);
        }
    }

    void check_json_eq(const Json::Value& l, const Json::Value& r);
    void check_json_eq(const Json::Object& l, const Json::Object& r);
    void check_json_eq(const Json::Array& l, const Json::Array& r);

    void check_json_eq_ordered(const Json::Value& l, const Json::Value& r);
    void check_json_eq_ordered(const Json::Object& l, const Json::Object& r);
    void check_json_eq_ordered(const Json::Array& l, const Json::Array& r);

    const Path& base_temporary_directory() noexcept;

    Optional<std::string> diff_lines(StringView a, StringView b);
}

#define REQUIRE_LINES(a, b)                                                                                            \
    do                                                                                                                 \
    {                                                                                                                  \
        if (auto delta = ::kmpkg::Test::diff_lines((b), (a))) FAIL(*delta.get());                                      \
    } while (0)
