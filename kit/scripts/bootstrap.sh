#!/bin/sh

# Find .kmpkg-root.
kmpkgRootDir=$(X= cd -- "$(dirname -- "$0")" && pwd -P)
while [ "$kmpkgRootDir" != "/" ] && ! [ -e "$kmpkgRootDir/.kmpkg-root" ]; do
    kmpkgRootDir="$(dirname "$kmpkgRootDir")"
done

# Parse arguments.
kmpkgDisableMetrics="OFF"
kmpkgUseSystem=false
kmpkgUseMuslC="OFF"
kmpkgSkipDependencyChecks="OFF"
for var in "$@"
do
    if [ "$var" = "-disableMetrics" -o "$var" = "--disableMetrics" ]; then
        kmpkgDisableMetrics="ON"
    elif [ "$var" = "-useSystemBinaries" -o "$var" = "--useSystemBinaries" ]; then
        echo "Warning: -useSystemBinaries no longer has any effect; ignored. Note that the KMPKG_USE_SYSTEM_BINARIES environment variable behavior is not changed."
    elif [ "$var" = "-allowAppleClang" -o "$var" = "--allowAppleClang" ]; then
        echo "Warning: -allowAppleClang no longer has any effect; ignored."
    elif [ "$var" = "-buildTests" ]; then
        echo "Warning: -buildTests no longer has any effect; ignored."
    elif [ "$var" = "-skipDependencyChecks" ]; then
        kmpkgSkipDependencyChecks="ON"
    elif [ "$var" = "-musl" ]; then
        kmpkgUseMuslC="ON"
    elif [ "$var" = "-help" -o "$var" = "--help" ]; then
        echo "Usage: ./bootstrap-kmpkg.sh [options]"
        echo
        echo "Options:"
        echo "    -help                 Display usage help"
        echo "    -disableMetrics       Mark this kmpkg root to disable metrics."
        echo "    -skipDependencyChecks Skip checks for kmpkg prerequisites. kmpkg may not run."
        echo "    -musl                 Use the musl binary rather than the glibc binary on Linux."
        exit 1
    else
        echo "Unknown argument $var. Use '-help' for help."
        exit 1
    fi
done

# Enable using this entry point on Windows from an msys2 or cygwin bash env. (e.g., git bash) by redirecting to the .bat file.
unixKernelName=$(uname -s | sed -E 's/(CYGWIN|MINGW|MSYS).*_NT.*/\1_NT/')
if [ "$unixKernelName" = CYGWIN_NT ] || [ "$unixKernelName" = MINGW_NT ] || [ "$unixKernelName" = MSYS_NT ]; then
    if [ "$kmpkgDisableMetrics" = "ON" ]; then
        args="-disableMetrics"
    else
        args=""
    fi

    kmpkgRootDir=$(cygpath -aw "$kmpkgRootDir")
    cmd "/C $kmpkgRootDir\\bootstrap-kmpkg.bat $args" || exit 1
    exit 0
fi

# Determine the downloads directory.
if [ -z ${KMPKG_DOWNLOADS+x} ]; then
    downloadsDir="$kmpkgRootDir/downloads"
else
    downloadsDir="$KMPKG_DOWNLOADS"
    if [ ! -d "$KMPKG_DOWNLOADS" ]; then
        echo "KMPKG_DOWNLOADS was set to '$KMPKG_DOWNLOADS', but that was not a directory."
        exit 1
    fi

fi

# Check for minimal prerequisites.
kmpkgCheckRepoTool()
{
    __tool=$1
    # Only perform dependency checks when they are not explicitly skipped.
    if [ "$kmpkgSkipDependencyChecks" = "OFF" ]; then
        if ! command -v "$__tool" >/dev/null 2>&1 ; then
            echo "Could not find $__tool. Please install it (and other dependencies) with:"
            echo "On Debian and Ubuntu derivatives:"
            echo "  sudo apt-get install curl zip unzip tar"
            echo "On recent Red Hat and Fedora derivatives:"
            echo "  sudo dnf install curl zip unzip tar"
            echo "On older Red Hat and Fedora derivatives:"
            echo "  sudo yum install curl zip unzip tar"
            echo "On SUSE Linux and derivatives:"
            echo "  sudo zypper install curl zip unzip tar"
            echo "On Arch Linux and derivatives:"
            echo "  sudo pacman -Syu base-devel git curl zip unzip tar cmake ninja"
            echo "On Alpine:"
            echo "  apk add build-base cmake ninja zip unzip curl git"
            echo "  (and export KMPKG_FORCE_SYSTEM_BINARIES=1)"
            echo "On Solaris and illumos distributions:"
            echo "  pkg install web/curl compress/zip compress/unzip"
            exit 1
        fi
    fi
}

