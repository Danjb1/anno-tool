#include <boost/optional.hpp>
#include <boost/program_options.hpp>

#include <filesystem>
#include <iostream>
#include <string>

#include "files/file_utils.h"
#include "tool/config.h"
#include "tool/tool.h"

namespace po = boost::program_options;

using namespace Anno;

static bool check_anno_installation(Config& cfg, boost::optional<std::string> anno_dir)
{
    // Ensure Anno directory was provided
    if (!anno_dir.has_value())
    {
        // TODO: check registry
        std::cerr << "Missing required argument: anno-dir\n";
        return false;
    }

    std::filesystem::path anno_dir_path = std::filesystem::path(*anno_dir);

    // Check for the original game
    if (std::filesystem::exists(anno_dir_path / "1602.exe"))
    {
        std::cout << "Found Anno 1602 installation\n\n";
        cfg.anno_dir = *anno_dir;
        cfg.user_dir = *anno_dir;
        return true;
    }

    // Check for the History Edition
    if (std::filesystem::exists(anno_dir_path / "Anno1602.exe"))
    {
        std::cout << "Found Anno 1602 History Edition installation\n\n";
        cfg.anno_dir = *anno_dir;
        cfg.user_dir = FileUtils::get_documents_folder() / "Anno 1602 History Edition";
        cfg.version = GameVersion::HistoryEdition;
        return true;
    }

    std::cerr << "Invalid Anno directory: " << *anno_dir << '\n';

    return false;
}

static void list_installed_campaigns(const Tool& tool)
{
    std::cout << "Installed campaigns:\n\n";

    const auto installed_campaigns = tool.get_installed_campaigns();
    if (installed_campaigns.empty())
    {
        std::cout << "None\n";
        return;
    }

    for (int i = 0; i < installed_campaigns.size(); ++i)
    {
        const auto& campaign = installed_campaigns[i];
        std::cout << "  " << campaign.name << " (Progress = " << tool.get_campaign_progress(i) << ")\n";
        for (const auto& level_name : campaign.level_names)
        {
            std::cout << "    " << level_name << '\n';
        }
        std::cout << '\n';
    }
}

static Campaign read_campaign_definition(const std::filesystem::path& path)
{
    const std::string campaign_name = path.stem().string();
    Campaign campaign(campaign_name);

    std::vector<std::string> lines = FileUtils::read_text_file(path);
    for (const auto& line : lines)
    {
        if (line.empty())
        {
            // Ignore black lines
            continue;
        }

        campaign.level_names.push_back(line);
    }

    return campaign;
}

static void install_campaign(Tool& tool, const po::variables_map& vm)
{
    std::string input_filename = vm["input-file"].as<std::string>();
    std::filesystem::path campaign_path = std::filesystem::path(input_filename);
    Campaign campaign = read_campaign_definition(campaign_path);

    tool.install_campaign(campaign);
}

int main(int argc, char* argv[])
{
    boost::optional<std::string> anno_dir;

    // General options (always allowed)
    po::options_description general_options("General options");
    general_options.add_options()                                      //
            ("help", "produce help message")                           //
            ("anno-dir", po::value(&anno_dir), "Anno 1602 directory")  //
            ;

    // Instructions (one allowed)
    po::options_description instructions("Instructions");
    instructions.add_options()                                                             //
            ("list-campaigns", "list all installed campaigns")                             //
            ("install-campaign", "install a campaign using the supplied definition file")  //
            ;

    // All accepted options combined
    po::options_description all_options("Allowed options");
    all_options.add(general_options).add(instructions);

    // Define positional program options
    po::positional_options_description positional_options;
    positional_options.add("input-file", 1);

    // Parse command-line arguments
    po::variables_map vm;
    try
    {
        po::store(po::command_line_parser(argc, argv)      //
                          .options(all_options)            //
                          .positional(positional_options)  //
                          .run(),
                vm);
        po::notify(vm);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error parsing command line: " << e.what() << "\n\n" << all_options << '\n';
        return 1;
    }

    // Help text
    if (vm.count("help"))
    {
        std::cout << all_options << '\n';
        return 1;
    }

    // Sanity checking
    size_t num_functions_requested = vm.count("install-campaign") + vm.count("list-campaigns");
    if (num_functions_requested == 0)
    {
        std::cerr << "Please specify an instruction.\n\n" << instructions << '\n';
        return 1;
    }
    if (num_functions_requested > 1)
    {
        std::cerr << "Only 1 instruction can be provided.\n\n" << instructions << '\n';
        return 1;
    }
    if (vm.count("install-campaign") && !vm.count("input-file"))
    {
        std::cerr << "No campaign file provided!\n";
        return 1;
    }

    // Find Anno directory
    Config cfg;
    if (!check_anno_installation(cfg, anno_dir))
    {
        return 1;
    }

    try
    {
        // Initialize the program
        Tool tool(cfg);

        // Execute the desired functionality
        if (vm.count("list-campaigns"))
        {
            list_installed_campaigns(tool);
        }
        else if (vm.count("install-campaign"))
        {
            install_campaign(tool, vm);
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "Fatal error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
