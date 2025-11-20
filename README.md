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

## Usage

### List Installed Campaigns

**Example:**

```
AnnoTool --anno-dir="C:/Anno 1602"
```


**Sample Output:**

```
Found Anno 1602 installation

Installed campaigns:

  New Horizons (Progress = 0)
    Halfway there
    To Each his Island
    Appearance can be deceiving
    Hard Times

  Trust no one (Progress = 0)
    Humility is a Virtue
    The Blinding
    The Thief

  The Magnate (Progress = 0)
    Gold Rush
    Spice Monopoly
    Dangerous waters

  Pirata (Progress = 0)
    Revenge is sweet
    The saviour
    Quest for peace

  Unfriendly neighbors (Progress = 0)
    Break the Monopoly
    The new Empire
    Imperial Proclamation

  On His Majesty's Service (Progress = 2)
    Veni, vidi, vici
    At all Costs
    The Deluge
    Close Quarters
```
