#include <filesystem>
#include <string>
#include <vector>

namespace Anno { namespace FileUtils {

std::filesystem::path get_documents_folder();

std::vector<char> read_binary_file(const std::filesystem::path& path);

std::vector<std::string> read_text_file(const std::filesystem::path& path);

void write_binary_file(const std::filesystem::path& path, const std::vector<char>& data);

void write_text_file(const std::filesystem::path& path, const std::vector<std::string>& lines);

}}  // namespace Anno::FileUtils
