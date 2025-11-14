#include "tool/tool.h"

#include <iostream>

namespace Anno {

Tool::Tool(const Config& cfg)
    : cfg(cfg)
{
}

std::vector<Campaign> Tool::get_installed_campaigns() const
{
    std::cout << "Not implemented yet!\n";
    return {};
}

void Tool::install_campaign(const Campaign& campaign) const
{
    std::cout << "Not implemented yet!\n";
}

void Tool::uninstall_campaign(const Campaign& campaign) const
{
    std::cout << "Not implemented yet!\n";
}

int Tool::get_main_game_progress() const
{
    std::cout << "Not implemented yet!\n";
    return -1;
}

void Tool::set_main_game_progress(int progress)
{
    std::cout << "Not implemented yet!\n";
}

int Tool::get_campaign_progress(int campaign_index) const
{
    std::cout << "Not implemented yet!\n";
    return -1;
}

void Tool::set_campaign_progress(int campaign_index, int progress)
{
    std::cout << "Not implemented yet!\n";
}

void Tool::save_player_data()
{
    std::cout << "Not implemented yet!\n";
}

}  // namespace Anno
