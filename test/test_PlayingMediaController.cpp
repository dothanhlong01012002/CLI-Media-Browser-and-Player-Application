#include "Mock.hpp"

// Test fixture
class PlayingMediaControllerTest : public ::testing::Test {
protected:
    std::unique_ptr<MockControllerManager> mockControllerManager;
    std::unique_ptr<MockModelManager> mockModelManager;
    std::unique_ptr<MockViewManager> mockViewManager;
    std::unique_ptr<MockPlayingMedia> mockPlayingMedia;
    std::unique_ptr<MockMediaFileLibrary> mockMediaFileLibrary;
    std::unique_ptr<MockPlayingMediaView> mockPlayingMediaView;
    std::unique_ptr<MockHardwareController> mockHardwareController;
    std::unique_ptr<PlayingMediaController> playingMediaController;
    void SetUp() override {
        mockControllerManager = std::make_unique<MockControllerManager>();
        mockModelManager = std::make_unique<MockModelManager>();
        mockViewManager = std::make_unique<MockViewManager>();
        mockPlayingMedia = std::make_unique<MockPlayingMedia>();
        mockMediaFileLibrary = std::make_unique<MockMediaFileLibrary>();
        mockPlayingMediaView = std::make_unique<MockPlayingMediaView>();
        mockHardwareController = std::make_unique<MockHardwareController>("/dev/ttyACM0", 9600);

        MockControllerManager::SetMockInstance(mockControllerManager.get());

        EXPECT_CALL(*mockControllerManager, getModelManager())
            .WillRepeatedly(::testing::Return(mockModelManager.get()));

        EXPECT_CALL(*mockControllerManager, getViewManager())
            .WillRepeatedly(::testing::Return(mockViewManager.get()));

        EXPECT_CALL(*mockControllerManager, getHardwareController())
            .WillRepeatedly(::testing::Return(mockHardwareController.get()));

        EXPECT_CALL(*mockModelManager, getPlayingMedia())
            .WillRepeatedly(::testing::Return(mockPlayingMedia.get()));

        EXPECT_CALL(*mockModelManager, getMediaLibrary())
            .WillRepeatedly(::testing::Return(mockMediaFileLibrary.get()));

        EXPECT_CALL(*mockViewManager, getPlayingMediaView())
            .WillRepeatedly(::testing::Return(mockPlayingMediaView.get()));

        playingMediaController = std::make_unique<PlayingMediaController>();
    }

    void TearDown() override {
        // Đảm bảo set lại instance thành nullptr để tránh core dump
        if (ControllerManager::getInstance() != nullptr) {
            MockControllerManager::SetMockInstance(nullptr);  // Reset instance nếu không null
        }
    }
};

// Test 1: Adjust volume
TEST_F(PlayingMediaControllerTest, AdjustVolume_ChangesVolume) {
    EXPECT_CALL(*mockPlayingMedia, adjustVolume(50)).Times(1);
    playingMediaController->adjustVolume(50);
}

// Test 2: Set current media file
TEST_F(PlayingMediaControllerTest, PlayMediaFile_SetCurrentMediaFile) {
    EXPECT_CALL(*mockPlayingMedia, setCurrentMediaFile(::testing::_)).Times(1);
    playingMediaController->playMediaFile(std::make_shared<MediaFile>());
}

// Test 3: Resume playback
TEST_F(PlayingMediaControllerTest, Resume_Playback) {
    EXPECT_CALL(*mockPlayingMedia, resumeMusic()).Times(1);
    playingMediaController->play();
}

// Test 4: Pause playback
TEST_F(PlayingMediaControllerTest, Pause_Playback) {
    EXPECT_CALL(*mockPlayingMedia, pauseMusic()).Times(1);
    playingMediaController->pause();
}

// Test 5: Next track playback
TEST_F(PlayingMediaControllerTest, NextTrack_Playback) {
    EXPECT_CALL(*mockPlayingMedia, nextTrack()).Times(1);
    playingMediaController->skipToNext();
}

// Test 6: Previous track playback
TEST_F(PlayingMediaControllerTest, PreviousTrack_Playback) {
    EXPECT_CALL(*mockPlayingMedia, previousTrack()).Times(1);
    playingMediaController->skipToPrevious();
}

// Test 7: Update Time
TEST_F(PlayingMediaControllerTest, UpdateTime_Playback) {
    int volume = 50;
    size_t current = 50;
    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(1);
    EXPECT_CALL(*mockPlayingMedia, getTotalTime()).WillOnce(::testing::Return(100));
    EXPECT_CALL(*mockPlayingMedia, getCurrentTime()).WillOnce(::testing::ReturnRef(current));
    EXPECT_CALL(*mockPlayingMedia, getVolume()).WillOnce(::testing::ReturnRef(volume));
    EXPECT_CALL(*mockPlayingMediaView, showPlayingMedia(mockPlayingMedia.get(), ::testing::Ref(current), 100, ::testing::Ref(volume))).Times(1);
    playingMediaController->updateTime();
}

