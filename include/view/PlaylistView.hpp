#ifndef PLAYLIST_VIEW_H
#define PLAYLIST_VIEW_H

#include "BaseView.hpp"
#include <vector>
#include <string>
#include "Playlist.hpp"

enum PlaylistStatus {
    PLAYLIST_NORMAL,        
    PLAYLIST_SELECT_STATUS,   
    PLAYLIST_ADD_STATUS,      
    PLAYLIST_REMOVE_STATUS   
};

enum PlaylistMenu {
    SELECT_PLAYLIST,       
    ADD_PLAYLIST,           
    REMOVE_PLAYLIST,       
    BACK_FROM_PLAYLIST     
};

class PlaylistView : public BaseView {
private:
    std::vector<std::string> menu_entries = {
        "Select a Playlist",
        "Add a Playlist",
        "Remove a Playlist",
        "Go Back"
    };
    int selected_option = 0; 

public:
    void showMenu() override;

    virtual int getSelectedOption() const;

    ~PlaylistView() override = default;

    void hideMenu() override;

    virtual void showPlaylistList(const std::vector<std::shared_ptr<Playlist>>& playlists);

    virtual void displayStatusMessage(PlaylistStatus& status);
};

#endif // PLAYLIST_VIEW_H
