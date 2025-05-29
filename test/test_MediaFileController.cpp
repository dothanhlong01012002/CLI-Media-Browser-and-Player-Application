#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MainMenuController.hpp"
#include "ControllerManager.hpp"
#include "ViewManager.hpp"
#include "ScanfOptionController.hpp"
#include "MediaFileController.hpp"
#include "PlaylistController.hpp"
#include "PlayingMediaController.hpp"
#include "PlayingMedia.hpp"
#include "ModelManager.hpp"

#include "Mock.hpp"

void mockExceptionHandler_InputAlphaStr2(const std::string& message, std::string& input, void(*validator)(const std::string&)) {
    input = "1234";
}

// Test fixture
class MediaFileControllerTest : public ::testing::Test {
protected:
    std::unique_ptr<MockControllerManager> mockControllerManager;
    std::unique_ptr<MockModelManager> mockModelManager;
    std::unique_ptr<MockViewManager> mockViewManager;
    std::unique_ptr<MockPlayingMedia> mockPlayingMedia;
    std::unique_ptr<MockMediaFileLibrary> mockMediaFileLibrary;
    std::unique_ptr<MockMediaFileView> mockMediaFileView;
    std::unique_ptr<MockPlayingMediaController> mockPlayingMediaController;
    std::unique_ptr<MockMetadataController> mockMetadataController;
    std::unique_ptr<MockHardwareController> mockHardwareController;
    std::unique_ptr<MediaFileController> mockMediaFileController;
    void SetUp() override {
        mockControllerManager = std::make_unique<MockControllerManager>();
        mockModelManager = std::make_unique<MockModelManager>();
        mockViewManager = std::make_unique<MockViewManager>();
        mockPlayingMedia = std::make_unique<MockPlayingMedia>();
        mockMediaFileLibrary = std::make_unique<MockMediaFileLibrary>();
        mockMediaFileView = std::make_unique<MockMediaFileView>();
        mockPlayingMediaController = std::make_unique<MockPlayingMediaController>();
        mockMediaFileController = std::make_unique<MediaFileController>();
        mockMetadataController = std::make_unique<MockMetadataController>();
        mockHardwareController = std::make_unique<MockHardwareController>("/dev/ttyACM0", 9600);

        MockControllerManager::SetMockInstance(mockControllerManager.get());

        EXPECT_CALL(*mockControllerManager, getHardwareController())
            .WillRepeatedly(::testing::Return(mockHardwareController.get()));

        EXPECT_CALL(*mockControllerManager, getModelManager())
            .WillRepeatedly(::testing::Return(mockModelManager.get()));

        EXPECT_CALL(*mockControllerManager, getViewManager())
            .WillRepeatedly(::testing::Return(mockViewManager.get()));

        EXPECT_CALL(*mockControllerManager, getPlayingMediaController())
            .WillRepeatedly(::testing::Return(mockPlayingMediaController.get()));

        EXPECT_CALL(*mockControllerManager, getMetadataController())
            .WillRepeatedly(::testing::Return(mockMetadataController.get()));

        EXPECT_CALL(*mockModelManager, getPlayingMedia())
            .WillRepeatedly(::testing::Return(mockPlayingMedia.get()));

        EXPECT_CALL(*mockModelManager, getMediaLibrary())
            .WillRepeatedly(::testing::Return(mockMediaFileLibrary.get()));
        
        EXPECT_CALL(*mockViewManager, getMediaFileView())
            .WillRepeatedly(::testing::Return(mockMediaFileView.get()));       

        mockMediaFileController = std::make_unique<MediaFileController>();
    }

    void TearDown() override {
        if (ControllerManager::getInstance() != nullptr) {
            MockControllerManager::SetMockInstance(nullptr);  
        }
    }
};

// Test 1: Back
TEST_F(MediaFileControllerTest, Back) {
    mockMediaFileController->back();
}

// Test 2: Page
TEST_F(MediaFileControllerTest, DisplayMediaFilesWithPagination_CallsShowMediaFilesPage) {
    std::vector<std::shared_ptr<MediaFile>> fakeMediaFiles = {
        std::make_shared<MediaFile>(), std::make_shared<MediaFile>()
    };
    EXPECT_CALL(*mockMediaFileView, showMediaFilesPage(::testing::_,::testing::_,::testing::_,::testing::_)).Times(1);

    mockMediaFileController->displayMediaFilesWithPagination(fakeMediaFiles, 1);
}

