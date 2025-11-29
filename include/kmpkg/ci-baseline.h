#pragma once
#include <kmpkg/fwd/build.h>
#include <kmpkg/fwd/ci-baseline.h>

#include <kmpkg/base/expected.h>
#include <kmpkg/base/sortedvector.h>
#include <kmpkg/base/span.h>

#include <kmpkg/packagespec.h>
#include <kmpkg/triplet.h>

#include <initializer_list>
#include <string>
#include <vector>

namespace kmpkg
{
    struct CiBaselineLine
    {
        std::string port_name;
        Triplet triplet;
        CiBaselineState state;

        friend bool operator==(const CiBaselineLine& lhs, const CiBaselineLine& rhs)
        {
            return lhs.port_name == rhs.port_name && lhs.triplet == rhs.triplet && lhs.state == rhs.state;
        }

        friend bool operator!=(const CiBaselineLine& lhs, const CiBaselineLine& rhs) { return !(lhs == rhs); }
    };

    struct TripletExclusions
    {
        Triplet triplet;
        SortedVector<std::string> exclusions;

        TripletExclusions(const Triplet& triplet);
        TripletExclusions(const Triplet& triplet, SortedVector<std::string>&& exclusions);
    };

    struct ExclusionsMap
    {
        std::vector<TripletExclusions> triplets;

        void insert(Triplet triplet, SortedVector<std::string>&& exclusions);
        bool is_excluded(const PackageSpec& spec) const;
    };

    std::vector<CiBaselineLine> parse_ci_baseline(StringView text, StringView origin, ParseMessages& messages);

    struct CiBaselineData
    {
        SortedVector<PackageSpec> expected_failures;
        SortedVector<PackageSpec> required_success;
    };

    CiBaselineData parse_and_apply_ci_baseline(View<CiBaselineLine> lines,
                                               ExclusionsMap& exclusions_map,
                                               SkipFailures skip_failures);

    LocalizedString format_ci_result(const PackageSpec& spec,
                                     BuildResult result,
                                     const CiBaselineData& cidata,
                                     const std::string* cifile,
                                     bool allow_unexpected_passing);
}
