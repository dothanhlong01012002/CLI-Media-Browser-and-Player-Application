#include "PlaylistController.hpp"
#include "ControllerManager.hpp"

// Handle user input for playlist operations
void PlaylistController::handleInput() {
    PlaylistStatus status = PlaylistStatus::PLAYLIST_NORMAL;

    do {
        // Hide the current view and display all playlists
        ControllerManager::getInstance()->getViewManager()->hideCurrentView();
        showAllPlaylists(ControllerManager::getInstance()->getModelManager()->getPlaylistLibrary()->getAllPlaylists());

        // Display the current status message
        ControllerManager::getInstance()->getViewManager()->getPlaylistView()->displayStatusMessage(status);

        // Switch to the playlist view
        ControllerManager::getInstance()->getViewManager()->switchView(SwitchView::SW_PLAYLIST_VIEW);

        // Handle user input based on the selected playlist menu option
        switch (ControllerManager::getInstance()->getViewManager()->getPlaylistView()->getSelectedOption()) {
            case PlaylistMenu::SELECT_PLAYLIST: {
                std::string PlaylistID;
                Exception_Handler("Enter playlist ID for looking: ",PlaylistID,validateID);
                if (ControllerManager::getInstance()->getModelManager()->getPlaylistLibrary()->isValidPlaylistIDInLibrary(PlaylistID))
                {
                    ControllerManager::getInstance()->getDetailedPlaylistController()->handleInput(PlaylistID);
                }
                else {
                    status = PlaylistStatus::PLAYLIST_SELECT_STATUS;
                }
                break;
            }
            case PlaylistMenu::ADD_PLAYLIST: {
                std::string playlistName;
                Exception_Handler("Enter playlist name for adding: ", playlistName, validateAlphaSring);
                createPlaylist(playlistName);
                break;
            }
            case PlaylistMenu::REMOVE_PLAYLIST: {
                std::string playlistID;
                Exception_Handler("Enter playlist ID for removing: ",playlistID,validateID);
                if (ControllerManager::getInstance()->getModelManager()->getPlaylistLibrary()->isValidPlaylistIDInLibrary(playlistID))
                {
                    deletePlaylist(playlistID);
                }
                else {
                    status = PlaylistStatus::PLAYLIST_REMOVE_STATUS;
                }
                break;
            }
            case PlaylistMenu::BACK_FROM_PLAYLIST: {
                back();
                break;
            }
        }
    } while (ControllerManager::getInstance()->getViewManager()->getPlaylistView()->getSelectedOption() != PlaylistMenu::BACK_FROM_PLAYLIST);
}

// Create a new playlist and add it to the library
void PlaylistController::createPlaylist(const std::string& name) {
    std::shared_ptr<Playlist> ptr = std::make_shared<Playlist>(name);
    ControllerManager::getInstance()->getModelManager()->getPlaylistLibrary()->addPlaylist(ptr);
}

// Delete a playlist from the library
void PlaylistController::deletePlaylist(const std::string& Id) {
    ControllerManager::getInstance()->getModelManager()->getPlaylistLibrary()->removePlaylist(Id);
}

// Handle the back navigation logic
void PlaylistController::back() {
}

// Display all playlists in the playlist view
void PlaylistController::showAllPlaylists(const std::vector<std::shared_ptr<Playlist>>& lists) {
    ControllerManager::getInstance()->getViewManager()->getPlaylistView()->showPlaylistList(lists);
}
