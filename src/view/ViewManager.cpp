#include "ViewManager.hpp"
#include <iostream>

ViewManager::ViewManager() {
    // Initialize all views and store them in the views vector
    views.push_back(std::make_unique<MainMenuView>()); 
    views.push_back(std::make_unique<ScanfOptionView>()); 
    views.push_back(std::make_unique<PlaylistView>());
    views.push_back(std::make_unique<MediaFileView>()); 
    views.push_back(std::make_unique<PlayingMediaView>()); 
    views.push_back(std::make_unique<DetailedPlaylistView>()); 
    views.push_back(std::make_unique<MetadataView>());

    currentView = views[SwitchView::SW_MAIN_VIEW].get(); 
}

void ViewManager::showCurrentView() {
    // Display the current view if it is not already shown
    if (currentView && !currentView->isShown()) {
        currentView->showMenu();
    }
}

void ViewManager::hideCurrentView() {
    // Hide the current view if it is currently shown
    if (currentView && currentView->isShown()) {
        currentView->hideMenu();
    }
}

void ViewManager::switchView(SwitchView viewIndex) {
    // Switch to a specific view based on the provided index
    currentView = views[viewIndex].get();
    showCurrentView(); 
}

MainMenuView* ViewManager::getMainMenuView() const {
    // Return the main menu view if it exists
    if (views[SwitchView::SW_MAIN_VIEW]) {
        return dynamic_cast<MainMenuView*>(views[SwitchView::SW_MAIN_VIEW].get());
    }
    else {
        return nullptr;
    }
}

ScanfOptionView* ViewManager::getScanfOptionView() const {
    // Return the scanf option view if it exists
    if (views[SwitchView::SW_SCANF_VIEW]) {
        return dynamic_cast<ScanfOptionView*>(views[SwitchView::SW_SCANF_VIEW].get());
    }
    else {
        return nullptr;
    }
}

PlaylistView* ViewManager::getPlaylistView() const {
    // Return the playlist view if it exists
    if (views[SwitchView::SW_PLAYLIST_VIEW]) {
        return dynamic_cast<PlaylistView*>(views[SwitchView::SW_PLAYLIST_VIEW].get());
    }
    else {
        return nullptr;
    }
}

MediaFileView* ViewManager::getMediaFileView() const {
    // Return the media file view if it exists
    if (views[SwitchView::SW_MEDIAFILE_VIEW]) {
        return dynamic_cast<MediaFileView*>(views[SwitchView::SW_MEDIAFILE_VIEW].get());
    }
    else {
        return nullptr;
    }
}

PlayingMediaView* ViewManager::getPlayingMediaView() const {
    // Return the playing media view if it exists
    if (views[SwitchView::SW_PLAYING_VIEW]) {
        return dynamic_cast<PlayingMediaView*>(views[SwitchView::SW_PLAYING_VIEW].get());
    }
    else {
        return nullptr;
    }
}

DetailedPlaylistView* ViewManager::getDetailedPlaylistView() const {
    // Return the detailed playlist view if it exists
    if (views[SwitchView::SW_DETAILED_VIEW]) {
        return dynamic_cast<DetailedPlaylistView*>(views[SwitchView::SW_DETAILED_VIEW].get());
    }
    else {
        return nullptr;
    }
}

MetadataView* ViewManager::getMetadataView() const {
    // Return the metadata view if it exists
    if (views[SwitchView::SW_METADATA_VIEW]) {
        return dynamic_cast<MetadataView*>(views[SwitchView::SW_METADATA_VIEW].get());
    }
    else {
        return nullptr;
    }
}
