#include <boost/program_options.hpp>

#include <filesystem>
#include <iostream>
#include <string>

#include "files/cod_file.h"
#include "tool/tool.h"

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
    // Define accepted program options
    po::options_description desc("Allowed options");
    desc.add_options()                                                     //
            ("help", "produce help message")                               //
            ("anno-dir", po::value<std::string>(), "Anno 1602 directory")  //
            ;

    // Parse command-line arguments
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    // Help text
    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return 1;
    }

    Anno::Config cfg;

    // Check Anno directory
    auto it = vm.find("anno-dir");
    if (it == vm.cend())
    {
        std::cout << "Missing required argument: anno-dir\n";
        return 1;
    }
    else
    {
        std::string anno_dir = it->second.as<std::string>();
        std::filesystem::path path = std::filesystem::path(anno_dir);
        if (std::filesystem::exists(path / "1602.exe"))
        {
            std::cout << "Found Anno 1602 directory: " << anno_dir << '\n';
            cfg.anno_dir = anno_dir;
            cfg.user_dir = anno_dir;
        }
        else if (std::filesystem::exists(path / "Anno1602.exe"))
        {
            std::cout << "Found Anno 1602 History Edition directory: " << anno_dir << '\n';
            cfg.anno_dir = anno_dir;
            cfg.user_dir = std::filesystem::path("%UserProfile%/Documents/Anno 1602 History Edition");
            cfg.version = Anno::GameVersion::HistoryEdition;
        }
        else
        {
            std::cout << "Invalid Anno directory: " << anno_dir << '\n';
            return 1;
        }
    }

    // TMP
    Anno::Tool tool(cfg);
    std::cout << "Main game progress = " << tool.get_main_game_progress();

    // TMP
    Anno::CodFile cod_file(cfg.anno_dir / "text.cod");
    cod_file.save_plain_text("C:/tmp/plain_text.cod");
    cod_file.save_encoded("C:/tmp/encoded.cod");

    return 0;
}
