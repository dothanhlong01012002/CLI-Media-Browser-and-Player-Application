#include <filesystem>
#include "ScanfOptionController.hpp"
#include <taglib/fileref.h>
#include "Mock.hpp"

namespace fs = std::filesystem;

void mockExceptionHandler_InputPath(const std::string& message, std::string& input, void(*validator)(const std::string&)) {
    input = "./resources/test";
}

void mockExceptionHandler_InputUSB(const std::string& message, size_t& input, void(*func)(const size_t&)) {
    input = 1;
}

class ScanfOptionControllerTest : public ::testing::Test {
protected:
    std::unique_ptr<MockControllerManager> mockControllerManager;
    std::unique_ptr<MockModelManager> mockModelManager;
    std::unique_ptr<MockViewManager> mockViewManager;
    std::unique_ptr<MockMediaFileLibrary> mockMediaFileLibrary;
    std::shared_ptr<MockMediaFile> mockMediaFile;
    std::unique_ptr<MockScanfOptionView> mockScanfOptionView;
    std::unique_ptr<MockPlaylistLibrary> mockPlaylistLibrary;
    std::unique_ptr<ScanfOptionController> controller;
    std::string username = std::getenv("USER");  // Lấy user hiện tại
    std::string mediaPath = "/media/" + username;

    // Tạo thư mục giả lập trong thư mục /media/<current_user>/
    void SetUp() override {
        mockControllerManager = std::make_unique<MockControllerManager>();
        mockModelManager = std::make_unique<MockModelManager>();
        mockViewManager = std::make_unique<MockViewManager>();
        mockScanfOptionView = std::make_unique<MockScanfOptionView>();
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
        // Khởi tạo controller
        controller = std::make_unique<ScanfOptionController>();
        if (fs::exists(mediaPath)) {
            fs::remove_all(mediaPath);
        }

        // Tạo thư mục test cho user hiện tại
        fs::create_directory(mediaPath);

        // Tạo các thư mục giả lập đại diện cho USB
        fs::create_directory(mediaPath + "/usb1");
        fs::create_directory(mediaPath + "/usb2");
    }

    // Xóa các thư mục sau khi kiểm tra xong
    void TearDown() override {
        if (fs::exists(mediaPath)) {
            fs::remove_all(mediaPath);
        }
        // Đảm bảo set lại instance thành nullptr để tránh core dump
        if (ControllerManager::getInstance() != nullptr) {
            MockControllerManager::SetMockInstance(nullptr);  // Reset instance nếu không null
        }
    }
};

// Kiểm tra trường hợp thư mục không tồn tại
TEST_F(ScanfOptionControllerTest, ScanUSB_NoMediaDirectory) {
    // Xóa thư mục /media/<current_user> nếu có
    fs::remove_all(mediaPath);

    // Kiểm tra scanUSB() khi không có thư mục người dùng
    std::vector<std::string> result = controller->scanUSB();

    EXPECT_TRUE(result.empty());  // Hàm trả về một vector rỗng
}

// Kiểm tra trường hợp thư mục không có USB
TEST_F(ScanfOptionControllerTest, ScanUSB_NoDirectory) {
    // Kiểm tra khi thư mục /media/<current_user> không có thư mục con
    fs::remove_all(mediaPath);
    fs::create_directory(mediaPath);

    // Kiểm tra scanUSB() khi thư mục không phải là thư mục USB
    std::vector<std::string> result = controller->scanUSB();

    EXPECT_TRUE(result.empty());  // Không có thư mục USB nào
}

// Kiểm tra khi có thư mục USB
TEST_F(ScanfOptionControllerTest, ScanUSB_WithMountPoints) {
    // Thư mục giả lập các thiết bị USB được mount
    std::vector<std::string> expected = {
        mediaPath + "/usb1",
        mediaPath + "/usb2"
    };

    // Kiểm tra scanUSB() với thư mục USB
    std::vector<std::string> result = controller->scanUSB();
    std::sort(result.begin(), result.end());  // Sắp xếp danh sách kết quả
    std::sort(expected.begin(), expected.end());  // Sắp xếp danh sách mong đợi

    EXPECT_EQ(result, expected);  // So sánh lại sau khi đã sắp xếp
}

TEST_F(ScanfOptionControllerTest, ValidMp3File) {
    std::string filePath = "./resources/test/AAA-WEANThisisVietnameseartistQueenB-35024694.mp3";  // Tạo tệp giả để test

    std::shared_ptr<MediaFile> result = controller->scanfFilePath(filePath);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->getType(), AUDIO);
    EXPECT_EQ(result->getName(), "AAA-WEANThisisVietnameseartistQueenB-35024694.mp3");
}

TEST_F(ScanfOptionControllerTest, ValidMp4File) {
    std::string filePath = "./resources/test/【HD】SING女團-寄明月MV(舞蹈版) [Official MV Dance Ver.]官方完整版MV.mp4";  // Tạo tệp giả để test

    std::shared_ptr<MediaFile> result = controller->scanfFilePath(filePath);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->getType(), VIDEO);
    EXPECT_EQ(result->getName(), "【HD】SING女團-寄明月MV(舞蹈版) [Official MV Dance Ver.]官方完整版MV.mp4");
}

TEST_F(ScanfOptionControllerTest, InvalidFile) {
    std::string filePath = "./resources/test/text.txt";

    std::shared_ptr<MediaFile> result = controller->scanfFilePath(filePath);

    ASSERT_EQ(result, nullptr);
}

