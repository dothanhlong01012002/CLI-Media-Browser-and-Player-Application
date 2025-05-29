#include "Mock.hpp"

// Test fixture
class MainMenuControllerTest : public ::testing::Test {
protected:
    std::unique_ptr<MockControllerManager> mockControllerManager;
    std::unique_ptr<MockModelManager> mockModelManager;
    std::unique_ptr<MockViewManager> mockViewManager;

    std::unique_ptr<MockPlayingMedia> mockPlayingMedia;
    std::unique_ptr<MockMediaFileLibrary> mockMediaFileLibrary;
    std::unique_ptr<MockPlaylistLibrary> mockPlaylistLibrary;

    std::unique_ptr<MockPlayingMediaView> mockPlayingMediaView;
    std::unique_ptr<MockMainMenuView> mockMainMenuView;
    
    std::unique_ptr<MockPlayingMediaController> mockPlayingMediaController;
    std::unique_ptr<MockHardwareController> mockHardwareController;
    std::unique_ptr<MockScanfOptionController> mockScanfOptionController;
    std::unique_ptr<MockMediaFileController> mockMediaFileController;
    std::unique_ptr<MockPlaylistController> mockPlaylistController;
    std::unique_ptr<MainMenuController> mainMenuController;

    void SetUp() override {
        mockControllerManager = std::make_unique<MockControllerManager>();
        mockModelManager = std::make_unique<MockModelManager>();
        mockViewManager = std::make_unique<MockViewManager>();

        mockPlayingMedia = std::make_unique<MockPlayingMedia>();
        mockMediaFileLibrary = std::make_unique<MockMediaFileLibrary>();
        mockPlaylistLibrary = std::make_unique<MockPlaylistLibrary>();

        mockPlayingMediaView = std::make_unique<MockPlayingMediaView>();
        mockMainMenuView = std::make_unique<MockMainMenuView>();

        mockPlayingMediaController = std::make_unique<MockPlayingMediaController>();
        mockHardwareController = std::make_unique<MockHardwareController>("/dev/ttyACM0", 9600);
        mockScanfOptionController = std::make_unique<MockScanfOptionController>();
        mockMediaFileController = std::make_unique<MockMediaFileController>();
        mockPlaylistController = std::make_unique<MockPlaylistController>();
        MockControllerManager::SetMockInstance(mockControllerManager.get());

        EXPECT_CALL(*mockControllerManager, getModelManager())
            .WillRepeatedly(::testing::Return(mockModelManager.get()));

        EXPECT_CALL(*mockControllerManager, getViewManager())
            .WillRepeatedly(::testing::Return(mockViewManager.get()));

        EXPECT_CALL(*mockControllerManager, getHardwareController())
            .WillRepeatedly(::testing::Return(mockHardwareController.get()));

        EXPECT_CALL(*mockControllerManager, getScanfOptionController())
            .WillRepeatedly(::testing::Return(mockScanfOptionController.get()));

        EXPECT_CALL(*mockControllerManager, getPlaylistController())
            .WillRepeatedly(::testing::Return(mockPlaylistController.get()));

        EXPECT_CALL(*mockControllerManager, getMediaFileController())
            .WillRepeatedly(::testing::Return(mockMediaFileController.get()));

        EXPECT_CALL(*mockControllerManager, getPlayingMediaController())
            .WillRepeatedly(::testing::Return(mockPlayingMediaController.get()));

        EXPECT_CALL(*mockModelManager, getPlayingMedia())
            .WillRepeatedly(::testing::Return(mockPlayingMedia.get()));

        EXPECT_CALL(*mockModelManager, getMediaLibrary())
            .WillRepeatedly(::testing::Return(mockMediaFileLibrary.get()));

        EXPECT_CALL(*mockModelManager, getPlaylistLibrary())
            .WillRepeatedly(::testing::Return(mockPlaylistLibrary.get()));

        EXPECT_CALL(*mockViewManager, getPlayingMediaView())
            .WillRepeatedly(::testing::Return(mockPlayingMediaView.get()));

        EXPECT_CALL(*mockViewManager, getMainMenuView())
            .WillRepeatedly(::testing::Return(mockMainMenuView.get()));

        mainMenuController = std::make_unique<MainMenuController>();
    }

    void TearDown() override {
        // Đảm bảo set lại instance thành nullptr để tránh core dump
        if (ControllerManager::getInstance() != nullptr) {
            MockControllerManager::SetMockInstance(nullptr);  // Reset instance nếu không null
        }
    }
};

TEST_F(MainMenuControllerTest, HandleInput_Test) {
    std::shared_ptr<MediaFile> file = std::make_shared<MediaFile>();

    EXPECT_CALL(*mockHardwareController, startReading()).Times(1);
    EXPECT_CALL(*mockHardwareController, stopReading()).Times(1);
    EXPECT_CALL(*mockScanfOptionController, scanPlaylistsFromTxt(::testing::_)).Times(1);
    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(5);
    EXPECT_CALL(*mockViewManager, switchView(::testing::_)).Times(5);

    EXPECT_CALL(*mockScanfOptionController, handleInput()).Times(1);
    EXPECT_CALL(*mockMediaFileController, handleInput()).Times(1);
    EXPECT_CALL(*mockPlaylistController, handleInput()).Times(1);
    EXPECT_CALL(*mockPlayingMediaController, handleInput(::testing::_)).Times(1);

    EXPECT_CALL(*mockPlayingMedia, getCurrentMediaFile())
        .WillRepeatedly(::testing::Return(file));

    EXPECT_CALL(*mockMainMenuView, getSelectedOption())
        .WillOnce(::testing::Return(MainMenu::SCAN_OPTIONS))
        .WillOnce(::testing::Return(MainMenu::SCAN_OPTIONS))
        .WillOnce(::testing::Return(MainMenu::ALL_SONGS))
        .WillOnce(::testing::Return(MainMenu::ALL_SONGS))
        .WillOnce(::testing::Return(MainMenu::PLAYLIST))
        .WillOnce(::testing::Return(MainMenu::PLAYLIST))
        .WillOnce(::testing::Return(MainMenu::NOW_PLAYING))
        .WillOnce(::testing::Return(MainMenu::NOW_PLAYING))
        .WillOnce(::testing::Return(MainMenu::EXIT))
        .WillOnce(::testing::Return(MainMenu::EXIT));
    EXPECT_CALL(*mockPlaylistLibrary, saveToFile(::testing::_)).Times(1);
    mainMenuController->handleInput();
}