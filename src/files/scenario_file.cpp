#include "files/scenario_file.h"

#include "files/file_utils.h"

namespace Anno {

ScenarioFile::ScenarioFile(const std::filesystem::path& path)
    : data(FileUtils::read_binary_file(path))
    , src_path(path)
{
    parse_scenario_data();
}

std::string ScenarioFile::get_filename() const
{
    return src_path.stem().string();
}

void ScenarioFile::parse_scenario_data()
{
    std::string_view view(data.data(), data.size());

    // Fortunately the campaign chunk is right at the start (if present), so we don't have to parse the whole file.
    // If we ever did want to parse the whole file, it would be best to do this separately on request, because it's
    // useful to be able to read the scenario "header" very quickly.
    // Note that values are stored as little-endian, so big-endian architectures would need to flip the bytes after
    // reading any values.
    if (view.substr(0, campaign_chunk_header.length()) == campaign_chunk_header)
    {
        // Read the value directly after the chunk header
        std::memcpy(&campaign_index, &data[chunk_header_size], sizeof(int32_t));
    }
}

void ScenarioFile::set_campaign_index(int new_campaign_index)
{
    if (campaign_index == new_campaign_index)
    {
        // No change
        return;
    }

    campaign_index = new_campaign_index;
    is_dirty = true;
}

void ScenarioFile::save_overwrite()
{
    save_to_path(src_path);
}

void ScenarioFile::save_to_path(const std::filesystem::path& path)
{
    if (is_dirty)
    {
        update_data();
        is_dirty = false;
    }

    FileUtils::write_binary_file(path, data);
}

void ScenarioFile::update_data()
{
    // TODO: remove campaign chunk if no longer needed
    // TODO: add campaign chunk if needed and not present
    // TODO: update campaign chunk if modified
}

}  // namespace Anno
