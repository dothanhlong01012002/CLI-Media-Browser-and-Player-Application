#include "Mock.hpp"
class DetailedPlaylistControllerTest : public ::testing::Test {
protected:
    std::unique_ptr<MockControllerManager> mockControllerManager;
    std::unique_ptr<MockModelManager> mockModelManager;
    std::unique_ptr<MockViewManager> mockViewManager;
    std::unique_ptr<MockPlaylistLibrary> mockPlaylistLibrary;
    std::unique_ptr<MockMediaFileLibrary> mockMediaLibrary;
    std::unique_ptr<MockDetailedPlaylistView> mockDetailedPlaylistView;
    std::unique_ptr<MockPlayingMedia> mockPlayingMedia;
    std::unique_ptr<MockHardwareController> mockHardwareController;
    std::unique_ptr<MockPlayingMediaController> mockPlayingMediaController;
    std::unique_ptr<DetailedPlaylistController> detailedPlaylistController;
    std::unique_ptr<MockMetadataController> mockMetadataController;
    std::unique_ptr<MockDetailedPlaylistController> mockDetailedPlaylistController;
    void SetUp() override {
        mockControllerManager = std::make_unique<MockControllerManager>();
        mockModelManager = std::make_unique<MockModelManager>();
        mockViewManager = std::make_unique<MockViewManager>();
        mockPlaylistLibrary = std::make_unique<MockPlaylistLibrary>();
        mockMediaLibrary = std::make_unique<MockMediaFileLibrary>();
        mockDetailedPlaylistView = std::make_unique<MockDetailedPlaylistView>();
        mockPlayingMedia = std::make_unique<MockPlayingMedia>();
        mockHardwareController = std::make_unique<MockHardwareController>("/dev/ttyACM0", 9600);
        mockPlayingMediaController = std::make_unique<MockPlayingMediaController>();
        mockDetailedPlaylistController = std::make_unique<MockDetailedPlaylistController>();
         // ThĂªm khá»Ÿi táº¡o mockMetadataController
        mockMetadataController = std::make_unique<MockMetadataController>();

        EXPECT_CALL(*mockControllerManager, getDetailedPlaylistController())
            .WillRepeatedly(::testing::Return(mockDetailedPlaylistController.get())); 

        // Sá»­a cáº¥u hĂ¬nh mock cho getMetadataController
        EXPECT_CALL(*mockControllerManager, getMetadataController())
            .WillRepeatedly(::testing::Return(mockMetadataController.get())); 

        // Thiáº¿t láº­p mock cho ControllerManager
        MockControllerManager::SetMockInstance(mockControllerManager.get());

        EXPECT_CALL(*mockControllerManager, getModelManager())
            .WillRepeatedly(::testing::Return(mockModelManager.get()));

        EXPECT_CALL(*mockControllerManager, getViewManager())
            .WillRepeatedly(::testing::Return(mockViewManager.get()));

        EXPECT_CALL(*mockControllerManager, getHardwareController())
            .WillRepeatedly(::testing::Return(mockHardwareController.get()));

        EXPECT_CALL(*mockControllerManager, getPlayingMediaController())
            .WillRepeatedly(::testing::Return(mockPlayingMediaController.get()));

        EXPECT_CALL(*mockModelManager, getPlaylistLibrary())
            .WillRepeatedly(::testing::Return(mockPlaylistLibrary.get()));

        EXPECT_CALL(*mockModelManager, getMediaLibrary())
            .WillRepeatedly(::testing::Return(mockMediaLibrary.get()));

        EXPECT_CALL(*mockModelManager, getPlayingMedia())
            .WillRepeatedly(::testing::Return(mockPlayingMedia.get()));

        EXPECT_CALL(*mockViewManager, getDetailedPlaylistView())
            .WillRepeatedly(::testing::Return(mockDetailedPlaylistView.get()));

        // Khá»Ÿi táº¡o Ä‘á»‘i tÆ°á»£ng DetailedPlaylistController
        detailedPlaylistController = std::make_unique<DetailedPlaylistController>();
    }

    void TearDown() override {
        if (ControllerManager::getInstance() != nullptr) {
            MockControllerManager::SetMockInstance(nullptr);
        }
    }
 };

 void mockExceptionHandler_DetailedPlaylist(const std::string& str, std::string& variable, void(*func)(const std::string&)) {
    variable = "1"; // Giáº£ láº­p ngÆ°á»i dĂ¹ng nháº­p vĂ o má»™t sá»‘ há»£p lá»‡
}