// Test 8: Back
TEST_F(PlayingMediaControllerTest, Back_Playback) {
    
    playingMediaController->back();
}

// Test 9: Start_Stop Update Thread
TEST_F(PlayingMediaControllerTest, StartStopUpdateThread_Playback) {

    playingMediaController->startUpdateThread();
    playingMediaController->stopUpdateThread();
}

// Test case
TEST_F(PlayingMediaControllerTest, UpdateElapsedTime_Playback) {
    size_t current = 5;

    // Giả lập các giá trị trả về của getCurrentTime và getTotalTime
    EXPECT_CALL(*mockPlayingMedia, getTotalTime()).WillRepeatedly(::testing::Return(10));
    EXPECT_CALL(*mockPlayingMedia, getCurrentTime()).WillRepeatedly(::testing::ReturnRef(current));

    playingMediaController->updateElapsedTime();
}

// Test 11: Handle Input : Back
TEST_F(PlayingMediaControllerTest, HandleInput_Back) {
    std::string ID = "1000";
    std::shared_ptr<MediaFile> newfile = std::make_shared<MediaFile>();
    
    EXPECT_CALL(*mockPlayingMediaView, getSelectedOption())
        .WillOnce(::testing::Return(PlayingMediaMenu::BACK_FROM_PLAYING))
        .WillOnce(::testing::Return(PlayingMediaMenu::BACK_FROM_PLAYING));
    EXPECT_CALL(*mockMediaFileLibrary, getMediaFileByID(ID))
        .WillOnce(::testing::Return(newfile));
    EXPECT_CALL(*mockPlayingMedia, setCurrentMediaFile(::testing::_)).Times(1);
    
    int volume = 50;
    size_t current = 50;
    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(1);
    EXPECT_CALL(*mockPlayingMedia, getTotalTime()).WillOnce(::testing::Return(100));
    EXPECT_CALL(*mockPlayingMedia, getCurrentTime()).WillOnce(::testing::ReturnRef(current));
    EXPECT_CALL(*mockPlayingMedia, getVolume()).WillOnce(::testing::ReturnRef(volume));
    EXPECT_CALL(*mockPlayingMediaView, showPlayingMedia(mockPlayingMedia.get(), ::testing::Ref(current), 100, ::testing::Ref(volume))).Times(1);
    playingMediaController->handleInput(ID);
}

// Test 12: Handle Input : Play
TEST_F(PlayingMediaControllerTest, HandleInput_Play) {
    std::string ID = "1000";
    std::shared_ptr<MediaFile> newfile = std::make_shared<MediaFile>();
    
    EXPECT_CALL(*mockPlayingMediaView, getSelectedOption())
        .WillOnce(::testing::Return(PlayingMediaMenu::PLAY))
        .WillOnce(::testing::Return(PlayingMediaMenu::BACK_FROM_PLAYING));
    EXPECT_CALL(*mockMediaFileLibrary, getMediaFileByID(ID))
        .WillOnce(::testing::Return(newfile));
    EXPECT_CALL(*mockPlayingMedia, setCurrentMediaFile(::testing::_)).Times(1);
    
    EXPECT_CALL(*mockHardwareController, sendPlayCommand()).Times(1);
    int volume = 50;
    size_t current = 50;
    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(1);
    EXPECT_CALL(*mockPlayingMedia, getTotalTime()).WillOnce(::testing::Return(100));
    EXPECT_CALL(*mockPlayingMedia, getCurrentTime()).WillOnce(::testing::ReturnRef(current));
    EXPECT_CALL(*mockPlayingMedia, getVolume()).WillOnce(::testing::ReturnRef(volume));
    EXPECT_CALL(*mockPlayingMediaView, showPlayingMedia(mockPlayingMedia.get(), ::testing::Ref(current), 100, ::testing::Ref(volume))).Times(1);
    
    EXPECT_CALL(*mockPlayingMedia, resumeMusic()).Times(1);
    playingMediaController->handleInput(ID);
}

