#include "PlayingMediaController.hpp"
#include "ControllerManager.hpp"

void PlayingMediaController::handleInput(const std::string& ID){
    playMediaFile(ControllerManager::getInstance()->getModelManager()->getMediaLibrary()->getMediaFileByID(ID));
    do {
    updateTime();
    switch (ControllerManager::getInstance()->getViewManager()->getPlayingMediaView()->getSelectedOption())
        {
            case PlayingMediaMenu::BACK_FROM_PLAYING: {
                back();
                break;
            }
            case PlayingMediaMenu::PLAY: {
                play();
                ControllerManager::getInstance()->getHardwareController()->sendPlayCommand();
                break;
            }
            case PlayingMediaMenu::PAUSE: {
                pause();
                ControllerManager::getInstance()->getHardwareController()->sendPauseCommand();
                break;
            }
            case PlayingMediaMenu::NEXT: {
                skipToNext();
                ControllerManager::getInstance()->getHardwareController()->sendSignal(ControllerManager::getInstance()->getModelManager()->getPlayingMedia()->getDurationStringType());
                break;
            }
            case PlayingMediaMenu::PREV: {
                ControllerManager::getInstance()->getHardwareController()->sendSignal(ControllerManager::getInstance()->getModelManager()->getPlayingMedia()->getDurationStringType());
                skipToPrevious();
                break;
            }
        }
    } while (ControllerManager::getInstance()->getViewManager()->getPlayingMediaView()->getSelectedOption() != PlayingMediaMenu::BACK_FROM_PLAYING);
}

// Play the specified media file
void PlayingMediaController::playMediaFile(const std::shared_ptr<MediaFile>& file) {
    ControllerManager::getInstance()->getModelManager()->getPlayingMedia()->setCurrentMediaFile(file);
    startUpdateThread();
}


// Resume playback
void PlayingMediaController::play() {
    startUpdateThread();
    ControllerManager::getInstance()->getModelManager()->getPlayingMedia()->resumeMusic();
}

// Pause playback
void PlayingMediaController::pause() {
    stopUpdateThread();
    ControllerManager::getInstance()->getModelManager()->getPlayingMedia()->pauseMusic();
}

// Skip to the next track
void PlayingMediaController::skipToNext() {
    stopUpdateThread();
    ControllerManager::getInstance()->getModelManager()->getPlayingMedia()->nextTrack();
    startUpdateThread();
}

// Skip to the previous track
void PlayingMediaController::skipToPrevious() {
    stopUpdateThread();
    ControllerManager::getInstance()->getModelManager()->getPlayingMedia()->previousTrack();
    startUpdateThread();
}

// Adjust the volume level
void PlayingMediaController::adjustVolume(size_t level) {
    ControllerManager::getInstance()->getModelManager()->getPlayingMedia()->adjustVolume(level);
}

// Update playback time and display the current status
void PlayingMediaController::updateTime() {
    ControllerManager::getInstance()->getViewManager()->hideCurrentView();
    size_t total = ControllerManager::getInstance()->getModelManager()->getPlayingMedia()->getTotalTime();
    ControllerManager::getInstance()->getViewManager()->getPlayingMediaView()->showPlayingMedia(ControllerManager::getInstance()->getModelManager()->getPlayingMedia(),ControllerManager::getInstance()->getModelManager()->getPlayingMedia()->getCurrentTime(),total,ControllerManager::getInstance()->getModelManager()->getPlayingMedia()->getVolume());
}

void PlayingMediaController::back(){
}

// Update the elapsed playback time in a separate thread
void PlayingMediaController::updateElapsedTime() {
    auto playing = ControllerManager::getInstance()->getModelManager()->getPlayingMedia();

    while (isPlayingMediaFile.load(std::memory_order_relaxed)) {
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        playing->setCurrentTime(playing->getCurrentTime() + 1);
        if (playing->getCurrentTime() >= playing->getTotalTime()) {
            playing->nextTrack();
            ControllerManager::getInstance()->getHardwareController()->sendSignal(ControllerManager::getInstance()->getModelManager()->getPlayingMedia()->getDurationStringType());
        }
    }
}

// Start the thread for updating playback time
void PlayingMediaController::startUpdateThread() {
    if (!isPlayingMediaFile.load(std::memory_order_relaxed)) {
        isPlayingMediaFile.store(true, std::memory_order_relaxed);
        if (!updateThread.joinable()) { 
            //updateThread = std::thread(&PlayingMediaController::updateElapsedTime, this);
        }
    }
}

// Stop the thread for updating playback time
void PlayingMediaController::stopUpdateThread() {
    if (isPlayingMediaFile.load(std::memory_order_relaxed)) {
        isPlayingMediaFile.store(false, std::memory_order_relaxed);
        if (updateThread.joinable()) {
            updateThread.join();
        }
    }
}