TEST_F(DetailedPlaylistControllerTest, HandleInput_ShowDetailSong_ValidSongID) {
    Exception_Handler = mockExceptionHandler_DetailedPlaylist;
    std::string listId = "1";
    std::string validSongID = "1";
    
    // Giáº£ láº­p hĂ nh vi cá»§a getSelectedOption Ä‘á»ƒ chá»n SHOW_DETAIL_SONG
    EXPECT_CALL(*mockDetailedPlaylistView, getSelectedOption())
        .WillOnce(::testing::Return(DetailedPlaylistMenu::SHOW_DETAIL_SONG))
        .WillOnce(::testing::Return(DetailedPlaylistMenu::BACK_FROM_DETAIL));
    
    EXPECT_CALL(*mockDetailedPlaylistView, displayStatusMessage(::testing::_)).Times(::testing::AnyNumber());
    EXPECT_CALL(*mockViewManager, switchView(SwitchView::SW_DETAILED_VIEW)).Times(::testing::AnyNumber());
    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(::testing::AnyNumber());

    // Táº¡o má»™t mock Playlist vá»›i tĂªn há»£p lá»‡
    auto mockPlaylist = std::make_shared<MockPlaylist>("Mocked Playlist");

    // Giáº£ láº­p Ä‘á»ƒ getPlaylistByID() luĂ´n tráº£ vá» mockPlaylist
    EXPECT_CALL(*mockPlaylistLibrary, getPlaylistByID(listId))
        .WillRepeatedly(::testing::Return(mockPlaylist));

    // Giáº£ láº­p hĂ nh vi cá»§a isValidMediaFileIDInPlaylist Ä‘á»ƒ tráº£ vá» true
    EXPECT_CALL(*mockPlaylist, isValidMediaFileIDInPlaylist(validSongID))
        .WillRepeatedly(::testing::Return(true));  // <-- Äáº£m báº£o luĂ´n tráº£ vá» true

    // Giáº£ láº­p gá»i handleInput cá»§a MetadataController khi cĂ³ ID há»£p lá»‡
    EXPECT_CALL(*mockMetadataController, handleInput(validSongID)).Times(1);

    // Thá»±c thi hĂ m handleInput
    detailedPlaylistController->handleInput(listId);
}

TEST_F(DetailedPlaylistControllerTest, HandleInput_AddSong_ValidSongID) {
    Exception_Handler = mockExceptionHandler_DetailedPlaylist;

     std::string listId = "1";
    std::string validSongID = "1";

    auto mockPlaylist = std::make_shared<MockPlaylist>("Mocked Playlist");
    auto mockMediaFile = std::make_shared<MockMediaFile>("Mock Song", validSongID);

    EXPECT_CALL(*mockPlaylistLibrary, getPlaylistByID(listId))
        .WillRepeatedly(::testing::Return(mockPlaylist));

    EXPECT_CALL(*mockMediaLibrary, getMediaFileByID(validSongID))
        .WillRepeatedly(::testing::Return(mockMediaFile));

    // Sá»­a lá»—i: Ă‰p kiá»ƒu vá» `std::shared_ptr<MediaFile>`
    EXPECT_CALL(*mockPlaylist, addSong(std::static_pointer_cast<MediaFile>(mockMediaFile)))
        .Times(1);

    EXPECT_CALL(*mockMediaLibrary, isValidMediaFileIDInLibrary(validSongID))
        .WillRepeatedly(::testing::Return(true));

    EXPECT_CALL(*mockPlaylist, isValidMediaFileIDInPlaylist(validSongID))
        .WillRepeatedly(::testing::Return(false));

    EXPECT_CALL(*mockDetailedPlaylistView, getSelectedOption())
        .WillOnce(::testing::Return(DetailedPlaylistMenu::ADD_SONG))
        .WillOnce(::testing::Return(DetailedPlaylistMenu::BACK_FROM_DETAIL));

    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(::testing::AnyNumber());
    EXPECT_CALL(*mockViewManager, switchView(::testing::_)).Times(::testing::AnyNumber());
    EXPECT_CALL(*mockDetailedPlaylistView, displayStatusMessage(::testing::_)).Times(::testing::AnyNumber());

    // **Gá»i hĂ m Ä‘á»ƒ test**
    detailedPlaylistController->handleInput(listId);
}