// Test 12: Handle Input : Pause
TEST_F(PlayingMediaControllerTest, HandleInput_Pause) {
    std::string ID = "1000";
    std::shared_ptr<MediaFile> newfile = std::make_shared<MediaFile>();
    
    EXPECT_CALL(*mockPlayingMediaView, getSelectedOption())
        .WillOnce(::testing::Return(PlayingMediaMenu::PAUSE))
        .WillOnce(::testing::Return(PlayingMediaMenu::BACK_FROM_PLAYING));
    EXPECT_CALL(*mockMediaFileLibrary, getMediaFileByID(ID))
        .WillOnce(::testing::Return(newfile));
    EXPECT_CALL(*mockPlayingMedia, setCurrentMediaFile(::testing::_)).Times(1);
    
    EXPECT_CALL(*mockHardwareController, sendPauseCommand()).Times(1);
    int volume = 50;
    size_t current = 50;
    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(1);
    EXPECT_CALL(*mockPlayingMedia, getTotalTime()).WillOnce(::testing::Return(100));
    EXPECT_CALL(*mockPlayingMedia, getCurrentTime()).WillOnce(::testing::ReturnRef(current));
    EXPECT_CALL(*mockPlayingMedia, getVolume()).WillOnce(::testing::ReturnRef(volume));
    EXPECT_CALL(*mockPlayingMediaView, showPlayingMedia(mockPlayingMedia.get(), ::testing::Ref(current), 100, ::testing::Ref(volume))).Times(1);
    
    EXPECT_CALL(*mockPlayingMedia, pauseMusic()).Times(1);
    playingMediaController->handleInput(ID);
}

// Test 12: Handle Input : Next
TEST_F(PlayingMediaControllerTest, HandleInput_Next) {
    std::string ID = "1000";
    std::shared_ptr<MediaFile> newfile = std::make_shared<MediaFile>();
    
    EXPECT_CALL(*mockPlayingMediaView, getSelectedOption())
        .WillOnce(::testing::Return(PlayingMediaMenu::NEXT))
        .WillOnce(::testing::Return(PlayingMediaMenu::BACK_FROM_PLAYING));
    EXPECT_CALL(*mockMediaFileLibrary, getMediaFileByID(ID))
        .WillOnce(::testing::Return(newfile));
    EXPECT_CALL(*mockPlayingMedia, setCurrentMediaFile(::testing::_)).Times(1);

    EXPECT_CALL(*mockPlayingMedia, getDurationStringType())
        .WillOnce(::testing::Return("200"));
    EXPECT_CALL(*mockHardwareController, sendSignal(::testing::_)).Times(1);
    int volume = 50;
    size_t current = 50;
    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(1);
    EXPECT_CALL(*mockPlayingMedia, getTotalTime()).WillOnce(::testing::Return(100));
    EXPECT_CALL(*mockPlayingMedia, getCurrentTime()).WillOnce(::testing::ReturnRef(current));
    EXPECT_CALL(*mockPlayingMedia, getVolume()).WillOnce(::testing::ReturnRef(volume));
    EXPECT_CALL(*mockPlayingMediaView, showPlayingMedia(mockPlayingMedia.get(), ::testing::Ref(current), 100, ::testing::Ref(volume))).Times(1);
    
    EXPECT_CALL(*mockPlayingMedia, nextTrack()).Times(1);
    playingMediaController->handleInput(ID);
}

// Test 12: Handle Input : Previous
TEST_F(PlayingMediaControllerTest, HandleInput_Previous) {
    std::string ID = "1000";
    std::shared_ptr<MediaFile> newfile = std::make_shared<MediaFile>();
    
    EXPECT_CALL(*mockPlayingMediaView, getSelectedOption())
        .WillOnce(::testing::Return(PlayingMediaMenu::PREV))
        .WillOnce(::testing::Return(PlayingMediaMenu::BACK_FROM_PLAYING));
    EXPECT_CALL(*mockMediaFileLibrary, getMediaFileByID(ID))
        .WillOnce(::testing::Return(newfile));
    EXPECT_CALL(*mockPlayingMedia, setCurrentMediaFile(::testing::_)).Times(1);

    
    EXPECT_CALL(*mockPlayingMedia, getDurationStringType())
        .WillOnce(::testing::Return("200"));
    EXPECT_CALL(*mockHardwareController, sendSignal(::testing::_)).Times(1);
    int volume = 50;
    size_t current = 50;
    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(1);
    EXPECT_CALL(*mockPlayingMedia, getTotalTime()).WillOnce(::testing::Return(100));
    EXPECT_CALL(*mockPlayingMedia, getCurrentTime()).WillOnce(::testing::ReturnRef(current));
    EXPECT_CALL(*mockPlayingMedia, getVolume()).WillOnce(::testing::ReturnRef(volume));
    EXPECT_CALL(*mockPlayingMediaView, showPlayingMedia(mockPlayingMedia.get(), ::testing::Ref(current), 100, ::testing::Ref(volume))).Times(1);
    
    EXPECT_CALL(*mockPlayingMedia, previousTrack()).Times(1);
    playingMediaController->handleInput(ID);
}

