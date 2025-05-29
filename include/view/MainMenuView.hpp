#ifndef MAIN_MENU_VIEW_H
#define MAIN_MENU_VIEW_H

#include "BaseView.hpp"
#include <vector>
#include <string>

enum MainMenu {
    SCAN_OPTIONS,   
    ALL_SONGS,    
    PLAYLIST,    
    NOW_PLAYING,    
    EXIT           
};

class MainMenuView : public BaseView {
public:
    void showMenu() override;

    void hideMenu() override;

    virtual int getSelectedOption() const;

    ~MainMenuView() override = default;

private:
    int selected_option = 0;                   
    std::vector<std::string> menu_entries = {  
        "Scan Options",
        "All Songs",
        "Playlist",
        "Now Playing",
        "Exit"
    };
};

#endif // MAIN_MENU_VIEW_H
