#include <filesystem>
#include <vector>

namespace Anno {

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
