#include <filesystem>
#include <string>
#include <vector>

namespace Anno { namespace FileUtils {

/** Gets the current user's Documents folder, e.g. `%USERPROFILE%/Documents` on Windows.
 * Throws a std::runtime_error if an error occurs. */
std::filesystem::path get_documents_folder();

/** Reads a file and returns the bytes contained within.
 * May throw a std::ios_base::failure. */
std::vector<char> read_binary_file(const std::filesystem::path& path);

/** Reads a text file and returns all lines contained within it (with line endings removed).
 * May throw a std::ios_base::failure. */
std::vector<std::string> read_text_file(const std::filesystem::path& path);

/** Writes bytes to a file.
 * May throw a std::ios_base::failure. */
void write_binary_file(const std::filesystem::path& path, const std::vector<char>& data);

/** Writes a single string to a file.
 * May throw a std::ios_base::failure. */
void write_text_file(const std::filesystem::path& path, const std::string& text);

/** Writes multiple lines to a file, with line separators in between.
 * May throw a std::ios_base::failure. */
void write_text_file(const std::filesystem::path& path, const std::vector<std::string>& lines);

}}  // namespace Anno::FileUtils
