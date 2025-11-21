# Anno Tool

A tool for parsing and editing the files of Anno 1602.

## 🔨 Build

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

## 🏃‍♂️ Run

### Display Help Text

#### Example

```bat
AnnoTool --help
```

#### Sample Output

```
Accepted options:

General options:
  --help                 produce help message
  --anno-dir arg         Anno 1602 directory

Instructions:
  --list-campaigns       list all installed campaigns
  --install-campaign     install a campaign using the supplied definition file
```

### List Installed Campaigns

This lists all campaigns that are currently installed.

#### Example

```bat
AnnoTool --anno-dir="C:/Anno 1602"
```

#### Sample Output

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

> **NOTE:** In the History Edition, localization keys may be returned instead of the campaign names, e.g. `[[103]]`.

### Install Campaigns

To install a campaign, first create a definition file, e.g. `From the Ashes.cmp` (the extension is not important). This should contain the desired level names, one per line:

```
Helping Neighbors
Salt and Pepper
Gold of Natives
```

The corresponding scenario files should be placed in the `Szenes` folder and named according to the campaign:

```
From the Ashes0.szs
From the Ashes1.szs
From the Ashes2.szs
```

The definition file can be safely deleted once the campaign is installed.

#### Example

```bat
AnnoTool --anno-dir="C:/Anno 1602" --install-campaign "From the Ashes.cmp"
```

#### Sample Output

```
Found Anno 1602 installation

Adding level names to text.cod...
Linking scenarios to campaign...
Adding entry to Game.dat...
Success!
```
