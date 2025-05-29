#include "Mock.hpp"
#include <filesystem>
#include <taglib/fileref.h>

namespace fs = std::filesystem;

void mockExceptionHandler_InputAlphaStr(const std::string& message, std::string& input, void(*validator)(const std::string&)) {
    input = "1234";
}

class MetadataControllerTest : public ::testing::Test {
protected:
    std::unique_ptr<MockControllerManager> mockControllerManager;
    std::unique_ptr<MockModelManager> mockModelManager;
    std::unique_ptr<MockViewManager> mockViewManager;
    std::unique_ptr<MockMediaFileLibrary> mockMediaFileLibrary;
    std::shared_ptr<MockMediaFile> mockMediaFile;
    MockMetadata mockMetadata;
    std::unique_ptr<MockScanfOptionView> mockScanfOptionView;
    std::unique_ptr<MockMetadataView> mockMetadataView;
    std::unique_ptr<MockPlaylistLibrary> mockPlaylistLibrary;
    std::unique_ptr<MetadataController> metadataController;

    // Tạo thư mục giả lập trong thư mục /media/<current_user>/
    void SetUp() override {
        mockControllerManager = std::make_unique<MockControllerManager>();
        mockModelManager = std::make_unique<MockModelManager>();
        mockViewManager = std::make_unique<MockViewManager>();
        mockScanfOptionView = std::make_unique<MockScanfOptionView>();
        mockMetadataView = std::make_unique<MockMetadataView>();
        mockPlaylistLibrary = std::make_unique<MockPlaylistLibrary>();
        mockMediaFile = std::make_shared<MockMediaFile>("aaa","111");
        mockMediaFileLibrary = std::make_unique<MockMediaFileLibrary>();

        MockControllerManager::SetMockInstance(mockControllerManager.get());

        EXPECT_CALL(*mockControllerManager, getModelManager())
            .WillRepeatedly(::testing::Return(mockModelManager.get()));

        EXPECT_CALL(*mockControllerManager, getViewManager())
            .WillRepeatedly(::testing::Return(mockViewManager.get()));

        EXPECT_CALL(*mockModelManager, getMediaLibrary())
            .WillRepeatedly(::testing::Return(mockMediaFileLibrary.get()));

        EXPECT_CALL(*mockModelManager, getPlaylistLibrary())
            .WillRepeatedly(::testing::Return(mockPlaylistLibrary.get()));

        EXPECT_CALL(*mockViewManager, getScanfOptionView())
            .WillRepeatedly(::testing::Return(mockScanfOptionView.get()));

        EXPECT_CALL(*mockViewManager, getMetadataView())
            .WillRepeatedly(::testing::Return(mockMetadataView.get()));

        EXPECT_CALL(*mockMediaFile, getMetadata())
            .WillRepeatedly(::testing::ReturnRef(mockMetadata));
        // Khởi tạo controller
        metadataController = std::make_unique<MetadataController>();
    }

    // Xóa các thư mục sau khi kiểm tra xong
    void TearDown() override {
        // Đảm bảo set lại instance thành nullptr để tránh core dump
        if (ControllerManager::getInstance() != nullptr) {
            MockControllerManager::SetMockInstance(nullptr);  // Reset instance nếu không null
        }
    }
};

TEST_F(MetadataControllerTest, HandleInputVideo_Test) {
    Exception_Handler = mockExceptionHandler_InputAlphaStr;
    std::string filePath = "./resources/test/【HD】SING女團-寄明月MV(舞蹈版) [Official MV Dance Ver.]官方完整版MV.mp4";
    std::string ID = "1000";
    std::shared_ptr<MediaFile> mediafile = std::make_shared<MediaFile>();;
    
    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(2);
    EXPECT_CALL(*mockMediaFileLibrary, getMediaFileByID(::testing::_))
        .WillRepeatedly(::testing::Return(mediafile));
    EXPECT_CALL(*mockMetadataView, showFileMetadata(::testing::_)).Times(2);
    EXPECT_CALL(*mockMetadataView, menuEditVideo()).Times(2);
    EXPECT_CALL(*mockMetadataView, getVideoOption())
        .WillOnce(::testing::Return(VideoEdit::VIDEO_TITLE))
        .WillOnce(::testing::Return(VideoEdit::VIDEO_TITLE))
        .WillOnce(::testing::Return(VideoEdit::VIDEO_BACK))
        .WillOnce(::testing::Return(VideoEdit::VIDEO_BACK));
    
    metadataController->handleEditVideo(filePath,ID);
}

TEST_F(MetadataControllerTest, HandleInputVideo_Test_Error) {
    Exception_Handler = mockExceptionHandler_InputAlphaStr;
    std::string filePath = "./resources/test/long.mp4";
    std::string ID = "1000";
    std::shared_ptr<MediaFile> mediafile = std::make_shared<MediaFile>();;
    
    metadataController->handleEditVideo(filePath,ID);
}