TEST_F(DetailedPlaylistControllerTest, HandleInput_DeleteSong_ValidSongID) {
    Exception_Handler = mockExceptionHandler_DetailedPlaylist;

    std::string listId = "1";
    std::string validSongID = "1";

    // Giáº£ láº­p playlist há»£p lá»‡
    auto mockPlaylist = std::make_shared<MockPlaylist>("Mocked Playlist");

    // Giáº£ láº­p `getPlaylistByID()`
    EXPECT_CALL(*mockPlaylistLibrary, getPlaylistByID(listId))
        .WillRepeatedly(::testing::Return(mockPlaylist));

    // Giáº£ láº­p bĂ i hĂ¡t tá»“n táº¡i trong playlist
    EXPECT_CALL(*mockPlaylist, isValidMediaFileIDInPlaylist(validSongID))
        .WillRepeatedly(::testing::Return(true));

    // Giáº£ láº­p gá»i `removeSong()` khi bĂ i hĂ¡t há»£p lá»‡
    EXPECT_CALL(*mockPlaylist, removeSong(validSongID))
        .Times(1);

    // Giáº£ láº­p chá»n `DELETE_SONG` rá»“i `BACK_FROM_DETAIL`
    EXPECT_CALL(*mockDetailedPlaylistView, getSelectedOption())
        .WillOnce(::testing::Return(DetailedPlaylistMenu::DELETE_SONG))
        .WillOnce(::testing::Return(DetailedPlaylistMenu::BACK_FROM_DETAIL));

    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(::testing::AnyNumber());
    EXPECT_CALL(*mockViewManager, switchView(::testing::_)).Times(::testing::AnyNumber());
    EXPECT_CALL(*mockDetailedPlaylistView, displayStatusMessage(::testing::_)).Times(::testing::AnyNumber());

    // **Gá»i hĂ m Ä‘á»ƒ test**
    detailedPlaylistController->handleInput(listId);
}

TEST_F(DetailedPlaylistControllerTest, HandleInput_PlaySong_ValidSongID) {
    // Giáº£ láº­p ngÆ°á»i dĂ¹ng nháº­p ID bĂ i hĂ¡t há»£p lá»‡
    Exception_Handler = mockExceptionHandler_DetailedPlaylist;

     std::string listId = "1";
    std::string validSongID = "1";

    // Táº¡o mock PlaylistLibrary
    auto mockPlaylistLibrary = std::make_shared<MockPlaylistLibrary>();

    // Táº¡o mock Playlist
    auto mockPlaylist = std::make_shared<MockPlaylist>("Mocked Playlist");

    // Táº¡o mock PlayingMedia
    auto mockPlayingMedia = std::make_shared<MockPlayingMedia>();

    // Táº¡o bĂ i hĂ¡t giáº£ láº­p há»£p lá»‡
    auto mockMediaFile = std::make_shared<MockMediaFile>("Mock Song", validSongID);

    // Giáº£ láº­p danh sĂ¡ch bĂ i hĂ¡t trong playlist (KhĂ´ng Ä‘Æ°á»£c rá»—ng)
    std::vector<std::shared_ptr<MediaFile>> mockSongs = { mockMediaFile };

    // Giáº£ láº­p `getPlaylistLibrary()` trĂ¡nh nullptr
    EXPECT_CALL(*mockModelManager, getPlaylistLibrary())
        .WillRepeatedly(::testing::Return(mockPlaylistLibrary.get()));

    // Giáº£ láº­p `getPlaylistByID()` Ä‘á»ƒ trĂ¡nh nullptr
    EXPECT_CALL(*mockPlaylistLibrary, getPlaylistByID(listId))
        .WillRepeatedly(::testing::Return(mockPlaylist));

    // Giáº£ láº­p bĂ i hĂ¡t tá»“n táº¡i trong playlist
    EXPECT_CALL(*mockPlaylist, isValidMediaFileIDInPlaylist(validSongID))
        .WillRepeatedly(::testing::Return(true));

    // Giáº£ láº­p `getSongs()` trĂ¡nh danh sĂ¡ch rá»—ng
    EXPECT_CALL(*mockPlaylist, getSongs())
        .WillRepeatedly(::testing::ReturnRef(mockSongs));

    // Giáº£ láº­p láº¥y `PlayingMedia` trĂ¡nh nullptr
    EXPECT_CALL(*mockModelManager, getPlayingMedia())
        .WillRepeatedly(::testing::Return(mockPlayingMedia.get()));

    // Giáº£ láº­p `getMediaFileByID()` Ä‘á»ƒ tráº£ vá» bĂ i hĂ¡t há»£p lá»‡
    EXPECT_CALL(*mockMediaLibrary, getMediaFileByID(validSongID))
        .WillRepeatedly(::testing::Return(mockMediaFile));

    // Giáº£ láº­p `setPlaylist()`
    EXPECT_CALL(*mockPlayingMedia, setPlaylist(mockSongs))
        .Times(1);

    // Giáº£ láº­p `getDurationStringType()`
    std::string mockDuration = "03:45";
    EXPECT_CALL(*mockPlayingMedia, getDurationStringType())
        .WillRepeatedly(::testing::Return(mockDuration));

    // Giáº£ láº­p gá»­i tĂ­n hiá»‡u vĂ  play
    EXPECT_CALL(*mockHardwareController, sendSignal(mockDuration))
        .Times(1);
    EXPECT_CALL(*mockHardwareController, sendPlayCommand())
        .Times(1);

    // Giáº£ láº­p `handleInput(songID)` cá»§a PlayingMediaController
    EXPECT_CALL(*mockPlayingMediaController, handleInput(validSongID))
        .Times(1);

    // **ThĂªm kiá»ƒm tra `setCurrentMediaFile()` Ä‘á»ƒ trĂ¡nh lá»—i Segmentation Fault**
    EXPECT_CALL(*mockPlayingMedia, setCurrentMediaFile(::testing::_))
        .Times(::testing::AnyNumber());

    // **ThĂªm kiá»ƒm tra `showListOfSongs()` Ä‘á»ƒ trĂ¡nh lá»—i**
    EXPECT_CALL(*mockDetailedPlaylistView, showListOfSongs(::testing::_))
        .Times(::testing::AnyNumber());

    // Giáº£ láº­p chá»n `PLAY_SONG_FROM_PLAYLIST` rá»“i `BACK_FROM_DETAIL`
    EXPECT_CALL(*mockDetailedPlaylistView, getSelectedOption())
        .WillOnce(::testing::Return(DetailedPlaylistMenu::PLAY_SONG_FROM_PLAYLIST))
        .WillOnce(::testing::Return(DetailedPlaylistMenu::BACK_FROM_DETAIL));

    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(::testing::AnyNumber());
    EXPECT_CALL(*mockViewManager, switchView(::testing::_)).Times(::testing::AnyNumber());
    EXPECT_CALL(*mockDetailedPlaylistView, displayStatusMessage(::testing::_)).Times(::testing::AnyNumber());

    // **Gá»i hĂ m Ä‘á»ƒ test**
    detailedPlaylistController->handleInput(listId);
}

