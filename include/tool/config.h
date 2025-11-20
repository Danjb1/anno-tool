#pragma once

#include <cstdint>
#include <filesystem>

namespace Anno {

enum class GameVersion : std::uint8_t
{
    Original,
    HistoryEdition
};

struct Config
{
    /** Directory containing the game executable. */
    std::filesystem::path anno_dir;

    /** Directory containing `Game.dat`.
     * For the original game, this is the same as `anno_dir`.
     * For the History Edition, this is `%UserProfile%/Documents/Anno 1602 History Edition` (on Windows). */
    std::filesystem::path user_dir;

    GameVersion version = GameVersion::Original;
};

}  // namespace Anno
