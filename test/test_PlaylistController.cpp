#include "Mock.hpp"


class PlaylistControllerTest : public ::testing::Test {
protected:
    std::unique_ptr<MockControllerManager> mockControllerManager;
    std::unique_ptr<MockModelManager> mockModelManager;
    std::unique_ptr<MockViewManager> mockViewManager;
    std::unique_ptr<MockPlaylistView> mockPlaylistView;
    std::unique_ptr<MockPlaylistLibrary> mockPlaylistLibrary;
    std::unique_ptr<MockDetailedPlaylistController> mockDetailedPlaylistController;
    std::unique_ptr<PlaylistController> playlistController;

void SetUp() override {


    mockControllerManager = std::make_unique<MockControllerManager>();
    mockModelManager = std::make_unique<MockModelManager>();
    mockViewManager = std::make_unique<MockViewManager>();
    mockPlaylistLibrary = std::make_unique<MockPlaylistLibrary>();
    mockPlaylistView = std::make_unique<MockPlaylistView>();
    mockDetailedPlaylistController = std::make_unique<MockDetailedPlaylistController>();

    // Set ControllerManager instance
    MockControllerManager::SetMockInstance(mockControllerManager.get());

    // Set expectations
    EXPECT_CALL(*mockControllerManager, getModelManager())
        .WillRepeatedly(::testing::Return(mockModelManager.get()));

    EXPECT_CALL(*mockControllerManager, getViewManager())
        .WillRepeatedly(::testing::Return(mockViewManager.get()));

    EXPECT_CALL(*mockModelManager, getPlaylistLibrary())
        .WillRepeatedly(::testing::Return(mockPlaylistLibrary.get()));

    EXPECT_CALL(*mockViewManager, getPlaylistView())
        .WillRepeatedly(::testing::Return(mockPlaylistView.get()));

    EXPECT_CALL(*mockControllerManager, getDetailedPlaylistController())
        .WillRepeatedly(::testing::Return(mockDetailedPlaylistController.get()));

    playlistController = std::make_unique<PlaylistController>();
}
    void TearDown() override {
        if (ControllerManager::getInstance() != nullptr) {
            MockControllerManager::SetMockInstance(nullptr);
        }
    }
};

void mockExceptionHandler_PlaylistController(const std::string& str, std::string& variable, void (*func)(const std::string&)) {
    variable = "1"; // Giáº£ láº­p ngÆ°á»i dĂ¹ng nháº­p vĂ o má»™t sá»‘ há»£p lá»‡
}

void mockExceptionHandler_PlaylistName(const std::string& str, std::string& variable, void (*func)(const std::string&)){
    variable = "trung";
}

TEST_F(PlaylistControllerTest, HandleInput_SelectValidPlaylist) {
    Exception_Handler = mockExceptionHandler_PlaylistController;
    std::string validPlaylistID = "1";

    EXPECT_CALL(*mockPlaylistView, getSelectedOption())
        .WillOnce(::testing::Return(PlaylistMenu::SELECT_PLAYLIST))
        .WillOnce(::testing::Return(PlaylistMenu::BACK_FROM_PLAYLIST));

    // Thay vĂ¬ MockPlaylist, trá»±c tiáº¿p dĂ¹ng Playlist
    std::vector<std::shared_ptr<Playlist>> mockPlaylists = {
        std::make_shared<MockPlaylist>("Mocked Playlist")
    };

    EXPECT_CALL(*mockPlaylistLibrary, getAllPlaylists())
        .WillRepeatedly(::testing::ReturnRef(mockPlaylists));

    EXPECT_CALL(*mockPlaylistLibrary, isValidPlaylistIDInLibrary(validPlaylistID))
        .WillRepeatedly(::testing::Return(true));

    EXPECT_CALL(*mockViewManager, switchView(SwitchView::SW_PLAYLIST_VIEW)).Times(1);
    EXPECT_CALL(*mockPlaylistView, displayStatusMessage(::testing::_)).Times(1);
    EXPECT_CALL(*mockDetailedPlaylistController, handleInput(validPlaylistID)).Times(1);

    std::cout << ">>> Calling playlistController->handleInput();" << std::endl;

    playlistController->handleInput();
}