TEST_F(DetailedPlaylistControllerTest, HandleInput_BackFromDetail) {
    std::string listId = "1";

    // **Táº¡o mock Playlist há»£p lá»‡**
    auto mockPlaylist = std::make_shared<MockPlaylist>("Mocked Playlist");

    EXPECT_CALL(*mockPlaylistLibrary, getPlaylistByID(listId))
        .WillRepeatedly(::testing::Return(mockPlaylist));

    // **Äáº£m báº£o `getSelectedOption()` gá»i Ä‘Ăºng 2 láº§n**
    EXPECT_CALL(*mockDetailedPlaylistView, getSelectedOption())
        .Times(2)
        .WillOnce(::testing::Return(DetailedPlaylistMenu::BACK_FROM_DETAIL))
        .WillOnce(::testing::Return(DetailedPlaylistMenu::BACK_FROM_DETAIL));


    // **Thá»±c thi `handleInput()`**
    detailedPlaylistController->handleInput(listId);
}

TEST_F(DetailedPlaylistControllerTest, HandleInput_ShowDetailSong_InvalidSongID) {
    Exception_Handler = mockExceptionHandler_DetailedPlaylist;
    std::string listId = "1";
    std::string invalidSongID = "999";  // Giáº£ láº­p ID bĂ i hĂ¡t khĂ´ng há»£p lá»‡

    // Giáº£ láº­p hĂ nh vi cá»§a getSelectedOption Ä‘á»ƒ chá»n SHOW_DETAIL_SONG
    EXPECT_CALL(*mockDetailedPlaylistView, getSelectedOption())
        .WillOnce(::testing::Return(DetailedPlaylistMenu::SHOW_DETAIL_SONG))
        .WillOnce(::testing::Return(DetailedPlaylistMenu::BACK_FROM_DETAIL));

    EXPECT_CALL(*mockDetailedPlaylistView, displayStatusMessage(::testing::_)).Times(::testing::AnyNumber());
    EXPECT_CALL(*mockViewManager, switchView(SwitchView::SW_DETAILED_VIEW)).Times(::testing::AnyNumber());
    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(::testing::AnyNumber());

    // Táº¡o má»™t mock Playlist vá»›i tĂªn há»£p lá»‡
    auto mockPlaylist = std::make_shared<MockPlaylist>("Mocked Playlist");

    // Giáº£ láº­p Ä‘á»ƒ getPlaylistByID() luĂ´n tráº£ vá» mockPlaylist
    EXPECT_CALL(*mockPlaylistLibrary, getPlaylistByID(listId))
        .WillRepeatedly(::testing::Return(mockPlaylist));

    // Giáº£ láº­p hĂ nh vi cá»§a isValidMediaFileIDInPlaylist Ä‘á»ƒ tráº£ vá» false (bĂ i hĂ¡t khĂ´ng há»£p lá»‡)
    EXPECT_CALL(*mockPlaylist, isValidMediaFileIDInPlaylist(::testing::_))
    .WillRepeatedly(::testing::Return(false));  // Cháº¥p nháº­n báº¥t ká»³ ID nĂ o vĂ  tráº£ vá» false

    // Giáº£ láº­p tráº¡ng thĂ¡i hiá»ƒn thá»‹ thĂ´ng bĂ¡o lá»—i DETAILED_SHOW_STATUS
    EXPECT_CALL(*mockDetailedPlaylistView, displayStatusMessage(::testing::_))
        .Times(1);

    // Thá»±c thi hĂ m handleInput
    detailedPlaylistController->handleInput(listId);
}

