#include <boost/program_options.hpp>

#include <filesystem>
#include <iostream>
#include <string>

#include "files/file_utils.h"
#include "tool/config.h"
#include "tool/tool.h"

namespace po = boost::program_options;

using namespace Anno;

int main(int argc, char* argv[])
{
    // Define accepted program options
    po::options_description desc("Allowed options");
    /* clang-format off */
    desc.add_options()
            ("help", "produce help message")
            ("anno-dir", po::value<std::string>(), "Anno 1602 directory")
            ;
    /* clang-format on */

    // Parse command-line arguments
    po::variables_map vm;
    try
    {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error parsing command line: " << e.what() << "\n\n" << desc << std::endl;
        return 1;
    }

    // Help text
    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return 1;
    }

    Config cfg;

    // Check Anno directory
    const auto it = vm.find("anno-dir");
    if (it == vm.cend())
    {
        std::cout << "Missing required argument: anno-dir\n";
        return 1;
    }
    else
    {
        const std::string anno_dir = it->second.as<std::string>();
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
            cfg.user_dir = FileUtils::get_documents_folder() / "Anno 1602 History Edition";
            cfg.version = GameVersion::HistoryEdition;
        }
        else
        {
            std::cout << "Invalid Anno directory: " << anno_dir << '\n';
            return 1;
        }
    }

    Tool tool(cfg);

    // TMP
    const auto installed_campaigns = tool.get_installed_campaigns();
    for (int i = 0; i < installed_campaigns.size(); ++i)
    {
        const auto& campaign = installed_campaigns[i];
        std::cout << "Campaign: " << campaign.name << " (Progress = " << tool.get_campaign_progress(i) << ")\n";
        for (const auto& level_name : campaign.level_names)
        {
            std::cout << "  " << level_name << "\n";
        }
    }

    std::cout << "Main game progress = " << tool.get_main_game_progress();

    return 0;
}
