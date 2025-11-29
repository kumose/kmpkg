#pragma once

#include <kmpkg/fwd/binaryparagraph.h>

#include <kmpkg/base/contractual-constants.h>
#include <kmpkg/base/path.h>

#include <kmpkg/packagespec.h>
#include <kmpkg/triplet.h>

namespace kmpkg
{
    struct InstalledPaths
    {
        explicit InstalledPaths(Path&& root) : m_root(std::move(root)) { }

        const Path& root() const { return m_root; }
        Path listfile_path(const BinaryParagraph& pgh) const;

        Path kmpkg_dir() const { return m_root / FileKmpkg; }
        Path kmpkg_dir_status_file() const { return kmpkg_dir() / FileStatus; }
        Path kmpkg_dir_info() const { return kmpkg_dir() / FileInfo; }
        Path kmpkg_dir_updates() const { return kmpkg_dir() / FileUpdates; }
        Path compiler_hash_cache_file() const { return kmpkg_dir() / FileCompilerFileHashCacheDotJson; }
        Path lockfile_path() const { return kmpkg_dir() / FileKmpkgLock; }
        Path triplet_dir(Triplet t) const { return m_root / t.canonical_name(); }
        Path share_dir(const PackageSpec& p) const { return triplet_dir(p.triplet()) / FileShare / p.name(); }
        Path usage_file(const PackageSpec& p) const { return share_dir(p) / FileUsage; }
        Path spdx_file(const PackageSpec& p) const { return share_dir(p) / FileKmpkgSpdxJson; }
        Path kmpkg_port_config_cmake(const PackageSpec& p) const { return share_dir(p) / FileKmpkgPortConfig; }

    private:
        Path m_root;
    };
}
