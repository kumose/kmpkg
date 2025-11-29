#pragma once

#include <kmpkg/base/fwd/expected.h>
#include <kmpkg/base/fwd/file-contents.h>
#include <kmpkg/base/fwd/fmt.h>
#include <kmpkg/base/fwd/optional.h>
#include <kmpkg/base/fwd/stringview.h>

#include <kmpkg/fwd/bundlesettings.h>

#include <string>

namespace kmpkg
{
    std::string to_string(DeploymentKind);
    StringLiteral to_string_literal(DeploymentKind) noexcept;

    struct BundleSettings
    {
        bool read_only = false;
        bool use_git_registry = false;
        Optional<std::string> embedded_git_sha;
        DeploymentKind deployment = DeploymentKind::Git;
        Optional<std::string> vsversion;

        std::string to_string() const;
    };

    ExpectedL<BundleSettings> try_parse_bundle_settings(const FileContents& bundle_contents);
}

KMPKG_FORMAT_WITH_TO_STRING_LITERAL_NONMEMBER(kmpkg::DeploymentKind);
KMPKG_FORMAT_WITH_TO_STRING(kmpkg::BundleSettings);
