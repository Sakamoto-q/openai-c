init:
	@if not exist "build" mkdir build
	cd build && cmake .. -DCMAKE_TOOLCHAIN_FILE="${vcpkgRoot}\scripts\buildsystems\vcpkg.cmake" -DCMAKE_PREFIX_PATH="${vcpkgRoot}\packages" -G "Visual Studio 17 2022" && chcp 65001

compile:
	cd build && cmake --build . --config Release

run:
	cd build\Release && test.exe