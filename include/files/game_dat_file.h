#include <filesystem>
#include <vector>

namespace Anno {

/**
 * Class used for reading and writing to `Game.dat`, which stores user save data and preferences.
 *
 * More info:
 * https://github.com/Green-Sky/anno16_docs/blob/master/files/game.dat.md
 */
class GameDatFile
{
public:
    GameDatFile(const std::filesystem::path& path);

    void save(const std::filesystem::path& path);

private:
    std::vector<std::string> lines;
};

}  // namespace Anno
