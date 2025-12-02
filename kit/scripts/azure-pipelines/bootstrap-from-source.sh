#!/bin/sh
set -e

git clone --depth 1 https://github.com/microsoft/kmpkg-tool kmpkg-tool
git -C kmpkg-tool fetch --depth 1 origin $1
git -C kmpkg-tool switch -d FETCH_HEAD
rm -rf build.x64.release
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF -DKMPKG_DEVELOPMENT_WARNINGS=OFF -DKMPKG_WARNINGS_AS_ERRORS=OFF -DKMPKG_BUILD_FUZZING=OFF -DKMPKG_BUILD_TLS12_DOWNLOADER=OFF -B build.x64.release -S kmpkg-tool
ninja -C build.x64.release
mv build.x64.release/kmpkg kmpkg
