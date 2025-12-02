cmake_minimum_required(VERSION 3.15...3.24)

if(NOT CMAKE_MSVC_RUNTIME_LIBRARY)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

# ===============
# === Options ===
# ===============

include(CMakeDependentOption)

option(BUILD_TESTING "Option for enabling testing" ON)
option(KMPKG_DEVELOPMENT_WARNINGS "Option for turning on all warnings" ON)
option(KMPKG_MSVC_ANALYZE "Option for turning on /analyze" ON)
option(KMPKG_WARNINGS_AS_ERRORS "Set warnings to be errors" OFF)
option(KMPKG_BUILD_TLS12_DOWNLOADER "Enable building the tls12-downloader" OFF)
option(KMPKG_BUILD_FUZZING "Option for enabling kmpkg-fuzz support" OFF)
option(KMPKG_EMBED_GIT_SHA "Option for to fill in the Git SHA version; off by default to avoid privacy concerns out of official builds" OFF)
option(KMPKG_ADD_SOURCELINK "Option for enabling SourceLink in debug information on Windows/MSVC builds" "${KMPKG_EMBED_GIT_SHA}")
option(KMPKG_OFFICIAL_BUILD "Option to cause immediate failure if variables required for official builds are unset." OFF)
set(KMPKG_PDB_SUFFIX "" CACHE STRING "Append this string to the name of the PDB for shipping kmpkg binaries.")

CMAKE_DEPENDENT_OPTION(KMPKG_BUILD_BENCHMARKING "Option for enabling benchmarking" OFF
    "BUILD_TESTING" OFF)

if(DEFINE_DISABLE_METRICS OR KMPKG_DISABLE_METRICS)
    message(DEPRECATION "DEFINE_DISABLE_METRICS / KMPKG_DISABLE_METRICS are now handled by creating a "
        "file kmpkg.disable_metrics next to the binary.")
endif()

