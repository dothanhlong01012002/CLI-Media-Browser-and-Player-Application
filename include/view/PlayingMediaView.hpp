#ifndef PLAYING_MEDIA_VIEW_H
#define PLAYING_MEDIA_VIEW_H

#include "BaseView.hpp"
#include <vector>
#include <string>
#include <functional>
#include "MediaFile.hpp"
#include <iostream>
#include <memory>
#include <PlayingMedia.hpp>

enum PlayingMediaMenu {
    PLAY,                  
    PAUSE,                 
    NEXT,                  
    PREV,                  
    BACK_FROM_PLAYING      
};

class PlayingMediaView : public BaseView {
private:
    std::vector<std::string> menu_entries = {
        "Play",
        "Pause",
        "Next Song",
        "Previous Song",
        "Go Back"
    };
    int selected_option = 0; 

public:
    void showMenu() override;

    virtual int getSelectedOption() const;

    ~PlayingMediaView() override = default;

    void hideMenu() override;
    virtual void showPlayingMedia(PlayingMedia* player, size_t& currentTime, size_t totalTime, int& volume) ;
};

#endif // PLAYING_MEDIA_VIEW_H
