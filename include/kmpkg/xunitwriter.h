#pragma once

#include <kmpkg/fwd/build.h>
#include <kmpkg/fwd/packagespec.h>
#include <kmpkg/fwd/triplet.h>

#include <kmpkg/base/chrono.h>

#include <chrono>
#include <map>
#include <string>
#include <vector>

namespace kmpkg
{
    struct CiBuiltResult
    {
        std::string package_abi;
        InternalFeatureSet feature_list;
        std::chrono::system_clock::time_point start_time;
        ElapsedTime timing;
    };

    struct CiResult
    {
        BuildResult code;
        Optional<CiBuiltResult> build;

        std::string to_string() const;
        void to_string(std::string& out_str) const;
    };

    struct XunitTest;

    // https://xunit.net/docs/format-xml-v2
    struct XunitWriter
    {
    public:
        // Out of line con/destructor avoids exposing XunitTest
        XunitWriter();
        ~XunitWriter();
        void add_test_results(const PackageSpec& spec, const CiResult& result);

        std::string build_xml(Triplet controlling_triplet) const;

    private:
        std::map<std::string, std::vector<XunitTest>> m_tests;
    };
}
