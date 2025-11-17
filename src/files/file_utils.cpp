#include "files/file_utils.h"

#include <fstream>
#include <stdexcept>

#ifdef _WIN32
#include <shlobj.h>
#endif

#ifdef _WIN32
#include "platform/windows/windows_config.h"
#endif

namespace Anno { namespace FileUtils {

std::filesystem::path get_documents_folder()
{
#ifdef _WIN32
    std::filesystem::path documents_path;

    PWSTR found_path = nullptr;
    if (SHGetKnownFolderPath(FOLDERID_Documents, 0, /* current user */ NULL, /* out */ &found_path) == S_OK)
    {
        documents_path = found_path;
    }
    CoTaskMemFree(found_path);

    if (documents_path.empty())
    {
        throw std::runtime_error("Failed to retrieve Documents folder!");
    }

    return documents_path;
#else
    throw std::runtime_error("Not implemented!");
#endif
}

std::vector<char> read_binary_file(const std::filesystem::path& path)
{
    // Try to open the file
    std::ifstream file_stream(path, std::ios::binary);
    if (!file_stream)
    {
        throw std::ios_base::failure("Failed to open file for reading: " + path.string());
    }

    // Create a buffer to hold the file contents
    const auto file_size = std::filesystem::file_size(path);
    std::vector<char> buffer(file_size);

    // Read the file fully
    file_stream.read(buffer.data(), file_size);

    // Check for errors
    if (file_stream.bad())
    {
        throw std::ios_base::failure("Error reading file: " + path.string());
    }

    return buffer;
}

std::vector<std::string> read_text_file(const std::filesystem::path& path)
{
    // Try to open the file
    std::ifstream file_stream(path, std::ios::binary);
    if (!file_stream)
    {
        throw std::ios_base::failure("Failed to open file for reading: " + path.string());
    }

    std::vector<std::string> lines;
    std::string line;

    // Reserve some space based on a rough line count estimate
    const auto file_size = std::filesystem::file_size(path);
    lines.reserve(static_cast<size_t>(file_size / 32));

    // Read the file line-by-line
    while (std::getline(file_stream, line))
    {
        // Strip out carriage return ('\r') if present
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        lines.push_back(line);
    }

    // Check for errors
    if (file_stream.bad())
    {
        throw std::ios_base::failure("Error reading file: " + path.string());
    }

    return lines;
}

void write_binary_file(const std::filesystem::path& path, const std::vector<char>& data)
{
    // Try to open the file
    std::ofstream file_stream(path, std::ios::binary);
    if (!file_stream)
    {
        throw std::ios_base::failure("Failed to open file for writing: " + path.string());
    }

    // Write the data to file
    file_stream.write(data.data(), data.size());

    // Check for errors
    if (!file_stream)
    {
        throw std::ios_base::failure("Error writing file: " + path.string());
    }
}

void write_text_file(const std::filesystem::path& path, const std::string& text)
{
    // Try to open the file
    std::ofstream file_stream(path, std::ios::binary);
    if (!file_stream)
    {
        throw std::ios_base::failure("Failed to open file for writing: " + path.string());
    }

    // Write file
    file_stream.write(text.data(), text.size());

    // Check for write errors
    if (!file_stream)
    {
        throw std::ios_base::failure("Error writing file: " + path.string());
    }
}

void write_text_file(const std::filesystem::path& path, const std::vector<std::string>& lines)
{
    // Try to open the file
    std::ofstream file_stream(path, std::ios::binary);
    if (!file_stream)
    {
        throw std::ios_base::failure("Failed to open file for writing: " + path.string());
    }

    // Write file line-by-line
    for (const auto& line : lines)
    {
        file_stream.write(line.data(), line.size());
        file_stream.put('\n');
    }

    // Check for write errors
    if (!file_stream)
    {
        throw std::ios_base::failure("Error writing file: " + path.string());
    }
}

}}  // namespace Anno::FileUtils