TEST_F(PlaylistControllerTest, HandleInput_AddPlaylist) {
    Exception_Handler = mockExceptionHandler_PlaylistController;

    // Giáº£ láº­p viá»‡c chá»n ADD_PLAYLIST trong menu
    EXPECT_CALL(*mockPlaylistView, getSelectedOption())
        .WillOnce(::testing::Return(PlaylistMenu::ADD_PLAYLIST))
        .WillOnce(::testing::Return(PlaylistMenu::BACK_FROM_PLAYLIST));  // Quay láº¡i khi hoĂ n thĂ nh

    EXPECT_CALL(*mockViewManager, switchView(SwitchView::SW_PLAYLIST_VIEW)).Times(1);
    EXPECT_CALL(*mockPlaylistView, displayStatusMessage(::testing::_)).Times(1);

    // Mock phÆ°Æ¡ng thá»©c Ä‘á»ƒ thĂªm playlist vĂ o thÆ° viá»‡n
    EXPECT_CALL(*mockPlaylistLibrary, addPlaylist(::testing::_))
        .Times(1); // Chá»‰ cáº§n kiá»ƒm tra phÆ°Æ¡ng thá»©c Ä‘Æ°á»£c gá»i má»™t láº§n

    // Giáº£ láº­p sá»± gá»i createPlaylist vá»›i tĂªn playlist
    EXPECT_CALL(*mockModelManager, getPlaylistLibrary())
        .WillRepeatedly(::testing::Return(mockPlaylistLibrary.get()));

    // Mock phÆ°Æ¡ng thá»©c hideCurrentView()
    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(1);

    // Giáº£ láº­p phÆ°Æ¡ng thá»©c getAllPlaylists() tráº£ vá» danh sĂ¡ch playlist
    std::vector<std::shared_ptr<Playlist>> mockPlaylists = {
        std::make_shared<MockPlaylist>("Mocked Playlist")
    };
    EXPECT_CALL(*mockPlaylistLibrary, getAllPlaylists())
        .WillRepeatedly(::testing::ReturnRef(mockPlaylists));

    // Gá»i handleInput Ä‘á»ƒ kiá»ƒm tra hĂ nh vi
    std::cout << ">>> Calling playlistController->handleInput();" << std::endl;
    playlistController->handleInput();
}

TEST_F(PlaylistControllerTest, HandleInput_RemovePlaylist) {
    Exception_Handler = mockExceptionHandler_PlaylistController;

    std::string playlistID = "1";  // Giáº£ láº­p ID cá»§a playlist muá»‘n xĂ³a

    // Giáº£ láº­p viá»‡c chá»n REMOVE_PLAYLIST trong menu
    EXPECT_CALL(*mockPlaylistView, getSelectedOption())
        .WillOnce(::testing::Return(PlaylistMenu::REMOVE_PLAYLIST))
        .WillOnce(::testing::Return(PlaylistMenu::BACK_FROM_PLAYLIST));  

    // Ká»³ vá»ng ráº±ng phÆ°Æ¡ng thá»©c Exception_Handler Ä‘Æ°á»£c gá»i Ä‘á»ƒ nháº­p playlist ID
    EXPECT_CALL(*mockPlaylistView, displayStatusMessage(::testing::_)).Times(1);

    // Mock phÆ°Æ¡ng thá»©c kiá»ƒm tra playlist ID há»£p lá»‡ trong thÆ° viá»‡n
    EXPECT_CALL(*mockPlaylistLibrary, isValidPlaylistIDInLibrary(playlistID))
        .WillOnce(::testing::Return(true));  // Giáº£ láº­p playlist ID há»£p lá»‡

    // Mock phÆ°Æ¡ng thá»©c Ä‘á»ƒ xĂ³a playlist tá»« thÆ° viá»‡n
    EXPECT_CALL(*mockPlaylistLibrary, removePlaylist(playlistID))
        .Times(1);  // Kiá»ƒm tra phÆ°Æ¡ng thá»©c Ä‘Æ°á»£c gá»i 1 láº§n vá»›i Ä‘Ăºng playlist ID

    // Giáº£ láº­p phÆ°Æ¡ng thá»©c hideCurrentView()
    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(1);

    // Giáº£ láº­p phÆ°Æ¡ng thá»©c getAllPlaylists() tráº£ vá» danh sĂ¡ch playlist
    std::vector<std::shared_ptr<Playlist>> mockPlaylists = {
        std::make_shared<MockPlaylist>("Mocked Playlist")
    };
    EXPECT_CALL(*mockPlaylistLibrary, getAllPlaylists())
        .WillRepeatedly(::testing::ReturnRef(mockPlaylists));

    // Gá»i handleInput Ä‘á»ƒ kiá»ƒm tra hĂ nh vi
    playlistController->handleInput();
}


TEST_F(PlaylistControllerTest, HandleInput_BackFromPlaylist) {
    // Giáº£ láº­p viá»‡c chá»n BACK_FROM_PLAYLIST trong menu
    EXPECT_CALL(*mockPlaylistView, getSelectedOption())
        .Times(2)
        .WillOnce(::testing::Return(PlaylistMenu::BACK_FROM_PLAYLIST))  // Quay láº¡i khi ngÆ°á»i dĂ¹ng chá»n BACK_FROM_PLAYLIST
        .WillOnce(::testing::Return(PlaylistMenu::BACK_FROM_PLAYLIST)); 

    // Mock phÆ°Æ¡ng thá»©c hideCurrentView()
    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(1);

    // Giáº£ láº­p phÆ°Æ¡ng thá»©c getAllPlaylists() tráº£ vá» danh sĂ¡ch playlist
    std::vector<std::shared_ptr<Playlist>> mockPlaylists = {
        std::make_shared<MockPlaylist>("Mocked Playlist")
    };
    EXPECT_CALL(*mockPlaylistLibrary, getAllPlaylists())
        .WillRepeatedly(::testing::ReturnRef(mockPlaylists));

    // Gá»i handleInput Ä‘á»ƒ kiá»ƒm tra hĂ nh vi
    std::cout << ">>> Calling playlistController->handleInput();" << std::endl;
    playlistController->handleInput();
}