#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <vector>

namespace Anno {

/**
 * Class used for reading and writing `.cod` files, which are used to store encoded text-based data.
 *
 * More info:
 * https://github.com/Green-Sky/anno16_docs/blob/master/file_formats/text.md
 */
class TextCodFile
{
    struct TextCodSection
    {
        TextCodSection(const std::string& name)
            : name(name)
        {
        }

        std::string name;
        std::vector<std::string> lines;
    };

public:
    /** Creates a TextCodFile by reading a file on disk.
     * May throw a std::ios_base::failure. */
    TextCodFile(const std::filesystem::path& path);

    void save_plain_text(const std::filesystem::path& path);
    void save_encoded(const std::filesystem::path& path);

    std::vector<std::string> get_section_contents(const std::string& section_name) const;

private:
    void read_cod_file(const std::filesystem::path& path);
    std::vector<char> make_buffer(bool should_encode_chars) const;

    static constexpr size_t line_buffer_size = 128;

    std::vector<TextCodSection> sections;
    std::map<std::string, int> section_map;
};

}  // namespace Anno
