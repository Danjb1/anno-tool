#include "tool/tool.h"

#include <ios>
#include <iostream>

#pragma warning(push)
#pragma warning(disable : 4100)  // TMP: Suppress unused parameter warnings for now

namespace Anno {

/*
 * Helper methods
 */

static bool is_scenario_file(const std::filesystem::directory_entry& entry)
{
    if (!entry.is_regular_file())
    {
        // Ignore directories
        return false;
    }

    const std::filesystem::path extension = entry.path().extension();
    return extension == ".szs" || extension == ".szm";
}

static std::string get_campaign_name(const std::string& scenario_filename)
{
    // Just remove the last character, which is the index of the scenario within the campaign
    return scenario_filename.substr(0, scenario_filename.length() - 1);
}

/*
 * Tool class
 */

Tool::Tool(const Config& cfg)
    : cfg(cfg)
    , game_dat_file(cfg.user_dir / "Game.dat", cfg.version)
    , text_cod(cfg.anno_dir / "text.cod")
{
    read_installed_scenarios();
    parse_campaign_level_names();
}

void Tool::read_installed_scenarios()
{
    std::map<int, std::string> campaign_names;

    // Read all scenarios in "Szenes" directory
    for (const auto& entry : std::filesystem::directory_iterator(cfg.anno_dir / "Szenes"))
    {
        if (!is_scenario_file(entry))
        {
            continue;
        }

        try
        {
            const ScenarioFile& scenario = installed_scenarios.emplace_back(entry.path());

            // Does this scenario belong to a campaign?
            int campaign_index = scenario.get_campaign_index();
            if (campaign_index >= 0)
            {
                campaign_names.try_emplace(campaign_index, get_campaign_name(scenario.get_filename()));
            }
        }
        catch (const std::ios_base::failure& error)
        {
            std::cout << "Failed to read scenario file: " << entry.path() << " (Error: " << error.what() << ")\n";
        }
    }

    // If no campaigns found, nothing to do
    if (campaign_names.empty())
    {
        return;
    }

    // Create campaign entries
    const int max_campaign_index = campaign_names.rbegin()->first;
    installed_campaigns.resize(max_campaign_index + 1);

    // Save the campaign names
    for (int i = 0; i <= max_campaign_index; ++i)
    {
        const auto it = campaign_names.find(i);
        if (it == campaign_names.cend())
        {
            std::cerr << "Campaign " << i << " is missing!\n";
        }
        else
        {
            installed_campaigns[i].name = it->second;
        }
    }
}

void Tool::parse_campaign_level_names()
{
    const auto campaign_data = text_cod.get_section_contents(TextCodFile::section_campaign);

    int campaign_index = 0;
    int last_campaign_index = -1;

    for (const auto& line : campaign_data)
    {
        // Skip blank lines
        if (line.empty())
        {
            campaign_index = last_campaign_index + 1;
            continue;
        }

        if (installed_campaigns.size() <= campaign_index)
        {
            std::cerr << "Found level names for non-existant campaign: " << campaign_index << '\n';
            installed_campaigns.emplace_back("[Missing Campaign]");
        }

        // Add level to campaign
        installed_campaigns[campaign_index].level_names.push_back(line);
        last_campaign_index = campaign_index;
    }
}

std::vector<Campaign> Tool::get_installed_campaigns() const
{
    return installed_campaigns;
}

// TODO: Failure inside this method could leave the program / game files in a weird state
bool Tool::install_campaign(const Campaign& campaign)
{
    int campaign_index = static_cast<int>(installed_campaigns.size());

    /*
     * 1. Add level names to `text.cod`
     */

    std::cout << "Adding level names to text.cod...\n";
    auto campaign_data = text_cod.get_section_contents(TextCodFile::section_campaign);
    campaign_data.push_back("");  // blank line
    for (const auto& level_name : campaign.level_names)
    {
        campaign_data.push_back(level_name);
    }
    campaign_data.push_back("");
    text_cod.set_section_contents(TextCodFile::section_campaign, campaign_data);
    try
    {
        text_cod.save_overwrite();
    }
    catch (const std::ios_base::failure& e)
    {
        std::cout << "Failed to write to text.cod: " << e.what();
        return false;
    }

    /*
     * 2. Modify scenario files
     */

    std::cout << "Linking scenarios to campaign...\n";
    std::vector<ScenarioFile> scenarios_in_campaign;  // TODO: look up scenarios based on campaign name
    for (auto& scenario_file : scenarios_in_campaign)
    {
        scenario_file.set_campaign_index(campaign_index);
        try
        {
            scenario_file.save_overwrite();
        }
        catch (const std::ios_base::failure& e)
        {
            std::cout << "Failed to write to " << scenario_file.get_filename() << ": " << e.what();
            return false;
        }
    }

    /*
     * 3. Add entry to Game.dat
     */

    std::cout << "Adding entry to Game.dat...\n";
    game_dat_file.set_campaign_progress(campaign_index, 0);
    try
    {
        game_dat_file.save_overwrite();
    }
    catch (const std::ios_base::failure& e)
    {
        std::cout << "Failed to write to Game.dat: " << e.what();
        return false;
    }

    std::cout << "Success!\n";
    return true;
}

void Tool::uninstall_campaign(const Campaign& campaign)
{
    // TODO: remove entries from text.cod
    // TODO: remove entry from Game.dat
    // TODO: adjust indices of all subsequent campaign progress entries in Game.dat
    std::cout << "Not implemented yet!\n";
}

int Tool::get_main_game_progress() const
{
    return game_dat_file.get_main_game_progress();
}

void Tool::set_main_game_progress(int progress)
{
    game_dat_file.set_main_game_progress(progress);
}

int Tool::get_campaign_progress(int campaign_index) const
{
    return game_dat_file.get_campaign_progress(campaign_index);
}

void Tool::set_campaign_progress(int campaign_index, int progress)
{
    game_dat_file.set_campaign_progress(campaign_index, progress);
}

}  // namespace Anno

#pragma warning(pop)
