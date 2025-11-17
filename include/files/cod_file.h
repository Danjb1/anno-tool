#pragma once

#include <filesystem>
#include <vector>

namespace Anno {

/**
 * Class used for reading and writing `.cod` files, which are used to store encoded text-based data.
 *
 * More info:
 * https://github.com/Green-Sky/anno16_docs/blob/master/file_formats/script.md
 */
class CodFile
{
public:
    CodFile(const std::filesystem::path& path);

    const std::vector<char>* get_data() const
    {
        return &data;
    }

    void save_plain_text(const std::filesystem::path& path);
    void save_encoded(const std::filesystem::path& path);

private:
    std::vector<char> data;
};

}  // namespace Anno
