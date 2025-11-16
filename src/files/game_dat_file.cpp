#include "files/game_dat_file.h"

#include <fstream>
#include <stdexcept>

#include "files/file_utils.h"

namespace Anno {

std::vector<std::string> read_dat_file(const std::filesystem::path& path)
{
    std::vector<std::string> lines = FileUtils::read_text_file(path);
    // TODO: parse data
    return lines;
}

GameDatFile::GameDatFile(const std::filesystem::path& path)
    : lines(read_dat_file(path))
{
}

void GameDatFile::save(const std::filesystem::path& path)
{
    FileUtils::write_text_file(path, lines);
}

}  // namespace Anno
