#pragma once

#include <kmpkg/base/fwd/messages.h>

#include <kmpkg/fwd/versions.h>

#include <kmpkg/base/contractual-constants.h>
#include <kmpkg/base/jsonreader.h>
#include <kmpkg/base/stringview.h>

namespace kmpkg
{
    extern const Json::IDeserializer<Version>& baseline_version_tag_deserializer;

    Optional<SchemedVersion> visit_optional_schemed_version(const LocalizedString& parent_type,
                                                            Json::Reader& r,
                                                            const Json::Object& obj);

    SchemedVersion visit_required_schemed_version(const LocalizedString& parent_type,
                                                  Json::Reader& r,
                                                  const Json::Object& obj);

    Version visit_version_override_version(const LocalizedString& parent_type,
                                           Json::Reader& r,
                                           const Json::Object& obj);

#define KMPKG_SCHEMED_DESERIALIZER_FIELDS                                                                              \
    JsonIdVersion, JsonIdVersionSemver, JsonIdVersionString, JsonIdVersionDate, JsonIdPortVersion

    void serialize_schemed_version(Json::Object& out_obj, VersionScheme scheme, const Version& version);

    struct VersionConstraintStringDeserializer : Json::StringDeserializer
    {
        virtual LocalizedString type_name() const override;

        static const VersionConstraintStringDeserializer instance;
    };
}