// Test 3: HandleInput_Back
TEST_F(MediaFileControllerTest, HandleInput_Back) {
    std::vector<std::shared_ptr<MediaFile>> fakeMediaFiles(5, std::make_shared<MediaFile>());


    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(1);

    EXPECT_CALL(*mockMediaFileView, getSelectedOption())
        .WillOnce(::testing::Return(MediaFileMenu::BACK_FROM_MEDIA))
        .WillOnce(::testing::Return(MediaFileMenu::BACK_FROM_MEDIA));

    EXPECT_CALL(*mockMediaFileLibrary, getAllMediaFiles())
        .WillOnce(testing::ReturnRef(fakeMediaFiles))
        .WillOnce(testing::ReturnRef(fakeMediaFiles));

    size_t totalPage = fakeMediaFiles.size() / 25 + 1;
       
    EXPECT_EQ(totalPage, 1); 
    
    MediaFileStatus status = MediaFileStatus::MEDIAFILE_NORMAL;

    EXPECT_EQ(status, MediaFileStatus::MEDIAFILE_NORMAL);  

    EXPECT_CALL(*mockMediaFileView, displayStatusMessage(::testing::_)).Times(1);
    EXPECT_CALL(*mockMediaFileView, showMediaFilesPage(::testing::_,::testing::_,::testing::_,::testing::_)).Times(1);
    EXPECT_CALL(*mockViewManager, switchView(::testing::_)).Times(1);

    mockMediaFileController->handleInput();
}

// Test 4.1: HandleInput_Play song from file if true
TEST_F(MediaFileControllerTest, HandleInput_Play_song_from_file) {

    Exception_Handler = mockExceptionHandler_InputAlphaStr2;

    std::vector<std::shared_ptr<MediaFile>> fakeMediaFiles = {
        std::make_shared<MediaFile>(),
        std::make_shared<MediaFile>()
    };
    EXPECT_CALL(*mockMediaFileLibrary, getAllMediaFiles())
        .WillOnce(testing::ReturnRef(fakeMediaFiles))
        .WillOnce(testing::ReturnRef(fakeMediaFiles))
        .WillOnce(testing::ReturnRef(fakeMediaFiles))
        .WillOnce(testing::ReturnRef(fakeMediaFiles));


    EXPECT_CALL(*mockMediaFileView, getSelectedOption())
        .WillOnce(::testing::Return(MediaFileMenu::PLAY_SONG_FROM_FILES))
        .WillOnce(::testing::Return(MediaFileMenu::BACK_FROM_MEDIA));
    
    EXPECT_CALL(*mockMediaFileLibrary, isValidMediaFileIDInLibrary(::testing::_))
        .WillOnce(::testing::Return(true));

    EXPECT_CALL(*mockPlayingMedia, setPlaylist(fakeMediaFiles)).Times(1);

    EXPECT_CALL(*mockPlayingMediaController, handleInput(::testing::_)).Times(1);
    
    size_t totalPage = fakeMediaFiles.size() / 25 + 1;

    EXPECT_CALL(*mockHardwareController, sendSignal(::testing::_)).Times(1);
    EXPECT_CALL(*mockHardwareController, sendPlayCommand()).Times(1);
      
    EXPECT_EQ(totalPage, 1); 
    
    MediaFileStatus status = MediaFileStatus::MEDIAFILE_NORMAL;

    EXPECT_EQ(status, MediaFileStatus::MEDIAFILE_NORMAL); 

    EXPECT_CALL(*mockMediaFileView, displayStatusMessage(::testing::_)).Times(1);
    EXPECT_CALL(*mockMediaFileView, showMediaFilesPage(::testing::_,::testing::_,::testing::_,::testing::_)).Times(2);
    EXPECT_CALL(*mockViewManager, switchView(::testing::_)).Times(1);
    

    mockMediaFileController->handleInput();
}

