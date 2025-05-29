#include "MediaFileController.hpp"
#include "ControllerManager.hpp"
#include "HardwareController.hpp"

// Handle user input for the media file view
void MediaFileController::handleInput() {
    MediaFileStatus status = MediaFileStatus::MEDIAFILE_NORMAL;
    size_t totalPage = ControllerManager::getInstance()->getModelManager()->getMediaLibrary()->getAllMediaFiles().size();
    totalPage = (size_t)(totalPage / 25) + 1; 

    do {
        ControllerManager::getInstance()->getViewManager()->hideCurrentView();
        displayMediaFilesWithPagination(ControllerManager::getInstance()->getModelManager()->getMediaLibrary()->getAllMediaFiles());
        ControllerManager::getInstance()->getViewManager()->getMediaFileView()->displayStatusMessage(status);
        ControllerManager::getInstance()->getViewManager()->switchView(SwitchView::SW_MEDIAFILE_VIEW);

    switch (ControllerManager::getInstance()->getViewManager()->getMediaFileView()->getSelectedOption())
        {
        case MediaFileMenu::BACK_FROM_MEDIA: {
            back();
            break;
        }
        case MediaFileMenu::PLAY_SONG_FROM_FILES:{
            std::string songID;
            displayMediaFilesWithPagination(ControllerManager::getInstance()->getModelManager()->getMediaLibrary()->getAllMediaFiles());
            Exception_Handler("Enter song ID for playing: ",songID,validateID);
            ControllerManager::getInstance()->getHardwareController()->sendSignal(ControllerManager::getInstance()->getModelManager()->getPlayingMedia()->getDurationStringType());
            ControllerManager::getInstance()->getHardwareController()->sendPlayCommand();
            if (ControllerManager::getInstance()->getModelManager()->getMediaLibrary()->isValidMediaFileIDInLibrary(songID))
            {
                ControllerManager::getInstance()->getModelManager()->getPlayingMedia()->setPlaylist(ControllerManager::getInstance()->getModelManager()->getMediaLibrary()->getAllMediaFiles());
                ControllerManager::getInstance()->getPlayingMediaController()->handleInput(songID);
            }
            else {
                status = MediaFileStatus::MEDIAFILE_PLAY_STATUS;
            }
            break;
        }
           
        case MediaFileMenu::SHOW_DETAIL:{
            std::string songID;
            displayMediaFilesWithPagination(ControllerManager::getInstance()->getModelManager()->getMediaLibrary()->getAllMediaFiles());
            Exception_Handler("Enter song ID for looking details: ",songID,validateID);
            if (ControllerManager::getInstance()->getModelManager()->getMediaLibrary()->isValidMediaFileIDInLibrary(songID))
            {
                ControllerManager::getInstance()->getMetadataController()->handleInput(songID);
            }
            else {
                status = MediaFileStatus::MEDIAFILE_DETAIL_STATUS;
            }
            break;
        }
        case MediaFileMenu::NEXT_PAGE:{
            if (currentPage < totalPage) {
                currentPage++;
            }
            else {
                status = MediaFileStatus::MEDIAFILE_NEXT_PAGE_STATUS;
            }
            break;
        }
        case MediaFileMenu::PREV_PAGE:{
            if (currentPage > 1) {
                currentPage--;
            }
            else {
                status = MediaFileStatus::MEDIAFILE_PREV_PAGE_STATUS;
            }
            break;
        }   
        }
    } while(ControllerManager::getInstance()->getViewManager()->getMediaFileView()->getSelectedOption() != MediaFileMenu::BACK_FROM_MEDIA);
}

void MediaFileController::back(){
    currentPage = 1;
}

// Display media files with pagination
void MediaFileController::displayMediaFilesWithPagination(const std::vector<std::shared_ptr<MediaFile>>& files, size_t pageSize) {
    size_t totalSongs = files.size();
    size_t firstSong = (currentPage - 1) * pageSize;
    size_t lastSong = std::min(currentPage * pageSize - 1, totalSongs - 1);
    ControllerManager::getInstance()->getViewManager()->getMediaFileView()->showMediaFilesPage(files, currentPage, firstSong, lastSong);

}