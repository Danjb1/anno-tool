#include <array>
#include <filesystem>
#include <map>
#include <vector>

#include "tool/config.h"

namespace Anno {

/**
 * Class used for reading and writing to `Game.dat`, which stores user save data and preferences.
 *
 * More info:
 * https://github.com/Green-Sky/anno16_docs/blob/master/files/game.dat.md
 */
class GameDatFile
{
    struct SaveSlot
    {
        std::string name;
        int num_players = 1;
    };

public:
    GameDatFile(const std::filesystem::path& path, GameVersion game_version);

    void save(const std::filesystem::path& path);

private:
    void read_dat_file(const std::filesystem::path& path);
    bool parse_bool_setting(const std::string& line) const;
    int parse_int_setting(const std::string& line, int default_value = 0) const;
    void parse_music_setting(const std::string& line);
    void parse_volume_setting(const std::string& line);
    void parse_disabled_music_track(const std::string& line);
    void parse_disabled_speech(const std::string& line);
    void parse_disabled_video(const std::string& line);
    void parse_autosave(const std::string& line);
    void parse_campaign_progress(const std::string& line);
    void parse_save_slots(const std::vector<std::string>& lines, int& i);

private:
    static constexpr int num_speech_categories = 8;
    static constexpr int num_video_categories = 8;
    static constexpr int num_savegames = 12;

    static constexpr int new_game_progress = -595;

    GameVersion game_version;

    // General settings
    int music_bitmask = 0;  // purpose unknown
    int music_volume = 0;
    int sound_volume = 0;
    int main_game_progress = new_game_progress;
    bool is_music_enabled = true;
    bool is_music_shuffle_enabled = true;
    bool is_sound_enabled = true;
    bool video_quality_flag = true;  // purpose unknown

    // Disabled music tracks
    std::vector<int> disabled_music_tracks;

    // Disabled speech / video (a value of true means disabled)
    std::array<bool, num_speech_categories> disabled_speech;
    std::array<bool, num_video_categories> disabled_videos;

    // Autosave file location
    std::string last_save_file;

    // Continuous Play / Tutorial selection
    int continuous_play_selection = 0;
    int tutorial_selection = 0;  // purpose unknown

    // Campaign progress
    std::map<int, int> campaign_progress;

    // Save slots
    std::array<SaveSlot, num_savegames> save_slots;
};

}  // namespace Anno
