option(KMPKG_DEPENDENCY_CMAKERC "CMake-based C++ resource compiler" OFF)

if(KMPKG_DEPENDENCY_CMAKERC)
    find_package(CMakeRC CONFIG REQUIRED)
    return()
endif()

# This option exists to allow the URI to be replaced with a Microsoft-internal URI in official
# builds which have restricted internet access; see azure-pipelines/signing.yml
# Note that the SHA512 is the same, so kmpkg-tool contributors need not be concerned that we built
# with different content.
set(KMPKG_CMAKERC_URL "https://github.com/vector-of-bool/cmrc/archive/refs/tags/2.0.1.tar.gz" CACHE STRING "URL to the cmrc release tarball to use.")

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)
find_package(Git REQUIRED)
set(CMAKE_SKIP_INSTALL_RULES TRUE CACHE BOOL "" FORCE)
FetchContent_Declare(
    CMakeRC
    URL "${KMPKG_CMAKERC_URL}"
    URL_HASH "SHA512=cb69ff4545065a1a89e3a966e931a58c3f07d468d88ecec8f00da9e6ce3768a41735a46fc71af56e0753926371d3ca5e7a3f2221211b4b1cf634df860c2c997f"
    PATCH_COMMAND "${GIT_EXECUTABLE}" "--work-tree=." apply "${CMAKE_CURRENT_LIST_DIR}/CMakeRC_cmake_4.patch"
)
FetchContent_MakeAvailable(CMakeRC)
set(CMAKE_SKIP_INSTALL_RULES FALSE CACHE BOOL "" FORCE)
if(NOT CMakeRC_FIND_REQUIRED)
    message(FATAL_ERROR "CMakeRC must be REQUIRED")
endif()
