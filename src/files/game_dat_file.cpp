#include "files/game_dat_file.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "files/file_utils.h"

namespace Anno {

std::string bool_as_str(bool b)
{
    return b ? "TRUE" : "FALSE";
}

GameDatFile::GameDatFile(const std::filesystem::path& path, GameVersion gameVersion)
    : gameVersion(gameVersion)
{
    read_dat_file(path);
}

void GameDatFile::read_dat_file(const std::filesystem::path& path)
{
    std::vector<std::string> lines = FileUtils::read_text_file(path);

    for (int i = 0; i < lines.size(); ++i)
    {
        std::string line = boost::trim_copy(lines[i]);

        if (line.empty())
        {
            continue;
        }

        if (line.starts_with("Musik:"))
        {
            parse_music_setting(line);
        }
        if (line.starts_with("Samples:"))
        {
            is_sound_enabled = parse_bool_setting(line);
        }
        if (line.starts_with("Random:"))
        {
            is_music_shuffle_enabled = parse_bool_setting(line);
        }
        if (line.starts_with("Volume:"))
        {
            parse_volume_setting(line);
        }
        else if (line.starts_with("Song:"))
        {
            parse_disabled_music_track(line);
        }
        else if (line.starts_with("Speach:"))
        {
            parse_disabled_speech(line);
        }
        else if (line.starts_with("Video:"))
        {
            parse_disabled_video(line);
        }
        else if (line.starts_with("Lastfile:"))
        {
            parse_autosave(line);
        }
        else if (line.starts_with("Endlosnr:"))
        {
            parse_continuous_play_selection(line);
        }
        else if (line.starts_with("Tutornr:"))
        {
            parse_tutorial_selection(line);
        }
        else if (line.starts_with("Kampagne:"))
        {
            parse_campaign_progress(line);
        }
        else if (line.starts_with("Objekt:"))
        {
            parse_savegame_names(lines, i);
        }
    }
}

std::string extract_value(const std::string& line)
{
    std::vector<std::string> parts;
    boost::algorithm::split(parts, line, boost::is_any_of(":"));
    if (parts.size() < 2)
    {
        // No colon found in the input
        return "";
    }
    return boost::algorithm::trim_copy(parts[1]);
}

bool GameDatFile::parse_bool_setting(const std::string& line) const
{
    const std::string value = extract_value(line);
    return value == "TRUE";
}

void GameDatFile::parse_music_setting(const std::string& line)
{
    const std::string value = extract_value(line);
    if (value.empty())
    {
        // Invalid input string (expected a colon)
        return;
    }

    std::vector<std::string> parts;
    boost::algorithm::split(parts, value, boost::is_any_of(","));
    if (parts.size() < 2)
    {
        // Invalid input string (expected a comma)
        return;
    }

    is_music_enabled = (parts[0] == "TRUE");

    try
    {
        music_bitmask = std::stoi(parts[1]);
    }
    catch (const std::logic_error& err)
    {
        std::cerr << "Failed to parse music setting: " << err.what() << '\n';
    }
}

void GameDatFile::parse_volume_setting(const std::string& line)
{
    // TODO
}

void GameDatFile::parse_disabled_music_track(const std::string& line)
{
    // TODO
}

void GameDatFile::parse_disabled_speech(const std::string& line)
{
    // TODO
}

void GameDatFile::parse_disabled_video(const std::string& line)
{
    // TODO
}

void GameDatFile::parse_autosave(const std::string& line)
{
    // TODO
}

void GameDatFile::parse_continuous_play_selection(const std::string& line)
{
    // TODO
}

void GameDatFile::parse_tutorial_selection(const std::string& line)
{
    // TODO
}

void GameDatFile::parse_campaign_progress(const std::string& line)
{
    // TODO
}

void GameDatFile::parse_savegame_names(const std::vector<std::string>& lines, int& i)
{
    // TODO
}

/* clang-format off */
void GameDatFile::save(const std::filesystem::path& path)
{
    std::stringstream ss;

    // General settings
    ss << "\n"
       << "  Musik:     " << std::format("{}, {}\n", bool_as_str(is_music_enabled), music_bitmask)
       << "  Samples:   " << std::format("{}\n", bool_as_str(is_sound_enabled))
       << "  Random:    " << std::format("{}\n", bool_as_str(is_music_shuffle_enabled))
       << "  Volume:    " << std::format("{}, {}, {}\n", music_volume, sound_volume, main_game_progress)
       << "  VideoQual: TRUE\n\n";
 
    // Disabled music tracks
    for (int track_id : disabled_music_tracks)
    {
        ss << "  Song:     " << std::format("{}, FALSE\n", track_id);
    }
    ss << "  \n";
 
    // Disabled speech
    for (int i = 0; i < num_speech_categories; i++)
    {
        ss << "  Speach:    " << std::format("{}, {}\n", i, bool_as_str(disabled_speech[0]));
    }
    ss << "  \n";

    // Disabled videos
    for (int i = 0; i < num_speech_categories; i++)
    {
        ss << "  Video:     " << std::format("{}, {}\n", i, bool_as_str(disabled_speech[0]));
    }
    ss << "  \n";

    // Autosave file location
    ss << "  Lastfile: " << std::format("\"{}\"\n\n", last_save_file);

    // Continuous Play / Tutorial selection
    ss << "  Endlosnr: " << std::format("{}\n", continuous_play_selection)
       << "  Tutornr:  " << std::format("{}\n\n", tutorial_selection);

    // Campaign progress
    for (const auto& [campaign_index, campaign_progress] : campaign_progress)
    {
        ss << "  Kampagne: " << std::format("{}, {}\n", campaign_index, campaign_progress);
    }
    ss << "  \n";

    // Save game names (not present in the History Edition)
    if (gameVersion == GameVersion::Original)
    {
        ss << "  Objekt:   SPIELNAME\n\n";
        for (int i = 0; i < num_speech_categories; i++)
        {
            ss << "    Name:      " << std::format("{}, {}\n", i, savegame_names[i]);
        }
        ss << "  \n  EndObj;\n";
    }
    ss << "  \n";

    FileUtils::write_text_file(path, ss.str());
}
/* clang-format on */

}  // namespace Anno