TEST_F(ScanfOptionControllerTest, NotExistsFile) {
    std::string filePath = "text.txt";

    std::shared_ptr<MediaFile> result = controller->scanfFilePath(filePath);

    ASSERT_EQ(result, nullptr);
}

TEST_F(ScanfOptionControllerTest, Back_Option) {
    controller->back();
}

TEST_F(ScanfOptionControllerTest, ScanDirectory) {
    std::string folderPath = "./resources/test";
    EXPECT_CALL(*mockMediaFileLibrary, isValidMediaFileNameInLibrary(::testing::_))
        .WillRepeatedly(::testing::Return(false));
    EXPECT_CALL(*mockMediaFileLibrary, addMediaFile(::testing::_)).Times(2);
    controller->scanDirectory(folderPath);
}

TEST_F(ScanfOptionControllerTest, ScanUSB) {
    std::string folderPath = "./resources/test";
    EXPECT_CALL(*mockMediaFileLibrary, isValidMediaFileNameInLibrary(::testing::_))
        .WillRepeatedly(::testing::Return(false));
    EXPECT_CALL(*mockMediaFileLibrary, addMediaFile(::testing::_)).Times(2);
    controller->scanUSBDevice(folderPath);
}

TEST_F(ScanfOptionControllerTest, HandleInput_Case1) {
    EXPECT_CALL(*mockScanfOptionView, getSelectedOption())
        .WillOnce(::testing::Return(ScanfMenu::BACK_FROM_SCAN))
        .WillOnce(::testing::Return(ScanfMenu::BACK_FROM_SCAN));
    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(1);
    EXPECT_CALL(*mockViewManager, switchView(::testing::_)).Times(1);
    EXPECT_CALL(*mockMediaFileLibrary, isValidMediaFileNameInLibrary(::testing::_))
        .WillRepeatedly(::testing::Return(false));
    EXPECT_CALL(*mockScanfOptionView, displayStatusMessage(::testing::_)).Times(1);
    controller->handleInput();
}

TEST_F(ScanfOptionControllerTest, HandleInput_Case2) {
    Exception_Handler = mockExceptionHandler_InputPath;

    EXPECT_CALL(*mockScanfOptionView, getSelectedOption())
        .WillOnce(::testing::Return(ScanfMenu::SCANF_DIRECTORY))
        .WillOnce(::testing::Return(ScanfMenu::BACK_FROM_SCAN));
    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(1);
    EXPECT_CALL(*mockViewManager, switchView(::testing::_)).Times(1);
    EXPECT_CALL(*mockMediaFileLibrary, isValidMediaFileNameInLibrary(::testing::_))
        .WillRepeatedly(::testing::Return(false));
    EXPECT_CALL(*mockScanfOptionView, displayStatusMessage(::testing::_)).Times(1);
    EXPECT_CALL(*mockMediaFileLibrary, addMediaFile(::testing::_)).Times(2);
    controller->handleInput();
}

TEST_F(ScanfOptionControllerTest, HandleInput_Case3) {
    Exception_Handler_Int = mockExceptionHandler_InputUSB;

    EXPECT_CALL(*mockScanfOptionView, getSelectedOption())
        .WillOnce(::testing::Return(ScanfMenu::SCANF_USB))
        .WillOnce(::testing::Return(ScanfMenu::BACK_FROM_SCAN));
    EXPECT_CALL(*mockViewManager, hideCurrentView()).Times(1);
    EXPECT_CALL(*mockViewManager, switchView(::testing::_)).Times(1);
    EXPECT_CALL(*mockScanfOptionView, displayStatusMessage(::testing::_)).Times(1);
    controller->handleInput();
}

TEST_F(ScanfOptionControllerTest, scanPlaylistsFromTxt_Test) {
    std::string filePath = "./resources/playlists.txt";
    std::vector<std::shared_ptr<Playlist>> playlists;

    EXPECT_CALL(*mockPlaylistLibrary, getAllPlaylists())
        .WillOnce(::testing::ReturnRef(playlists));

    EXPECT_CALL(*mockMediaFileLibrary, isValidMediaFileNameInLibrary(::testing::_))
        .WillOnce(::testing::Return(false))  // Lần 1
        .WillOnce(::testing::Return(false))  // Lần 2
        .WillOnce(::testing::Return(false))  // Lần 3
        .WillOnce(::testing::Return(false))  // Lần 4
        .WillOnce(::testing::Return(false))  // Lần 5
        .WillOnce(::testing::Return(false))  // Lần 6
        .WillOnce(::testing::Return(false))  // Lần 7
        .WillOnce(::testing::Return(true));  // Lần 8

    EXPECT_CALL(*mockMediaFileLibrary, addMediaFile(::testing::_)).Times(7);
    controller->scanPlaylistsFromTxt(filePath);
}

TEST_F(ScanfOptionControllerTest, scanPlaylistsFromTxt_Test2) {
    std::string filePath = "./resources/playlists.txt";
    std::vector<std::shared_ptr<Playlist>> playlists;
    playlists.push_back(std::make_shared<Playlist>("abc"));

    EXPECT_CALL(*mockPlaylistLibrary, getAllPlaylists())
        .WillOnce(::testing::ReturnRef(playlists));

    controller->scanPlaylistsFromTxt(filePath);
}