// TEST_F(DetailedPlaylistControllerTest, HandleInput_AddSong_InvalidSongID) {
//     Exception_Handler = mockExceptionHandler_DetailedPlaylist;

//     std::string listId = "1";
//     std::string invalidSongID = "999";

//     auto mockPlaylist = std::make_shared<MockPlaylist>("Mocked Playlist");

//     EXPECT_CALL(*mockPlaylistLibrary, getPlaylistByID(listId))
//         .WillRepeatedly(::testing::Return(mockPlaylist));

//     EXPECT_CALL(*mockMediaLibrary, isValidMediaFileIDInLibrary(::testing::_))
//         .WillRepeatedly(::testing::Return(false));  // BĂ i hĂ¡t khĂ´ng tá»“n táº¡i

//     EXPECT_CALL(*mockDetailedPlaylistView, getSelectedOption())
//         .WillOnce(::testing::Return(DetailedPlaylistMenu::ADD_SONG))
//         .WillOnce(::testing::Return(DetailedPlaylistMenu::BACK_FROM_DETAIL));

//     // Kiá»ƒm tra ráº±ng `DETAILED_ADD_STATUS` Ä‘Æ°á»£c hiá»ƒn thá»‹ khi bĂ i hĂ¡t khĂ´ng há»£p lá»‡
//     EXPECT_CALL(*mockDetailedPlaylistView, displayStatusMessage(DetailedPlaylistStatus::DETAILED_ADD_STATUS))
//         .Times(1);

//     EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(::testing::AnyNumber());
//     EXPECT_CALL(*mockViewManager, switchView(::testing::_)).Times(::testing::AnyNumber());

//     // **Gá»i hĂ m handleInput Ä‘á»ƒ kiá»ƒm tra**
//     detailedPlaylistController->handleInput(listId);
// }

TEST_F(DetailedPlaylistControllerTest, HandleInput_AddSong_AlreadyInPlaylist) {
    Exception_Handler = mockExceptionHandler_DetailedPlaylist;

    std::string listId = "1";
    std::string existingSongID = "2";

    auto mockPlaylist = std::make_shared<MockPlaylist>("Mocked Playlist");

    EXPECT_CALL(*mockPlaylistLibrary, getPlaylistByID(listId))
        .WillRepeatedly(::testing::Return(mockPlaylist));

    EXPECT_CALL(*mockMediaLibrary, isValidMediaFileIDInLibrary(::testing::_))
        .WillRepeatedly(::testing::Return(true));

    EXPECT_CALL(*mockPlaylist, isValidMediaFileIDInPlaylist(::testing::_))
        .WillRepeatedly(::testing::Return(true));  // BĂ i hĂ¡t Ä‘Ă£ cĂ³ trong playlist

    EXPECT_CALL(*mockDetailedPlaylistView, getSelectedOption())
        .WillOnce(::testing::Return(DetailedPlaylistMenu::ADD_SONG))
        .WillOnce(::testing::Return(DetailedPlaylistMenu::BACK_FROM_DETAIL));

    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(::testing::AnyNumber());
    EXPECT_CALL(*mockViewManager, switchView(::testing::_)).Times(::testing::AnyNumber());

    // **Gá»i hĂ m handleInput Ä‘á»ƒ kiá»ƒm tra**
    detailedPlaylistController->handleInput(listId);
}