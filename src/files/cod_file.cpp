#include "files/cod_file.h"

#include <fstream>
#include <stdexcept>

#include "files/file_utils.h"

namespace Anno {

// Used to transform from encoded `.cod` format to readable ANSI (and vice versa)
// See: https://github.com/Green-Sky/anno16_docs/blob/master/file_formats/encryption.md
void transform_chars(std::vector<char>& buffer)
{
    for (int i = 0; i < buffer.size(); ++i)
    {
        buffer[i] = -buffer[i] & 0xff;
    }
}

std::vector<char> read_cod_file(const std::filesystem::path& path)
{
    std::vector<char> buffer = FileUtils::read_binary_file(path);
    transform_chars(buffer);
    return buffer;
}

CodFile::CodFile(const std::filesystem::path& path)
    : data(read_cod_file(path))
{
}

void CodFile::save_plain_text(const std::filesystem::path& path)
{
    FileUtils::write_binary_file(path, data);
}

void CodFile::save_encoded(const std::filesystem::path& path)
{
    std::vector<char> cod_data = data;
    transform_chars(cod_data);
    FileUtils::write_binary_file(path, cod_data);
}

}  // namespace Anno