kmpkgCheckRepoTool curl
kmpkgCheckRepoTool zip
kmpkgCheckRepoTool unzip
kmpkgCheckRepoTool tar

UNAME="$(uname)"
ARCH="$(uname -m)"

if [ -e /etc/alpine-release ]; then
    kmpkgUseSystem="ON"
    kmpkgUseMuslC="ON"
fi

if [ "$UNAME" = "OpenBSD" ]; then
    kmpkgUseSystem="ON"
fi

if [ "$kmpkgUseSystem" = "ON" ]; then
    kmpkgCheckRepoTool cmake
    kmpkgCheckRepoTool ninja
    kmpkgCheckRepoTool git
fi

kmpkgCheckEqualFileHash()
{
    url=$1; filePath=$2; expectedHash=$3

    if command -v "sha512sum" >/dev/null 2>&1 ; then
        actualHash=$(sha512sum "$filePath")
    elif command -v "sha512" >/dev/null 2>&1 ; then
        # OpenBSD
        actualHash=$(sha512 -q "$filePath")
    else
        # [g]sha512sum is not available by default on osx
        # shasum is not available by default on Fedora
        actualHash=$(shasum -a 512 "$filePath")
    fi

    actualHash="${actualHash%% *}" # shasum returns [hash filename], so get the first word

    if ! [ "$expectedHash" = "$actualHash" ]; then
        echo ""
        echo "File does not have expected hash:"
        echo "              url: [ $url ]"
        echo "        File path: [ $downloadPath ]"
        echo "    Expected hash: [ $sha512 ]"
        echo "      Actual hash: [ $actualHash ]"
        exit 1
    fi
}

kmpkgDownloadFile()
{
    url=$1; downloadPath=$2 sha512=$3
    rm -rf "$downloadPath.part"
    curl -L $url --tlsv1.2 --create-dirs --retry 3 --output "$downloadPath.part" --silent --show-error --fail || exit 1

    kmpkgCheckEqualFileHash $url "$downloadPath.part" $sha512
    chmod +x "$downloadPath.part"
    mv "$downloadPath.part" "$downloadPath"
}

kmpkgExtractArchive()
{
    archive=$1; toPath=$2
    rm -rf "$toPath" "$toPath.partial"
    case "$archive" in
        *.tar.gz)
            mkdir -p "$toPath.partial"
            $(cd "$toPath.partial" && tar xzf "$archive")
            ;;
        *.zip)
            unzip -qd "$toPath.partial" "$archive"
            ;;
    esac
    mv "$toPath.partial" "$toPath"
}

# Determine what we are going to do to bootstrap:
# MacOS -> Download kmpkg-macos
# Linux
#   useMuslC -> download kmpkg-muslc
#   amd64 -> download kmpkg-glibc
#   arm64 -> download kmpkg-glibc-arm64
# Otherwise
#   Download and build from source

# Read the kmpkg-tool config file to determine what release to download
. "$kmpkgRootDir/scripts/kmpkg-tool-metadata.txt"

kmpkgDownloadTool="ON"
if [ "$UNAME" = "Darwin" ]; then
    echo "Downloading kmpkg-macos..."
    kmpkgToolReleaseSha=$KMPKG_MACOS_SHA
    kmpkgToolName="kmpkg-macos"
elif [ "$UNAME" = "Linux" ] && [ "$kmpkgUseMuslC" = "ON" ] && [ "$ARCH" = "x86_64" ]; then
    echo "Downloading kmpkg-muslc..."
    kmpkgToolReleaseSha=$KMPKG_MUSLC_SHA
    kmpkgToolName="kmpkg-muslc"
