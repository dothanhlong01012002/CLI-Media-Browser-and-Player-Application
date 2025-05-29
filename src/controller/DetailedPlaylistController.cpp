#include "DetailedPlaylistController.hpp"
#include "ControllerManager.hpp"

// Handle user input for the detailed playlist view
// Main method to handle user interactions within the detailed playlist view.
void DetailedPlaylistController::handleInput(std::string listId) {
    DetailedPlaylistStatus status = DetailedPlaylistStatus::DETAILED_NORMAL; // Initialize the playlist status to normal.
    do {
        // Hide the current view and display the list of media files in the playlist.
        ControllerManager::getInstance()->getViewManager()->hideCurrentView();
        showMediafileInList(listId);

        // Display the status message (e.g., success, failure, etc.).
        ControllerManager::getInstance()->getViewManager()->getDetailedPlaylistView()->displayStatusMessage(status);

        // Switch to the detailed playlist view for user interaction.
        ControllerManager::getInstance()->getViewManager()->switchView(SwitchView::SW_DETAILED_VIEW);

        // Handle user input based on the selected option in the detailed playlist menu.
        switch (ControllerManager::getInstance()->getViewManager()->getDetailedPlaylistView()->getSelectedOption()) {
            case DetailedPlaylistMenu::SHOW_DETAIL_SONG: {
                std::string songID;
                Exception_Handler("Enter song ID for looking details: ", songID, validateID);
                if (ControllerManager::getInstance()->getModelManager()->getPlaylistLibrary()
                        ->getPlaylistByID(listId)
                        ->isValidMediaFileIDInPlaylist(songID)) {
                    ControllerManager::getInstance()->getMetadataController()->handleInput(songID);
                } else {
                    status = DetailedPlaylistStatus::DETAILED_SHOW_STATUS;
                }
                break;
            }
            case DetailedPlaylistMenu::ADD_SONG: {
                std::string songID;
                Exception_Handler("Enter song ID for adding: ", songID, validateID);
                if (ControllerManager::getInstance()->getModelManager()->getMediaLibrary()->isValidMediaFileIDInLibrary(songID)) {
                    if (!ControllerManager::getInstance()->getModelManager()->getPlaylistLibrary()
                            ->getPlaylistByID(listId)
                            ->isValidMediaFileIDInPlaylist(songID)) {
                        addSongToPlaylist(listId, songID);
                    } else {
                        status = DetailedPlaylistStatus::DETAILED_ADD_STATUS;
                    }
                } else {
                    status = DetailedPlaylistStatus::DETAILED_ADD_STATUS;
                }
                break;
            }
            case DetailedPlaylistMenu::DELETE_SONG: {
                std::string songID;
                Exception_Handler("Enter song ID for removing: ", songID, validateID);
                if (ControllerManager::getInstance()->getModelManager()->getPlaylistLibrary()
                        ->getPlaylistByID(listId)
                        ->isValidMediaFileIDInPlaylist(songID)) {
                    removeSongFromPlaylist(listId, songID);
                } else {
                    status = DetailedPlaylistStatus::DETAILED_DELETE_STATUS;
                }
                break;
            }
            case DetailedPlaylistMenu::BACK_FROM_DETAIL: {
                back();
                break;
            }
            case DetailedPlaylistMenu::PLAY_SONG_FROM_PLAYLIST: {
                std::string songID;
                Exception_Handler("Enter song ID for playing: ", songID, validateID);
                if (ControllerManager::getInstance()->getModelManager()->getPlaylistLibrary()
                        ->getPlaylistByID(listId)
                        ->isValidMediaFileIDInPlaylist(songID)) {
                    ControllerManager::getInstance()->getModelManager()->getPlayingMedia()
                        ->setPlaylist(ControllerManager::getInstance()->getModelManager()->getPlaylistLibrary()
                                      ->getPlaylistByID(listId)
                                      ->getSongs());
                    ControllerManager::getInstance()->getHardwareController()->sendSignal(ControllerManager::getInstance()->getModelManager()->getPlayingMedia()->getDurationStringType());
                    ControllerManager::getInstance()->getHardwareController()->sendPlayCommand();
                    ControllerManager::getInstance()->getPlayingMediaController()->handleInput(songID);
                } else {
                    status = DetailedPlaylistStatus::DETAILED_PLAY_STATUS;
                }
                break;
            }
        }
    } while (ControllerManager::getInstance()->getViewManager()->getDetailedPlaylistView()->getSelectedOption() != 
             DetailedPlaylistMenu::BACK_FROM_DETAIL); // Continue until the user chooses to go back.
}

// Add a song to the playlist
// Adds a song to the specified playlist by ID.
void DetailedPlaylistController::addSongToPlaylist(const std::string& PlaylistId, const std::string& songID) {
    ControllerManager::getInstance()->getModelManager()->getPlaylistLibrary()
        ->getPlaylistByID(PlaylistId)
        ->addSong((ControllerManager::getInstance()->getModelManager()->getMediaLibrary()->getMediaFileByID(songID)));
}

// Remove a song from the playlist
// Removes a song from the specified playlist by ID.
void DetailedPlaylistController::removeSongFromPlaylist(const std::string& PlaylistId, const std::string& songID) {
    ControllerManager::getInstance()->getModelManager()->getPlaylistLibrary()
        ->getPlaylistByID(PlaylistId)
        ->removeSong(songID);
}

// Handle the back navigation logic
// Handles the back navigation from the detailed playlist view.
void DetailedPlaylistController::back() {
}

// Show media files in the playlist
// Displays the list of songs in the specified playlist ID in the detailed playlist view.
void DetailedPlaylistController::showMediafileInList(const std::string& listId) {
    ControllerManager::getInstance()->getViewManager()->getDetailedPlaylistView()
        ->showListOfSongs(ControllerManager::getInstance()->getModelManager()->getPlaylistLibrary()->getPlaylistByID(listId));
}
