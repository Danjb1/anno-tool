#pragma once

#include <filesystem>
#include <string_view>
#include <vector>

namespace Anno {

/**
 * Class used for reading and writing to scenario (.szs) files.
 *
 * In particular we are interested in the `SZENE_KAMPAGNE` chunk.
 *
 * More info:
 * https://github.com/Green-Sky/anno16_docs/blob/master/file_formats/chunks.md
 * https://github.com/wzurborg/CmpInstall/blob/main/Scenario.cpp
 * https://github.com/siredmar/mdcii-engine/blob/master/source/mdcii/mdcii/src/gam/gam_parser.cpp
 */
class ScenarioFile
{
public:
    /** Creates a ScenarioFile by reading a file on disk.
     * May throw a std::ios_base::failure. */
    ScenarioFile(const std::filesystem::path& path);

    std::string get_filename() const;

    int get_campaign_index() const
    {
        return campaign_index;
    }

    void set_campaign_index(int new_campaign_index);

    void save_overwrite();
    void save_to_path(const std::filesystem::path& path);
    void update_data();

private:
    static constexpr std::string_view campaign_chunk_header = "SZENE_KAMPAGNE\0\0";
    static constexpr size_t campaign_chunk_size = 24;
    static constexpr size_t chunk_header_size = 20;

    void parse_scenario_data();
    bool is_campaign_chunk_present(const std::vector<char>& data) const;
    std::vector<char> prepend_campaign_chunk(const std::vector<char>& data) const;

    std::filesystem::path src_path;
    std::vector<char> file_data;
    int campaign_index = -1;
    bool is_dirty = false;
};

}  // namespace Anno