// Test 4.2: HandleInput_Play song from file if false
TEST_F(MediaFileControllerTest, HandleInput_Play_song_from_file_false) {

    Exception_Handler = mockExceptionHandler_InputAlphaStr2;

    std::vector<std::shared_ptr<MediaFile>> fakeMediaFiles = {
        std::make_shared<MediaFile>(),
        std::make_shared<MediaFile>()
    };
    EXPECT_CALL(*mockMediaFileLibrary, getAllMediaFiles())
        .WillOnce(testing::ReturnRef(fakeMediaFiles))
        .WillOnce(testing::ReturnRef(fakeMediaFiles))
        .WillOnce(testing::ReturnRef(fakeMediaFiles));


    EXPECT_CALL(*mockMediaFileView, getSelectedOption())
        .WillOnce(::testing::Return(MediaFileMenu::PLAY_SONG_FROM_FILES))
        .WillOnce(::testing::Return(MediaFileMenu::BACK_FROM_MEDIA));
    
    EXPECT_CALL(*mockMediaFileLibrary, isValidMediaFileIDInLibrary(::testing::_))
        .WillOnce(::testing::Return(false));
    
    size_t totalPage = fakeMediaFiles.size() / 25 + 1;
       
    EXPECT_EQ(totalPage, 1); 
    
    MediaFileStatus status = MediaFileStatus::MEDIAFILE_NORMAL;

    EXPECT_EQ(status, MediaFileStatus::MEDIAFILE_NORMAL); 

    EXPECT_CALL(*mockMediaFileView, displayStatusMessage(::testing::_)).Times(1);
    EXPECT_CALL(*mockMediaFileView, showMediaFilesPage(::testing::_,::testing::_,::testing::_,::testing::_)).Times(2);
    EXPECT_CALL(*mockViewManager, switchView(::testing::_)).Times(1);
    

    mockMediaFileController->handleInput();
}

// Test 5.1: HandleInput_Show_detail
TEST_F(MediaFileControllerTest, HandleInput_Show_Detail) {
        Exception_Handler = mockExceptionHandler_InputAlphaStr2;

    std::vector<std::shared_ptr<MediaFile>> fakeMediaFiles = {
        std::make_shared<MediaFile>(),
        std::make_shared<MediaFile>()
    };
    EXPECT_CALL(*mockMediaFileLibrary, getAllMediaFiles())
        .WillOnce(testing::ReturnRef(fakeMediaFiles))
        .WillOnce(testing::ReturnRef(fakeMediaFiles))
        .WillOnce(testing::ReturnRef(fakeMediaFiles));


    EXPECT_CALL(*mockMediaFileView, getSelectedOption())
        .WillOnce(::testing::Return(MediaFileMenu::SHOW_DETAIL))
        .WillOnce(::testing::Return(MediaFileMenu::BACK_FROM_MEDIA));
    
    EXPECT_CALL(*mockMediaFileLibrary, isValidMediaFileIDInLibrary(::testing::_))
        .WillOnce(::testing::Return(true));

    EXPECT_CALL(*mockMetadataController, handleInput(::testing::_)).Times(1);
    
    size_t totalPage = fakeMediaFiles.size() / 25 + 1;
       
    EXPECT_EQ(totalPage, 1); 
    
    MediaFileStatus status = MediaFileStatus::MEDIAFILE_NORMAL;

    EXPECT_EQ(status, MediaFileStatus::MEDIAFILE_NORMAL);

    EXPECT_CALL(*mockMediaFileView, displayStatusMessage(::testing::_)).Times(1);
    EXPECT_CALL(*mockMediaFileView, showMediaFilesPage(::testing::_,::testing::_,::testing::_,::testing::_)).Times(2);
    EXPECT_CALL(*mockViewManager, switchView(::testing::_)).Times(1);
    

    mockMediaFileController->handleInput();
}

