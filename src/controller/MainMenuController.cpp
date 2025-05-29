#include "MainMenuController.hpp"
#include "ControllerManager.hpp"

void MainMenuController::handleInput() {
    // Start the thread for reading data from the hardware controller
    ControllerManager::getInstance()->getHardwareController()->startReading();

    bool status = false;
    std::string filePath = "resources/playlists.txt";

    // Load playlists from the specified file
    ControllerManager::getInstance()->getScanfOptionController()->scanPlaylistsFromTxt(filePath);

    do {
    ControllerManager::getInstance()->getViewManager()->hideCurrentView();
    ControllerManager::getInstance()->getViewManager()->switchView(SwitchView::SW_MAIN_VIEW);
    if (status) {
        std::cout << "No current mediafile playing\n";
        status = false;
    }
    
    switch (ControllerManager::getInstance()->getViewManager()->getMainMenuView()->getSelectedOption())
        {
            case MainMenu::EXIT: {
                back();
                break;
            }
            case MainMenu::SCAN_OPTIONS: {
                ControllerManager::getInstance()->getScanfOptionController()->handleInput();
                break;
            }
            case MainMenu::ALL_SONGS: {
                ControllerManager::getInstance()->getMediaFileController()->handleInput();
                break;
            }
            case MainMenu::PLAYLIST: {
                ControllerManager::getInstance()->getPlaylistController()->handleInput();
                break;
            }
            case MainMenu::NOW_PLAYING:{
                if (ControllerManager::getInstance()->getModelManager()->getPlayingMedia()->getCurrentMediaFile())
                {
                std::string ID = ControllerManager::getInstance()->getModelManager()->getPlayingMedia()->getCurrentMediaFile()->getID();
                ControllerManager::getInstance()->getPlayingMediaController()->handleInput(ID);
                }
                else {
                    status = true;
                }
                break;
            }
        }
    } while (ControllerManager::getInstance()->getViewManager()->getMainMenuView()->getSelectedOption() != MainMenu::EXIT);
}

void MainMenuController::back() {
    std::string filePath = "resources/playlists.txt";
    ControllerManager::getInstance()->getModelManager()->getPlaylistLibrary()->saveToFile(filePath);
    ControllerManager::getInstance()->getHardwareController()->stopReading();
}