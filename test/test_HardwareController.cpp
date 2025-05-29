#include "Mock.hpp"

class HardwareControllerTest : public ::testing::Test {
protected:
    std::unique_ptr<MockControllerManager> mockControllerManager;
    std::unique_ptr<MockModelManager> mockModelManager;
    std::unique_ptr<MockViewManager> mockViewManager;
    std::unique_ptr<MockPlaylistLibrary> mockPlaylistLibrary;
    std::unique_ptr<MockMediaFileLibrary> mockMediaLibrary;
    std::unique_ptr<MockHardwareController> mockHardwareController;
    std::unique_ptr<HardwareController> hardwareController;
    std::unique_ptr<MockPlayingMediaController> mockPlayingMediaController;
    std::unique_ptr<MockPlayingMedia> mockPlayingMedia;  

    void SetUp() override {
        // Táº¡o cĂ¡c mock cho ControllerManager vĂ  cĂ¡c thĂ nh pháº§n khĂ¡c
        mockControllerManager = std::make_unique<MockControllerManager>();
        mockModelManager = std::make_unique<MockModelManager>();
        mockViewManager = std::make_unique<MockViewManager>();
        mockPlaylistLibrary = std::make_unique<MockPlaylistLibrary>();
        mockMediaLibrary = std::make_unique<MockMediaFileLibrary>();
        mockHardwareController = std::make_unique<MockHardwareController>("/dev/ttyACM0", 9600);  // Giáº£ láº­p port UART
        mockPlayingMediaController = std::make_unique<MockPlayingMediaController>();
        mockPlayingMedia = std::make_unique<MockPlayingMedia>(); 

        // Thiáº¿t láº­p cĂ¡c mock cho cĂ¡c phÆ°Æ¡ng thá»©c cá»§a ControllerManager
        EXPECT_CALL(*mockControllerManager, getPlayingMediaController())
            .WillRepeatedly(::testing::Return(mockPlayingMediaController.get()));

        // Thiáº¿t láº­p cĂ¡c mock cho ModelManager vĂ  ViewManager
        EXPECT_CALL(*mockControllerManager, getModelManager())
            .WillRepeatedly(::testing::Return(mockModelManager.get()));

        EXPECT_CALL(*mockControllerManager, getViewManager())
            .WillRepeatedly(::testing::Return(mockViewManager.get()));

        EXPECT_CALL(*mockControllerManager, getHardwareController())
            .WillRepeatedly(::testing::Return(mockHardwareController.get()));

        EXPECT_CALL(*mockModelManager, getPlayingMedia())
            .WillRepeatedly(::testing::Return(mockPlayingMedia.get()));

        // Thiáº¿t láº­p instance cá»§a ControllerManager vá»›i mock
        MockControllerManager::SetMockInstance(mockControllerManager.get());

        // Khá»Ÿi táº¡o Ä‘á»‘i tÆ°á»£ng HardwareController
        hardwareController = std::make_unique<HardwareController>("/dev/ttyACM0", 9600);
    }

    void TearDown() override {
        if (ControllerManager::getInstance() != nullptr) {
            MockControllerManager::SetMockInstance(nullptr);  // Reset instance sau khi test xong
        }
    }
};
TEST_F(HardwareControllerTest, SendPlayCommand) {
    hardwareController->sendPlayCommand();
}

TEST_F(HardwareControllerTest, SendPauseCommand) {
    hardwareController->sendPauseCommand();
}

TEST_F(HardwareControllerTest, SendStopCommand) {
    hardwareController->sendStopCommand();
}

TEST_F(HardwareControllerTest, SendCustomSignal) {
    std::string customSignal = "Custom Signal";
    hardwareController->sendSignal(customSignal);
}

TEST_F(HardwareControllerTest, startReading) {
    hardwareController->startReading();
}

TEST_F(HardwareControllerTest, HandleReceivedData_Play) {
    // Ká»³ vá»ng ráº±ng khi nháº­n dá»¯ liá»‡u PLAY_MODE_RECEIVE, phÆ°Æ¡ng thá»©c play() sáº½ Ä‘Æ°á»£c gá»i
    EXPECT_CALL(*mockPlayingMediaController, play()).Times(1);

    hardwareController->handleReceivedData(PLAY_MODE_RECEIVE);
}

// Test PAUSE_MODE_RECEIVE
TEST_F(HardwareControllerTest, HandleReceivedData_Pause) {
    // Ká»³ vá»ng ráº±ng khi nháº­n dá»¯ liá»‡u PAUSE_MODE_RECEIVE, phÆ°Æ¡ng thá»©c pause() sáº½ Ä‘Æ°á»£c gá»i
    EXPECT_CALL(*mockPlayingMediaController, pause()).Times(1);

    hardwareController->handleReceivedData(PAUSE_MODE_RECEIVE);
}

// Test NEXT_MODE_RECEIVE
TEST_F(HardwareControllerTest, HandleReceivedData_Next) {
    // Ká»³ vá»ng ráº±ng khi nháº­n dá»¯ liá»‡u NEXT_MODE_RECEIVE, phÆ°Æ¡ng thá»©c skipToNext() sáº½ Ä‘Æ°á»£c gá»i
    EXPECT_CALL(*mockPlayingMediaController, skipToNext()).Times(1);

    hardwareController->handleReceivedData(NEXT_MODE_RECEIVE);
}

// Test PREV_MODE_RECEIVE
TEST_F(HardwareControllerTest, HandleReceivedData_Previous) {
    // Ká»³ vá»ng ráº±ng khi nháº­n dá»¯ liá»‡u PREV_MODE_RECEIVE, phÆ°Æ¡ng thá»©c skipToPrevious() sáº½ Ä‘Æ°á»£c gá»i
    EXPECT_CALL(*mockPlayingMediaController, skipToPrevious()).Times(1);

    hardwareController->handleReceivedData(PREV_MODE_RECEIVE);
}

// Test Volume Adjustment with data
TEST_F(HardwareControllerTest, HandleReceivedData_Volume) {
    std::string volumeData = "50";  // Giáº£ láº­p nháº­n dá»¯ liá»‡u Ä‘iá»u chá»‰nh Ă¢m lÆ°á»£ng

    // Mock phÆ°Æ¡ng thá»©c setVolume() Ä‘á»ƒ Ä‘áº£m báº£o nĂ³ Ä‘Æ°á»£c gá»i vá»›i giĂ¡ trá»‹ Ă¢m lÆ°á»£ng Ä‘Ăºng
    EXPECT_CALL(*mockModelManager, getPlayingMedia())
        .WillRepeatedly(::testing::Return(mockPlayingMedia.get()));
    EXPECT_CALL(*mockPlayingMedia, setVolume(50)).Times(1);

    hardwareController->handleReceivedData(volumeData);  // Gá»i handleReceivedData vá»›i dá»¯ liá»‡u Ă¢m lÆ°á»£ng
}