elif [ "$UNAME" = "Linux" ] && [ "$ARCH" = "x86_64" ]; then
    echo "Downloading kmpkg-glibc..."
    kmpkgToolReleaseSha=$KMPKG_GLIBC_SHA
    kmpkgToolName="kmpkg-glibc"
elif [ "$UNAME" = "Linux" ] && [ "$kmpkgUseMuslC" = "OFF" ] && { [ "$ARCH" = "aarch64" ] || [ "$ARCH" = "arm64" ]; }; then
    echo "Downloading kmpkg-arm64-glibc..."
    kmpkgToolReleaseSha=$KMPKG_GLIBC_ARM64_SHA
    kmpkgToolName="kmpkg-glibc-arm64"
else
    echo "Unable to determine a binary release of kmpkg; attempting to build from source."
    kmpkgDownloadTool="OFF"
    kmpkgToolReleaseSha=$KMPKG_TOOL_SOURCE_SHA
fi

# Do the download or build.
if [ "$kmpkgDownloadTool" = "ON" ]; then
    kmpkgDownloadFile "https://github.com/microsoft/kmpkg-tool/releases/download/$KMPKG_TOOL_RELEASE_TAG/$kmpkgToolName" "$kmpkgRootDir/kmpkg" $kmpkgToolReleaseSha
else
    kmpkgToolReleaseArchive="$KMPKG_TOOL_RELEASE_TAG.zip"
    kmpkgToolUrl="https://github.com/microsoft/kmpkg-tool/archive/$kmpkgToolReleaseArchive"
    baseBuildDir="$kmpkgRootDir/buildtrees/_kmpkg"
    buildDir="$baseBuildDir/build"
    archivePath="$downloadsDir/$kmpkgToolReleaseArchive"
    srcBaseDir="$baseBuildDir/src"
    srcDir="$srcBaseDir/kmpkg-tool-$KMPKG_TOOL_RELEASE_TAG"

    if [ -e "$archivePath" ]; then
        kmpkgCheckEqualFileHash "$kmpkgToolUrl" "$archivePath" "$kmpkgToolReleaseSha"
    else
        echo "Downloading kmpkg tool sources"
        kmpkgDownloadFile "$kmpkgToolUrl" "$archivePath" "$kmpkgToolReleaseSha"
    fi

    echo "Building kmpkg-tool..."
    rm -rf "$baseBuildDir"
    mkdir -p "$buildDir"
    kmpkgExtractArchive "$archivePath" "$srcBaseDir"
    cmakeConfigOptions="-DCMAKE_BUILD_TYPE=Release -G 'Ninja' -DKMPKG_DEVELOPMENT_WARNINGS=OFF"

    if [ "${KMPKG_MAX_CONCURRENCY}" != "" ] ; then
        cmakeConfigOptions=" $cmakeConfigOptions '-DCMAKE_JOB_POOL_COMPILE:STRING=compile' '-DCMAKE_JOB_POOL_LINK:STRING=link' '-DCMAKE_JOB_POOLS:STRING=compile=$KMPKG_MAX_CONCURRENCY;link=$KMPKG_MAX_CONCURRENCY' "
    fi

    (cd "$buildDir" && eval cmake "$srcDir" $cmakeConfigOptions) || exit 1
    (cd "$buildDir" && cmake --build .) || exit 1

    rm -rf "$kmpkgRootDir/kmpkg"
    cp "$buildDir/kmpkg" "$kmpkgRootDir/"
fi

"$kmpkgRootDir/kmpkg" version --disable-metrics

# Apply the disable-metrics marker file.
if [ "$kmpkgDisableMetrics" = "ON" ]; then
    touch "$kmpkgRootDir/kmpkg.disable-metrics"
elif ! [ -f "$kmpkgRootDir/kmpkg.disable-metrics" ]; then
    # Note that we intentionally leave any existing kmpkg.disable-metrics; once a user has
    # opted out they should stay opted out.
    cat <<EOF
Telemetry
---------
kmpkg collects usage data in order to help us improve your experience.
The data collected by Microsoft is anonymous.
You can opt-out of telemetry by re-running the bootstrap-kmpkg script with -disableMetrics,
passing --disable-metrics to kmpkg on the command line,
or by setting the KMPKG_DISABLE_METRICS environment variable.

Read more about kmpkg telemetry at docs/about/privacy.md
EOF
fi
