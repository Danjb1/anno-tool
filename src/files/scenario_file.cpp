#include "files/scenario_file.h"

#include <cstdint>

#include "files/file_utils.h"
#include "util/buffer_utils.h"

namespace Anno {

ScenarioFile::ScenarioFile(const std::filesystem::path& path)
    : file_data(FileUtils::read_binary_file(path))
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
    // Fortunately the campaign chunk is right at the start (if present), so we don't have to parse the whole file.
    // If we ever did want to parse the whole file, it would be best to do this separately on request, because it's
    // useful to be able to read the scenario "header" very quickly.
    // Note that values are stored as little-endian, so big-endian architectures would need to flip the bytes after
    // reading any values.
    if (is_campaign_chunk_present(file_data))
    {
        // Read the value directly after the chunk header
        std::memcpy(&campaign_index, &file_data[chunk_header_size], sizeof(int32_t));
    }
}

bool ScenarioFile::is_campaign_chunk_present(const std::vector<char>& data) const
{
    std::string_view view(data.data(), data.size());
    return view.substr(0, campaign_chunk_header.length()) == campaign_chunk_header;
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

    FileUtils::write_binary_file(path, file_data);
}

void ScenarioFile::update_data()
{
    const bool wants_campaign_chunk = (campaign_index >= 0);
    const bool has_campaign_chunk = is_campaign_chunk_present(file_data);

    if (wants_campaign_chunk)
    {
        if (has_campaign_chunk)
        {
            // Modify the existing campaign header
            std::memcpy(&file_data[chunk_header_size], &campaign_index, sizeof(int32_t));
        }
        else
        {
            // Add the campaign header
            file_data = prepend_campaign_chunk(file_data);
        }
    }
    else if (has_campaign_chunk)
    {
        // Remove the campaign header entirely
        file_data.erase(file_data.begin(), file_data.begin() + campaign_chunk_size);
    }
}

std::vector<char> ScenarioFile::prepend_campaign_chunk(const std::vector<char>& data) const
{
    std::vector<char> new_data;
    new_data.reserve(data.size() + campaign_chunk_size);

    BufferUtils::append(new_data, campaign_chunk_header);
    BufferUtils::append(new_data, int32_t { static_cast<std::int32_t>(/* chunk size (bytes) */ 4) });
    BufferUtils::append(new_data, int32_t { static_cast<std::int32_t>(campaign_index) });
    BufferUtils::append(new_data, data);

    return new_data;
}

}  // namespace Anno
