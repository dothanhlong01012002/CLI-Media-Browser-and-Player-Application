#ifndef VIEW_MANAGER_H
#define VIEW_MANAGER_H

#include "DetailedPlaylistView.hpp"
#include "MainMenuView.hpp"
#include "MediaFileView.hpp"
#include "MetadataView.hpp"
#include "PlayingMediaView.hpp"
#include "PlaylistView.hpp"
#include "ScanfOptionView.hpp"
#include <memory>

enum SwitchView {
    SW_MAIN_VIEW,         
    SW_SCANF_VIEW,        
    SW_PLAYLIST_VIEW,    
    SW_MEDIAFILE_VIEW,    
    SW_PLAYING_VIEW,     
    SW_DETAILED_VIEW,    
    SW_METADATA_VIEW     
};

class ViewManager {
private:
    BaseView* currentView;                              
    std::vector<std::unique_ptr<BaseView>> views;      

public:
    ViewManager();

    virtual void showCurrentView();

    virtual void hideCurrentView();

    virtual void switchView(SwitchView viewIndex);

    virtual ScanfOptionView* getScanfOptionView() const;

    virtual PlaylistView* getPlaylistView() const;

    virtual MediaFileView* getMediaFileView() const;

    virtual PlayingMediaView* getPlayingMediaView() const;

    virtual DetailedPlaylistView* getDetailedPlaylistView() const;

    virtual MetadataView* getMetadataView() const;

    virtual MainMenuView* getMainMenuView() const;

    virtual ~ViewManager() = default;
};

#endif // VIEW_MANAGER_H
