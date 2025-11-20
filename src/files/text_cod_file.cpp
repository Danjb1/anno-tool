#include "files/text_cod_file.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include "files/file_utils.h"

namespace Anno {

/*
 * Helper methods
 */

// Used to transform from encoded `.cod` format to readable ANSI (and vice versa)
// See: https://github.com/Green-Sky/anno16_docs/blob/master/file_formats/encryption.md
static void transform_chars(std::vector<char>& buffer)
{
    for (int i = 0; i < buffer.size(); ++i)
    {
        buffer[i] = -buffer[i] & 0xff;
    }
}

static bool is_section_name(const std::string& line)
{
    return line.starts_with('[') && line.ends_with(']');
}

static std::string strip_enclosing_brackets(const std::string& line)
{
    return line.substr(1, line.size() - 2);
}

/*
 * TextCodFile class
 */

TextCodFile::TextCodFile(const std::filesystem::path& path)
{
    read_cod_file(path);
}

void TextCodFile::read_cod_file(const std::filesystem::path& path)
{
    // Read and decode the file
    std::vector<char> buffer = FileUtils::read_binary_file(path);
    transform_chars(buffer);

    // Split the file based on line breaks
    size_t start = 0;
    int current_section_index = -1;
    for (size_t i = 0; i < buffer.size() - 1; ++i)
    {
        // Search for a line break
        const bool is_eol = (buffer[i] == '\r' && buffer[i + 1] == '\n');
        if (!is_eol)
        {
            continue;
        }

        // We have found a line!
        // We subtract 1 from the length here to exclude the '\n' character.
        const size_t num_chars = i - start;
        std::string line(buffer.data() + start, num_chars);

        // Are we inside a section?
        if (current_section_index >= 0)
        {
            if (line == "[END]")
            {
                // End of section
                current_section_index = -1;
            }
            else
            {
                // Line found inside a section.
                // NOTE: Lines outside of a section are skipped (these are typically separators).
                auto& section = sections[current_section_index];
                section.lines.push_back(line);
            }
        }
        else if (is_section_name(line))
        {
            // New section
            std::string section_name = strip_enclosing_brackets(line);
            sections.emplace_back(section_name);
            current_section_index = static_cast<int>(sections.size()) - 1;
            section_map.emplace(section_name, current_section_index);
        }

        // Skip past the EOL characters to the next line
        start = i + 2;
    }

    // NOTE: If start < buffer.size() then it means the buffer ended with a non-terminated line which we did not
    // process. We ignore this for now as it doesn't matter in practice.
}

void TextCodFile::save_plain_text(const std::filesystem::path& path)
{
    std::vector<char> data = make_buffer(false);
    FileUtils::write_binary_file(path, data);
}

void TextCodFile::save_encoded(const std::filesystem::path& path)
{
    std::vector<char> data = make_buffer(true);
    FileUtils::write_binary_file(path, data);
}

std::vector<char> TextCodFile::make_buffer(bool should_encode_chars) const
{
    std::ostringstream oss;

    oss << "--------------------------------------------------\r\n";

    for (const auto& section : sections)
    {
        oss << "[" << section.name << "]\r\n";

        for (const auto& line : section.lines)
        {
            oss << line << "\r\n";
        }

        oss << "--------------------------------------------------\r\n";
    }

    std::string temp = oss.str();
    std::vector<char> data(temp.begin(), temp.end());

    if (should_encode_chars)
    {
        transform_chars(data);
    }

    return data;
}

std::vector<std::string> TextCodFile::get_section_contents(const std::string& section_name) const
{
    const auto it = section_map.find(section_name);
    if (it == section_map.cend())
    {
        // Section not found
        return {};
    }

    int section_index = it->second;
    return sections[section_index].lines;
}

}  // namespace Anno