project(kmpkg
    DESCRIPTION "kmpkg helps you manage C and C++ libraries on Windows, Linux and MacOS."
    HOMEPAGE_URL "https://github.com/kumose/kmpkg"
    LANGUAGES C CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(cmake/utilities.cmake)

# ========================
# === System detection ===
# ========================

kmpkg_detect_compiler()

if(MSVC AND NOT COMMAND target_precompile_headers)
    message(FATAL_ERROR "CMake 3.16 (target_precompile_headers) is required to build with MSVC")
endif()

if(KMPKG_EMBED_GIT_SHA)
    if(DEFINED KMPKG_VERSION)
        message(STATUS "Using supplied version SHA ${KMPKG_VERSION}.")
    else()
        find_package(Git REQUIRED)
        execute_process(
            COMMAND "${GIT_EXECUTABLE}" status --porcelain=v1
            WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
            OUTPUT_VARIABLE KMPKG_GIT_STATUS
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )

        if(KMPKG_GIT_STATUS STREQUAL "")
            execute_process(
                COMMAND "${GIT_EXECUTABLE}" rev-parse HEAD
                WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
                OUTPUT_VARIABLE KMPKG_VERSION
                OUTPUT_STRIP_TRAILING_WHITESPACE
            )
        else()
            message(WARNING "Skipping embedding SHA due to local changes.")
        endif()
    endif()
endif()

if(NOT DEFINED KMPKG_VERSION OR KMPKG_VERSION STREQUAL "")
    set(KMPKG_VERSION "unknownhash")
endif()

if(NOT DEFINED KMPKG_BASE_VERSION OR KMPKG_BASE_VERSION STREQUAL "")
    if(KMPKG_OFFICIAL_BUILD)
        message(FATAL_ERROR "KMPKG_BASE_VERSION must be set for official builds.")
    endif()

    # The first digit is 2 to work with kmpkg_minimum_required in scripts.
    set(KMPKG_BASE_VERSION "2991-12-31")
endif()

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
if(MSVC)
    # either MSVC, or clang-cl
    string(APPEND CMAKE_C_FLAGS " -FC -permissive- -utf-8 /guard:cf")
    string(APPEND CMAKE_CXX_FLAGS " /EHsc -FC -permissive- -utf-8 /guard:cf")
    string(APPEND CMAKE_C_FLAGS_RELEASE " /Zi /GL")
    string(APPEND CMAKE_CXX_FLAGS_RELEASE " /Zi /GL")

    string(APPEND CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO " /DEBUG /INCREMENTAL:NO /debugtype:cv,fixup /guard:cf /OPT:REF /OPT:ICF /LTCG")
    string(APPEND CMAKE_EXE_LINKER_FLAGS_RELEASE " /DEBUG /INCREMENTAL:NO /debugtype:cv,fixup /guard:cf /OPT:REF /OPT:ICF /LTCG")
    if (MSVC_CXX_ARCHITECTURE_ID STREQUAL "x64")
        string(APPEND CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO " /CETCOMPAT")
        string(APPEND CMAKE_EXE_LINKER_FLAGS_RELEASE " /CETCOMPAT")
    endif()

    # Avoid CMake's default taking of the pretty names
    string(REPLACE "/DAMD64" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    string(REPLACE "/DAMD64" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REPLACE "/DARM64EC" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    string(REPLACE "/DARM64EC" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

    if(KMPKG_DEVELOPMENT_WARNINGS)
        string(APPEND CMAKE_C_FLAGS " /W4 /sdl")
        string(APPEND CMAKE_CXX_FLAGS " /W4 /sdl")
        if(KMPKG_COMPILER STREQUAL "clang")
            string(APPEND CMAKE_C_FLAGS " -Wmissing-prototypes -Wno-missing-field-initializers")
            string(APPEND CMAKE_CXX_FLAGS " -Wmissing-prototypes -Wno-missing-field-initializers")
        elseif(KMPKG_MSVC_ANALYZE)
            # -wd6553 is to workaround a violation in the Windows SDK
            # c:\program files (x86)\windows kits\10\include\10.0.22000.0\um\winreg.h(780) : warning C6553: The annotation for function 'RegOpenKeyExW' on _Param_(3) does not apply to a value type.
            string(APPEND CMAKE_C_FLAGS " -analyze -analyze:stacksize 39000 -wd6553")
            string(APPEND CMAKE_CXX_FLAGS " -analyze -analyze:stacksize 39000 -wd6553")
        endif()
    endif()

    if(KMPKG_WARNINGS_AS_ERRORS)
        string(APPEND CMAKE_C_FLAGS " /WX")
        string(APPEND CMAKE_CXX_FLAGS " /WX")
    endif()
else()
    # Neither MSVC nor clang-cl
    if(KMPKG_DEVELOPMENT_WARNINGS)
        # GCC and clang have different names for the same warning
        if(KMPKG_COMPILER STREQUAL "gcc")
            set(DECL_WARNING "-Wmissing-declarations")
        elseif(KMPKG_COMPILER STREQUAL "clang")
            set(DECL_WARNING "-Wmissing-prototypes -Wno-range-loop-analysis")
        endif()

        string(APPEND CMAKE_C_FLAGS " -Wall -Wextra -Wpedantic -Wno-unknown-pragmas -Wno-missing-field-initializers ${DECL_WARNING}")
        string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -Wpedantic -Wno-unknown-pragmas -Wno-missing-field-initializers -Wno-redundant-move ${DECL_WARNING}")

        if(KMPKG_COMPILER STREQUAL "gcc" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 13 AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 14)
            # -Wno-dangling-reference results from https://gcc.gnu.org/bugzilla/show_bug.cgi?id=107532 :(
            string(APPEND CMAKE_CXX_FLAGS " -Wno-dangling-reference")
        endif()
    endif()

    if(KMPKG_WARNINGS_AS_ERRORS)
        string(APPEND CMAKE_C_FLAGS " -Werror")
        string(APPEND CMAKE_CXX_FLAGS " -Werror")
    endif()
endif()

if(APPLE)
    set(CMAKE_C_ARCHIVE_CREATE   "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
    set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
    set(CMAKE_C_ARCHIVE_FINISH   "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
    set(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
endif()

# =======================
# === Paths detection ===
# =======================

include(GNUInstallDirs)

file(GLOB KMPKGLIB_BASE_SOURCES CONFIGURE_DEPENDS "src/kmpkg/base/*.cpp")
file(GLOB KMPKGLIB_SOURCES CONFIGURE_DEPENDS "src/kmpkg/*.cpp")
file(GLOB KMPKGLIB_BASE_INCLUDES CONFIGURE_DEPENDS "include/kmpkg/base/*.h" "include/kmpkg/base/fwd/*.h")
file(GLOB KMPKGLIB_INCLUDES CONFIGURE_DEPENDS "include/kmpkg/*.h" "include/kmpkg/fwd/*.h")

set(KMPKG_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/kmpkg.cpp")

file(GLOB KMPKG_TEST_SOURCES CONFIGURE_DEPENDS "src/kmpkg-test/*.cpp")
file(GLOB KMPKG_TEST_INCLUDES CONFIGURE_DEPENDS "include/kmpkg-test/*.h")

set(KMPKG_FUZZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/kmpkg-fuzz/main.cpp")
set(TLS12_DOWNLOAD_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/tls12-download.c")
set(CLOSES_EXIT_MINUS_ONE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/closes-exit-minus-one.c")
set(CLOSES_STDIN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/closes-stdin.c")
set(CLOSES_STDOUT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/closes-stdout.c")
set(READS_STDIN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/reads-stdin.c")
set(TEST_EDITOR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/test-editor.c")
set(TEST_SCRIPT_ASSET_CACHE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/test-script-asset-cache.c")

# ====================
# === Dependencies ===
# ====================
set(CMAKE_SKIP_INSTALL_RULES TRUE CACHE BOOL "" FORCE)
add_subdirectory(fmt)
add_subdirectory(cmrc)
set(CMAKE_SKIP_INSTALL_RULES FALSE CACHE BOOL "" FORCE)

# === Target: locale-resources ===

file(GLOB LOCALE_RESOURCES CMAKE_CONFIGURE_DEPENDS "locales/*.json")
cmrc_add_resource_library(locale-resources ALIAS cmakerc::locales NAMESPACE cmakerc ${LOCALE_RESOURCES})

if(NOT MSVC)
    if(KMPKG_COMPILER STREQUAL "gcc")
        target_compile_options(locale-resources PRIVATE
            -Wno-missing-declarations
            )
    elseif(KMPKG_COMPILER STREQUAL "clang")
        target_compile_options(locale-resources PRIVATE
            -Wno-missing-prototypes
            )
    endif()
endif()

# === Target: kmpkglib ===

add_library(kmpkglib OBJECT
    ${KMPKGLIB_BASE_SOURCES}
    ${KMPKGLIB_SOURCES}
    ${KMPKGLIB_BASE_INCLUDES}
    ${KMPKGLIB_INCLUDES}
)
target_include_directories(kmpkglib PUBLIC include)

target_compile_definitions(kmpkglib PUBLIC
    KMPKG_VERSION=${KMPKG_VERSION}
    KMPKG_BASE_VERSION=${KMPKG_BASE_VERSION}
    _FILE_OFFSET_BITS=64
)

if(KMPKG_STANDALONE_BUNDLE_SHA)
    target_compile_definitions(kmpkglib PUBLIC
        "KMPKG_STANDALONE_BUNDLE_SHA=${KMPKG_STANDALONE_BUNDLE_SHA}"
    )
elseif(KMPKG_OFFICIAL_BUILD)
    message(FATAL_ERROR "KMPKG_STANDALONE_BUNDLE_SHA is required for official builds.")
endif()

if(KMPKG_ARTIFACTS_SHA)
    target_compile_definitions(kmpkglib PUBLIC
        "KMPKG_ARTIFACTS_SHA=${KMPKG_ARTIFACTS_SHA}"
    )
elseif(KMPKG_OFFICIAL_BUILD)
    message(FATAL_ERROR "KMPKG_ARTIFACTS_SHA is required for official builds.")
endif()


set(CPP_ATOMIC_LIBRARY "")
include(CheckCXXSourceCompiles)
if(NOT MSVC)
    # Some platforms require a explicit linkage against libatomic to operate on 64-bit numbers
    set(TEST_SOURCE "
#include <stdint.h>
#include <atomic>
std::atomic<uintptr_t> x;
std::atomic<uintmax_t> y;
int main() {
    return x + y;
}
")
    check_cxx_source_compiles("${TEST_SOURCE}" CPP_ATOMIC_BUILTIN)
    if(NOT CPP_ATOMIC_BUILTIN)
        list(APPEND CMAKE_REQUIRED_LIBRARIES atomic)
        set(CPP_ATOMIC_LIBRARY atomic)
        check_cxx_source_compiles("${TEST_SOURCE}" CPP_ATOMIC_WITH_LIBATOMIC)
        if (NOT CPP_ATOMIC_WITH_LIBATOMIC)
            message(
                FATAL_ERROR "unable to link C++ std::atomic code: you may need \
                to install GNU libatomic"
            )
        endif()
    endif()
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(kmpkglib
    PUBLIC
        fmt::fmt
        cmakerc::locales
        Threads::Threads
    PRIVATE
        ${CPP_ATOMIC_LIBRARY}
)

if(CMAKE_VERSION GREATER_EQUAL "3.16")
    target_precompile_headers(kmpkglib PRIVATE "include/pch.h")
elseif(NOT MSVC)
    target_compile_options(kmpkglib PRIVATE -include "${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
    target_link_libraries(kmpkglib PRIVATE socket)
endif()

if(ANDROID AND CMAKE_SYSTEM_VERSION LESS "28")
    # pkg install libandroid-spawn
    target_link_libraries(kmpkglib PRIVATE android-spawn)
endif()

if(MINGW)
    target_compile_definitions(kmpkglib
        PUBLIC
            UNICODE
            _WIN32_WINNT=0x0601
            WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY=4
            __fastfail=exit
    )
    target_link_libraries(kmpkglib PUBLIC winhttp bcrypt version ole32 uuid)
endif()

# === Target: kmpkg-ps1 ===
add_custom_command(
    OUTPUT
        "${CMAKE_CURRENT_BINARY_DIR}/kmpkg-shell.ps1"
    COMMAND
        "${CMAKE_COMMAND}" ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/src/kmpkg-in-development.ps1" "${CMAKE_CURRENT_BINARY_DIR}/kmpkg-shell.ps1"
    DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/kmpkg-in-development.ps1"
    VERBATIM
)
add_custom_target(kmpkg-ps1 ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/kmpkg-shell.ps1")

# === Target: kmpkg ===

add_executable(kmpkg ${KMPKG_SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/src/kmpkg.manifest")
target_link_libraries(kmpkg PRIVATE kmpkglib ${CMAKE_DL_LIBS})

if(KMPKG_ADD_SOURCELINK)
    if(KMPKG_VERSION STREQUAL "unknownhash")
        message(FATAL_ERROR "Attempted to add source link information, but there was no git SHA defined. KMPKG_ADD_SOURCELINK only works if KMPKG_EMBED_GIT_SHA is set.")
    endif()
    kmpkg_target_add_sourcelink(kmpkg
        REPO "kumose/kmpkg"
        REF "${KMPKG_VERSION}"
    )
endif()

set_property(TARGET kmpkg PROPERTY PDB_NAME "kmpkg${KMPKG_PDB_SUFFIX}")


# === Target: kmpkg-test ===

if (BUILD_TESTING)
    enable_testing()

    add_executable(kmpkg-test
        ${KMPKG_TEST_SOURCES}
        ${KMPKG_TEST_INCLUDES}
        "${CMAKE_CURRENT_SOURCE_DIR}/src/kmpkg.manifest"
    )
    target_link_libraries(kmpkg-test PRIVATE kmpkglib)
    set_property(TARGET kmpkg-test PROPERTY PDB_NAME "kmpkg-test${KMPKG_PDB_SUFFIX}")
    if(ANDROID)
        target_link_libraries(kmpkg-test PRIVATE log)
    endif()

    add_dependencies(kmpkg-test reads-stdin closes-exit-minus-one closes-stdin closes-stdout test-editor)

    if(CMAKE_VERSION GREATER_EQUAL "3.16")
        target_precompile_headers(kmpkg-test REUSE_FROM kmpkglib)
    elseif(NOT MSVC)
       target_compile_options(kmpkg-test PRIVATE -include "${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h")
    endif()

    add_test(NAME kmpkg-test COMMAND "${CMAKE_COMMAND}" -E env LC_ALL=C "$<TARGET_FILE:kmpkg-test>" --order rand --rng-seed time)
    if(KMPKG_BUILD_BENCHMARKING)
        target_compile_options(kmpkg-test PRIVATE -DCATCH_CONFIG_ENABLE_BENCHMARKING)
    endif()
endif()

# === Target: kmpkg-fuzz ===
if(KMPKG_BUILD_FUZZING)
    add_executable(kmpkg-fuzz ${KMPKG_FUZZ_SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/src/kmpkg.manifest")
    target_link_libraries(kmpkg-fuzz PRIVATE kmpkglib)
    set_property(TARGET kmpkg-fuzz PROPERTY PDB_NAME "kmpkg-fuzz${KMPKG_PDB_SUFFIX}")
endif()


# === Target: tls12-download ===

if(KMPKG_BUILD_TLS12_DOWNLOADER)
    add_executable(tls12-download ${TLS12_DOWNLOAD_SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/src/kmpkg.manifest")
    set_property(TARGET tls12-download PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
    set_property(TARGET tls12-download APPEND PROPERTY LINK_OPTIONS "$<IF:$<CONFIG:Debug>,,/ENTRY:entry>")
    target_link_libraries(tls12-download winhttp wintrust shell32)
    set_property(TARGET tls12-download PROPERTY PDB_NAME "tls12-download${KMPKG_PDB_SUFFIX}")
endif()

if (BUILD_TESTING)
# === Target: closes-exit-minus-one ===

add_executable(closes-exit-minus-one ${CLOSES_EXIT_MINUS_ONE_SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/src/kmpkg.manifest")
set_property(TARGET closes-exit-minus-one PROPERTY PDB_NAME "closes-exit-minus-one${KMPKG_PDB_SUFFIX}")

# === Target: closes-stdin ===

add_executable(closes-stdin ${CLOSES_STDIN_SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/src/kmpkg.manifest")
set_property(TARGET closes-stdin PROPERTY PDB_NAME "closes-stdin${KMPKG_PDB_SUFFIX}")

# === Target: closes-stdout ===

add_executable(closes-stdout ${CLOSES_STDOUT_SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/src/kmpkg.manifest")
set_property(TARGET closes-stdout PROPERTY PDB_NAME "closes-stdout${KMPKG_PDB_SUFFIX}")

# === Target: reads-stdin ===

add_executable(reads-stdin ${READS_STDIN_SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/src/kmpkg.manifest")
set_property(TARGET reads-stdin PROPERTY PDB_NAME "reads-stdin${KMPKG_PDB_SUFFIX}")

# === Target: test-editor ===

add_executable(test-editor ${TEST_EDITOR_SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/src/kmpkg.manifest")
set_property(TARGET test-editor PROPERTY PDB_NAME "test-editor${KMPKG_PDB_SUFFIX}")

# === Target: test-editor ===

add_executable(test-script-asset-cache ${TEST_SCRIPT_ASSET_CACHE_SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/src/kmpkg.manifest")
set_property(TARGET test-script-asset-cache PROPERTY PDB_NAME "test-script-asset-cache${KMPKG_PDB_SUFFIX}")
endif()

# === Target: verify_messages ===
add_custom_target(
    verify-messages
    COMMAND pwsh -File "${CMAKE_CURRENT_SOURCE_DIR}/scripts/verifyMessages.ps1"
    COMMENT "Running PowerShell script to verify message usage..."
)

# === Target: format ===

find_program(CLANG_FORMAT clang-format PATHS "$ENV{PROGRAMFILES}/LLVM/bin")
if(CLANG_FORMAT)
    # doing all of these formats in one line has a tendency to overflow the command line length
    add_custom_target(format
        COMMAND "${CLANG_FORMAT}" -i -verbose ${KMPKGLIB_BASE_SOURCES}
        COMMAND "${CLANG_FORMAT}" -i -verbose ${KMPKGLIB_SOURCES}
        COMMAND "${CLANG_FORMAT}" -i -verbose ${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h
        COMMAND "${CLANG_FORMAT}" -i -verbose ${KMPKGLIB_BASE_INCLUDES}
        COMMAND "${CLANG_FORMAT}" -i -verbose ${KMPKGLIB_INCLUDES}

        COMMAND "${CLANG_FORMAT}" -i -verbose ${KMPKG_SOURCES}

        COMMAND "${CLANG_FORMAT}" -i -verbose ${KMPKG_TEST_SOURCES}
        COMMAND "${CLANG_FORMAT}" -i -verbose ${KMPKG_TEST_INCLUDES}

        COMMAND "${CLANG_FORMAT}" -i -verbose ${KMPKG_FUZZ_SOURCES} ${TLS12_DOWNLOAD_SOURCES}
            ${CLOSES_STDIN_SOURCES} ${CLOSES_STDOUT_SOURCES} ${READS_STDIN_SOURCES} ${CLOSES_EXIT_MINUS_ONE_SOURCES}
            ${TEST_EDITOR_SOURCES} ${TEST_SCRIPT_ASSET_CACHE_SOURCES}
    )
endif()

# ====================
# === Installation ===
# ====================

install(TARGETS kmpkg
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    BUNDLE DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

install(DIRECTORY kit DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(FILES kmpkg.toml DESTINATION "${CMAKE_INSTALL_BINDIR}")