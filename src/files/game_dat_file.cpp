#include "files/game_dat_file.h"

#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/regex.hpp>

#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "files/file_utils.h"

namespace Anno {

/*
 * Helper methods
 */

std::string extract_value(const std::string& line)
{
    const auto split_pos = line.find(':');
    if (split_pos == std::string::npos)
    {
        // line does not contain a colon
        std::cerr << "Failed to extract value from line: " << line << "\n";
        return "";
    }

    std::string value = line.substr(split_pos + 1);
    boost::algorithm::trim(value);
    return value;
}

bool extract_multipart_value(const std::string& line, /* out */ std::vector<std::string>& parts, int expected_num_parts)
{
    const std::string value = extract_value(line);
    if (value.empty())
    {
        return false;
    }

    boost::algorithm::split_regex(parts, value, boost::regex(",\\s*"));
    if (parts.size() != expected_num_parts)
    {
        std::cerr << "Failed to find " << expected_num_parts << " values in line: " << line << "\n";
        return false;
    }

    return true;
}

void trim_quotes(std::string& line)
{
    boost::trim_if(line, boost::is_any_of("\""));
}

std::string bool_as_str(bool b)
{
    return b ? "TRUE" : "FALSE";
}

/*
 * GameDatFile class
 */

GameDatFile::GameDatFile(const std::filesystem::path& path, GameVersion game_version)
    : game_version(game_version)
{
    read_dat_file(path);
}

// TODO: We could use some better error handling here.
// For now, errors are generally logged to the console but otherwise ignored.
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
        else if (line.starts_with("Samples:"))
        {
            is_sound_enabled = parse_bool_setting(line);
        }
        else if (line.starts_with("Random:"))
        {
            is_music_shuffle_enabled = parse_bool_setting(line);
        }
        else if (line.starts_with("Volume:"))
        {
            parse_volume_setting(line);
        }
        else if (line.starts_with("VideoQual:"))
        {
            video_quality_flag = parse_bool_setting(line);
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
            continuous_play_selection = parse_int_setting(line);
        }
        else if (line.starts_with("Tutornr:"))
        {
            tutorial_selection = parse_int_setting(line);
        }
        else if (line.starts_with("Kampagne:"))
        {
            parse_campaign_progress(line);
        }
        else if (line.starts_with("Objekt:"))
        {
            parse_save_slots(lines, i);
        }
        else
        {
            std::cerr << "Encountered unexpected setting: " << line << "\n";
        }
    }
}

bool GameDatFile::parse_bool_setting(const std::string& line) const
{
    const std::string value = extract_value(line);
    return value == "TRUE";
}

int GameDatFile::parse_int_setting(const std::string& line, int default_value) const
{
    const std::string value = extract_value(line);

    try
    {
        return std::stoi(value);
    }
    catch (const std::logic_error& err)
    {
        std::cerr << "Failed to parse int value: " << err.what() << '\n'  //
                  << " in line: " << line << "\n";
        return default_value;
    }
}

