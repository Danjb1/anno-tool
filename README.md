# Anno Tool

A tool for parsing and editing the files of Anno 1602.

## Build

1. Install [CMake](https://cmake.org/) and [vcpkg](https://learn.microsoft.com/en-gb/vcpkg/get_started/get-started?pivots=shell-powershell#1---set-up-vcpkg).

1. Set the `VCPKG_ROOT` environment variable to your vcpkg root directory.

1. Configure the project:

    ```
    cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
    ```

1. Build using your desired IDE or by running:

    ```
    cmake --build build
    ```