// Test 5.2: HandleInput_Show_detail_false
TEST_F(MediaFileControllerTest, HandleInput_Show_Detail_False) {
        Exception_Handler = mockExceptionHandler_InputAlphaStr2;

    std::vector<std::shared_ptr<MediaFile>> fakeMediaFiles = {
        std::make_shared<MediaFile>(),
        std::make_shared<MediaFile>()
    };
    EXPECT_CALL(*mockMediaFileLibrary, getAllMediaFiles())
        .WillOnce(testing::ReturnRef(fakeMediaFiles))
        .WillOnce(testing::ReturnRef(fakeMediaFiles))
        .WillOnce(testing::ReturnRef(fakeMediaFiles));


    EXPECT_CALL(*mockMediaFileView, getSelectedOption())
        .WillOnce(::testing::Return(MediaFileMenu::SHOW_DETAIL))
        .WillOnce(::testing::Return(MediaFileMenu::BACK_FROM_MEDIA));
    
    EXPECT_CALL(*mockMediaFileLibrary, isValidMediaFileIDInLibrary(::testing::_))
        .WillOnce(::testing::Return(false));
    
    size_t totalPage = fakeMediaFiles.size() / 25 + 1;
       
    EXPECT_EQ(totalPage, 1); 
    
    MediaFileStatus status = MediaFileStatus::MEDIAFILE_NORMAL;

    EXPECT_EQ(status, MediaFileStatus::MEDIAFILE_NORMAL); 

    EXPECT_CALL(*mockMediaFileView, displayStatusMessage(::testing::_)).Times(1);
    EXPECT_CALL(*mockMediaFileView, showMediaFilesPage(::testing::_,::testing::_,::testing::_,::testing::_)).Times(2);
    EXPECT_CALL(*mockViewManager, switchView(::testing::_)).Times(1);

    mockMediaFileController->handleInput();
}

// Test 6: HandleInput_NEXT_PAGE
TEST_F(MediaFileControllerTest, HandleInput_Next_Page) {
    std::vector<std::shared_ptr<MediaFile>> fakeMediaFiles(27, std::make_shared<MediaFile>());


    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(2);

    EXPECT_CALL(*mockMediaFileView, getSelectedOption())
        .WillOnce(::testing::Return(MediaFileMenu::NEXT_PAGE))
        .WillOnce(::testing::Return(MediaFileMenu::NEXT_PAGE))
        .WillOnce(::testing::Return(MediaFileMenu::NEXT_PAGE))
        .WillOnce(::testing::Return(MediaFileMenu::BACK_FROM_MEDIA));

    EXPECT_CALL(*mockMediaFileLibrary, getAllMediaFiles())
        .WillRepeatedly(testing::ReturnRef(fakeMediaFiles));

    size_t totalPage = fakeMediaFiles.size() / 25 + 1;
       
    EXPECT_EQ(totalPage, 2); 
    
    MediaFileStatus status = MediaFileStatus::MEDIAFILE_NORMAL;

    EXPECT_EQ(status, MediaFileStatus::MEDIAFILE_NORMAL);  

    EXPECT_CALL(*mockMediaFileView, displayStatusMessage(::testing::_)).Times(2);
    EXPECT_CALL(*mockMediaFileView, showMediaFilesPage(::testing::_,::testing::_,::testing::_,::testing::_)).Times(2);
    EXPECT_CALL(*mockViewManager, switchView(::testing::_)).Times(2);

    mockMediaFileController->handleInput();

}
// Test 7: HandleInput_PREVIOUS_PAGE
TEST_F(MediaFileControllerTest, HandleInput_Previous_Page) {
    std::vector<std::shared_ptr<MediaFile>> fakeMediaFiles(27, std::make_shared<MediaFile>());

    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(3);

    EXPECT_CALL(*mockMediaFileView, getSelectedOption())
        .WillOnce(::testing::Return(MediaFileMenu::NEXT_PAGE))
        .WillOnce(::testing::Return(MediaFileMenu::NEXT_PAGE))
        .WillOnce(::testing::Return(MediaFileMenu::PREV_PAGE))
        .WillOnce(::testing::Return(MediaFileMenu::PREV_PAGE))
        .WillOnce(::testing::Return(MediaFileMenu::PREV_PAGE))
        .WillOnce(::testing::Return(MediaFileMenu::BACK_FROM_MEDIA));

    EXPECT_CALL(*mockMediaFileLibrary, getAllMediaFiles())
        .WillRepeatedly(testing::ReturnRef(fakeMediaFiles));

    size_t totalPage = fakeMediaFiles.size() / 25 + 1;
       
    EXPECT_EQ(totalPage, 2);  
    MediaFileStatus status = MediaFileStatus::MEDIAFILE_NORMAL;

    EXPECT_EQ(status, MediaFileStatus::MEDIAFILE_NORMAL);  

    EXPECT_CALL(*mockMediaFileView, displayStatusMessage(::testing::_)).Times(3);
    EXPECT_CALL(*mockMediaFileView, showMediaFilesPage(::testing::_,::testing::_,::testing::_,::testing::_)).Times(3);
    EXPECT_CALL(*mockViewManager, switchView(::testing::_)).Times(3);

    mockMediaFileController->handleInput();

}