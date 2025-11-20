#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "files/game_dat_file.h"
#include "files/scenario_file.h"
#include "files/text_cod_file.h"
#include "tool/config.h"

namespace Anno {

struct Campaign
{
    std::string name;
    std::vector<std::string> level_names;
};

class Tool
{
public:
    Tool(const Config& cfg);

    /** Gets the list of installed campaigns. */
    std::vector<Campaign> get_installed_campaigns() const;

    /** Installs a campaign. */
    bool install_campaign(const Campaign& campaign);

    /** Uninstalls a campaign. */
    void uninstall_campaign(const Campaign& campaign);

    /** Gets the player's progress in the main game. */
    int get_main_game_progress() const;

    /** Sets the player's progress in the main game. */
    void set_main_game_progress(int progress);

    /** Gets the player's progress in a campaign. */
    int get_campaign_progress(int campaign_index) const;

    /** Sets the player's progress in a campaign.
     * Changes will not be saved to disk until `save_player_data` is called. */
    void set_campaign_progress(int campaign_index, int progress);

private:
    void read_installed_scenarios();
    void parse_campaign_level_names();

    Config cfg;
    GameDatFile game_dat_file;
    TextCodFile text_cod;
    std::vector<ScenarioFile> installed_scenarios;
    std::vector<Campaign> installed_campaigns;
};

}  // namespace Anno
