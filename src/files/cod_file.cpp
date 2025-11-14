#include "files/cod_file.h"

#include <fstream>
#include <stdexcept>

namespace Anno {

// TODO: Move to utils
std::vector<char> read_entire_file(const std::filesystem::path& path)
{
    // Create a buffer to hold the file contents
    auto size = std::filesystem::file_size(path);
    std::vector<char> buffer(size);

    // Read the file
    std::ifstream file(path, std::ios::binary);
    if (!file)
    {
        throw std::ios_base::failure("Failed to open file: " + path.string());
    }
    file.read(buffer.data(), size);

    return buffer;
}

// TODO: Move to utils
void save_file(const std::filesystem::path& path, const std::vector<char>& data)
{
    std::ofstream output(path, std::ios::binary);
    if (!output)
    {
        throw std::ios_base::failure("Failed to open output file: " + path.string());
    }

    output.write(data.data(), data.size());
    if (!output)
    {
        throw std::ios_base::failure("Failed writing file: " + path.string());
    }
}

// Used to transform from encoded `.cod` format to readable ANSI (and vice versa)
void transform_chars(std::vector<char>& buffer)
{
    for (int i = 0; i < buffer.size(); ++i)
    {
        buffer[i] = -buffer[i] & 0xff;
    }
}

std::vector<char> read_cod_file(const std::filesystem::path& path)
{
    std::vector<char> buffer = read_entire_file(path);
    transform_chars(buffer);
    return buffer;
}

CodFile::CodFile(const std::filesystem::path& path)
    : data(read_cod_file(path))
{
}

void CodFile::save_plain_text(const std::filesystem::path& path)
{
    save_file(path, data);
}

void CodFile::save_encoded(const std::filesystem::path& path)
{
    std::vector<char> cod_data = data;
    transform_chars(cod_data);
    save_file(path, cod_data);
}

}  // namespace Anno
