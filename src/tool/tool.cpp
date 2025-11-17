#include "tool/tool.h"

#include <iostream>

#pragma warning(push)
#pragma warning(disable : 4100)  // TMP: Suppress unused parameter warnings for now

namespace Anno {

Tool::Tool(const Config& cfg)
    : cfg(cfg)
    , game_dat_file(cfg.user_dir / "Game.dat", cfg.version)
    , text_cod(cfg.anno_dir / "text.cod")
{
}

std::vector<Campaign> Tool::get_installed_campaigns() const
{
    // TODO: read from text.cod
    std::cout << "Not implemented yet!\n";
    return {};
}

void Tool::install_campaign(const Campaign& campaign) const
{
    // TODO: copy scenarios to "Szenes" folder
    // TODO: add entries to text.cod
    // TODO: add entry to Game.dat
    std::cout << "Not implemented yet!\n";
}

void Tool::uninstall_campaign(const Campaign& campaign) const
{
    // TODO: remove scenarios from "Szenes" folder
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

void Tool::save_player_data()
{
    game_dat_file.save_overwrite();
}

}  // namespace Anno

#pragma warning(pop)
