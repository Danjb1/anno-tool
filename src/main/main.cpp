#include <boost/program_options.hpp>

#include <filesystem>
#include <iostream>
#include <string>

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

    // Check Anno directory
    auto it = vm.find("anno-dir");
    if (it == vm.cend())
    {
        std::cout << "Missing required argument: anno-dir\n";
        return 1;
    }
    else
    {
        std::string annoDir = it->second.as<std::string>();
        // text.cod is present in both the original game and the History Edition
        std::filesystem::path filePath = std::filesystem::path(annoDir) / "text.cod";
        if (std::filesystem::exists(filePath))
        {
            std::cout << "Found Anno directory: " << annoDir << "\n";
        }
        else
        {
            std::cout << "Invalid Anno directory: " << annoDir << "\n";
            return 1;
        }
    }

    return 0;
}
