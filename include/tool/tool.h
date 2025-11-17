#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "files/cod_file.h"
#include "files/game_dat_file.h"
#include "tool/config.h"

namespace Anno {

struct CampaignLevel
{
    std::string name;
    std::filesystem::path path;
};

struct Campaign
{
    std::string name;
    std::vector<CampaignLevel> levels;
};

class Tool
{
public:
    Tool(const Config& cfg);

    /** Gets the list of installed campaigns. */
    std::vector<Campaign> get_installed_campaigns() const;

    /** Installs a campaign. */
    void install_campaign(const Campaign& campaign) const;

    /** Uninstalls a campaign. */
    void uninstall_campaign(const Campaign& campaign) const;

    /** Gets the player's progress in the main game. */
    int get_main_game_progress() const;

    /** Sets the player's progress in the main game. */
    void set_main_game_progress(int progress);

    /** Gets the player's progress in a campaign. */
    int get_campaign_progress(int campaign_index) const;

    /** Sets the player's progress in a campaign.
     * Changes will not be saved to disk until `save_player_data` is called. */
    void set_campaign_progress(int campaign_index, int progress);

    /** Saves the player data (contained within Game.dat) to disk. */
    void save_player_data();

private:
    Config cfg;
    GameDatFile game_dat_file;
    CodFile text_cod;
    std::vector<Campaign> installed_campaigns;
};

}  // namespace Anno