void GameDatFile::parse_music_setting(const std::string& line)
{
    std::vector<std::string> parts;
    if (!extract_multipart_value(line, /* out */ parts, 2))
    {
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
    std::vector<std::string> parts;
    if (!extract_multipart_value(line, /* out */ parts, 3))
    {
        return;
    }

    try
    {
        music_volume = std::stoi(parts[0]);
        sound_volume = std::stoi(parts[1]);

        // For some reason this is grouped with the volume settings
        main_game_progress = std::stoi(parts[2]);
    }
    catch (const std::logic_error& err)
    {
        std::cerr << "Failed to parse music setting: " << err.what() << '\n';
    }
}

void GameDatFile::parse_disabled_music_track(const std::string& line)
{
    std::vector<std::string> parts;
    if (!extract_multipart_value(line, /* out */ parts, 2))
    {
        return;
    }

    try
    {
        int track_index = std::stoi(parts[0]);
        disabled_music_tracks.push_back(track_index);
    }
    catch (const std::logic_error& err)
    {
        std::cerr << "Failed to parse disabled music track: " << err.what() << '\n';
    }
}

void GameDatFile::parse_disabled_speech(const std::string& line)
{
    std::vector<std::string> parts;
    if (!extract_multipart_value(line, /* out */ parts, 2))
    {
        return;
    }

    try
    {
        int index = std::stoi(parts[0]);
        bool is_disabled = (parts[1] == "TRUE");
        disabled_speech[index] = is_disabled;
    }
    catch (const std::logic_error& err)
    {
        std::cerr << "Failed to parse disabled music track: " << err.what() << '\n';
    }
}

void GameDatFile::parse_disabled_video(const std::string& line)
{
    std::vector<std::string> parts;
    if (!extract_multipart_value(line, /* out */ parts, 2))
    {
        return;
    }

    try
    {
        int index = std::stoi(parts[0]);
        bool is_disabled = (parts[1] == "TRUE");
        disabled_videos[index] = is_disabled;
    }
    catch (const std::logic_error& err)
    {
        std::cerr << "Failed to parse disabled video: " << err.what() << '\n';
    }
}

void GameDatFile::parse_autosave(const std::string& line)
{
    last_save_file = extract_value(line);
    trim_quotes(last_save_file);
}

void GameDatFile::parse_campaign_progress(const std::string& line)
{
    std::vector<std::string> parts;
    if (!extract_multipart_value(line, /* out */ parts, 2))
    {
        return;
    }

    try
    {
        int index = std::stoi(parts[0]);
        int progress = std::stoi(parts[1]);
        campaign_progress[index] = progress;
    }
    catch (const std::logic_error& err)
    {
        std::cerr << "Failed to parse campaign progress: " << err.what() << '\n';
    }
}

void GameDatFile::parse_save_slots(const std::vector<std::string>& lines, int& i)
{
    // Skip over the "Objekt" line
    ++i;

    // Parse the section line-by-line
    for (; i < lines.size(); ++i)
    {
        const std::string line = boost::trim_copy(lines[i]);

        if (line.empty())
        {
            continue;
        }

        if (line == "EndObj;")
        {
            // End of section
            break;
        }

        std::vector<std::string> parts;
        if (!extract_multipart_value(line, /* out */ parts, 3))
        {
            continue;
        }

        try
        {
            int index = std::stoi(parts[0]);
            std::string name = parts[1];
            trim_quotes(name);
            int num_players = std::stoi(parts[2]);

            save_slots[index] = { name, num_players };
        }
        catch (const std::logic_error& err)
        {
            std::cerr << "Failed to parse campaign progress: " << err.what() << '\n';
        }
    }
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
       << "  VideoQual: " << std::format("{}\n\n", bool_as_str(video_quality_flag));
 
    // Disabled music tracks
    for (int track_id : disabled_music_tracks)
    {
        ss << "  Song:     " << std::format("{}, FALSE\n", track_id);
    }
    ss << "  \n";
 
    // Disabled speech
    for (int i = 0; i < num_speech_categories; i++)
    {
        ss << "  Speach:    " << std::format("{}, {}\n", i, bool_as_str(disabled_speech[i]));
    }
    ss << "  \n";

    // Disabled videos
    for (int i = 0; i < num_video_categories; i++)
    {
        ss << "  Video:     " << std::format("{}, {}\n", i, bool_as_str(disabled_videos[i]));
    }
    ss << "  \n";

    // Autosave file location
    ss << "  Lastfile: " << std::format("\"{}\"\n\n", last_save_file);

    // Continuous Play / Tutorial selection
    ss << "  Endlosnr: " << std::format("{}\n", continuous_play_selection)
       << "  Tutornr:  " << std::format("{}\n\n", tutorial_selection);

    // Campaign progress
    for (const auto& [campaign_index, progress] : campaign_progress)
    {
        ss << "  Kampagne: " << std::format("{}, {}\n", campaign_index, progress);
    }
    ss << "  \n";

    // Save game names (not present in the History Edition)
    if (game_version == GameVersion::Original)
    {
        ss << "  Objekt:   SPIELNAME\n\n";
        for (int i = 0; i < num_savegames; i++)
        {
            const auto& save_slot = save_slots[i];
            // TODO: spaces should shrink for double digit indices
            ss << "    Name:      " << std::format("{}, \"{}\", {}\n", i, save_slot.name, save_slot.num_players);
        }
        ss << "  \n  EndObj;\n\n";
    }

    FileUtils::write_text_file(path, ss.str());
}
/* clang-format on */

}  // namespace Anno