TEST_F(MetadataControllerTest, HandleInputAudio_Test) {
    Exception_Handler = mockExceptionHandler_InputAlphaStr;
    std::string filePath = "./resources/test/AAA-WEANThisisVietnameseartistQueenB-35024694.mp3";
    std::string ID = "1000";
    std::shared_ptr<MediaFile> mediafile = std::make_shared<MediaFile>();;
    
    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(7);
    EXPECT_CALL(*mockMediaFileLibrary, getMediaFileByID(::testing::_))
        .WillRepeatedly(::testing::Return(mediafile));
    EXPECT_CALL(*mockMetadataView, showFileMetadata(::testing::_)).Times(7);
    EXPECT_CALL(*mockMetadataView, menuEditAudio()).Times(7);
    EXPECT_CALL(*mockMetadataView, getAuditoOption())
        .WillOnce(::testing::Return(AudioEdit::AUDIO_TITLE))
        .WillOnce(::testing::Return(AudioEdit::AUDIO_TITLE))
        .WillOnce(::testing::Return(AudioEdit::AUDIO_ALBUM))
        .WillOnce(::testing::Return(AudioEdit::AUDIO_ALBUM))
        .WillOnce(::testing::Return(AudioEdit::AUDIO_ARTIST))
        .WillOnce(::testing::Return(AudioEdit::AUDIO_ARTIST))
        .WillOnce(::testing::Return(AudioEdit::AUDIO_GENRE))
        .WillOnce(::testing::Return(AudioEdit::AUDIO_GENRE))
        .WillOnce(::testing::Return(AudioEdit::AUDIO_TRACK))
        .WillOnce(::testing::Return(AudioEdit::AUDIO_TRACK))
        .WillOnce(::testing::Return(AudioEdit::AUDIO_YEAR))
        .WillOnce(::testing::Return(AudioEdit::AUDIO_YEAR))
        .WillOnce(::testing::Return(AudioEdit::AUDIO_BACK))
        .WillOnce(::testing::Return(AudioEdit::AUDIO_BACK));
    
    metadataController->handleEditAudio(filePath,ID);
}

TEST_F(MetadataControllerTest, HandleInputAudio_Test_Error) {
    Exception_Handler = mockExceptionHandler_InputAlphaStr;
    std::string filePath = "./resources/test/long.mp3";
    std::string ID = "1000";
    std::shared_ptr<MediaFile> mediafile = std::make_shared<MediaFile>();;
    
    metadataController->handleEditAudio(filePath,ID);
}

TEST_F(MetadataControllerTest, HandleInput_Test1) {
    Exception_Handler = mockExceptionHandler_InputAlphaStr;
    std::string filePath = "./resources/test/【HD】SING女團-寄明月MV(舞蹈版) [Official MV Dance Ver.]官方完整版MV.mp4";
    std::string ID = "1000";
    std::shared_ptr<MediaFile> mediafile = std::make_shared<MediaFile>();;
    mediafile->setType(VIDEO);
    mediafile->setPath(filePath);

    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(3);
    EXPECT_CALL(*mockMediaFileLibrary, getMediaFileByID(::testing::_))
        .WillRepeatedly(::testing::Return(mediafile));
    EXPECT_CALL(*mockMetadataView, showFileMetadata(::testing::_)).Times(3);
    EXPECT_CALL(*mockViewManager, switchView(::testing::_)).Times(2);
    EXPECT_CALL(*mockMetadataView, menuEditVideo()).Times(1);
    EXPECT_CALL(*mockMetadataView, getSelectedOption())
        .WillOnce(::testing::Return(MetadataMenu::EDIT_METADATA))
        .WillOnce(::testing::Return(MetadataMenu::EDIT_METADATA))
        .WillOnce(::testing::Return(MetadataMenu::BACK_FROM_METADATA))
        .WillOnce(::testing::Return(MetadataMenu::BACK_FROM_METADATA));

    EXPECT_CALL(*mockMetadataView, getVideoOption())
        .WillOnce(::testing::Return(VideoEdit::VIDEO_BACK))
        .WillOnce(::testing::Return(VideoEdit::VIDEO_BACK));
    metadataController->handleInput(ID);
}

TEST_F(MetadataControllerTest, HandleInput_Test2) {
    Exception_Handler = mockExceptionHandler_InputAlphaStr;
    std::string filePath = "./resources/test/AAA-WEANThisisVietnameseartistQueenB-35024694.mp3";
    std::string ID = "1000";
    std::shared_ptr<MediaFile> mediafile = std::make_shared<MediaFile>();;
    mediafile->setType(AUDIO);
    mediafile->setPath(filePath);

    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(3);
    EXPECT_CALL(*mockMediaFileLibrary, getMediaFileByID(::testing::_))
        .WillRepeatedly(::testing::Return(mediafile));
    EXPECT_CALL(*mockMetadataView, showFileMetadata(::testing::_)).Times(3);
    EXPECT_CALL(*mockViewManager, switchView(::testing::_)).Times(2);
    EXPECT_CALL(*mockMetadataView, menuEditAudio()).Times(1);
    EXPECT_CALL(*mockMetadataView, getSelectedOption())
        .WillOnce(::testing::Return(MetadataMenu::EDIT_METADATA))
        .WillOnce(::testing::Return(MetadataMenu::EDIT_METADATA))
        .WillOnce(::testing::Return(MetadataMenu::BACK_FROM_METADATA))
        .WillOnce(::testing::Return(MetadataMenu::BACK_FROM_METADATA));

    EXPECT_CALL(*mockMetadataView, getAuditoOption())
        .WillOnce(::testing::Return(AudioEdit::AUDIO_BACK))
        .WillOnce(::testing::Return(AudioEdit::AUDIO_BACK));
    metadataController->handleInput(ID);
}

