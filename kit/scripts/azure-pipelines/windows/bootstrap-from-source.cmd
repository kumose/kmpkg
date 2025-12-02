call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\VsDevCmd.bat" -arch=x86 -host_arch=x86
git clone --depth 1 https://github.com/microsoft/kmpkg-tool kmpkg-tool
git -C kmpkg-tool fetch --depth 1 origin %1
git -C kmpkg-tool switch -d FETCH_HEAD
rmdir /s /q build.x86.release > nul 2> nul
cmake.exe -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF -DKMPKG_DEVELOPMENT_WARNINGS=OFF -DKMPKG_WARNINGS_AS_ERRORS=OFF -DKMPKG_BUILD_FUZZING=OFF -DKMPKG_BUILD_TLS12_DOWNLOADER=OFF -B build.x86.release -S kmpkg-tool
ninja.exe -C build.x86.release
move build.x86.release\kmpkg.exe kmpkg.exe
