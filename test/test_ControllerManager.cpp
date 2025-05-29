#include <gtest/gtest.h>
#include "ControllerManager.hpp"
#include "ViewManager.hpp"
#include "ModelManager.hpp"

// 🔹 Test: `getInstance` khởi tạo đúng
TEST(ControllerManagerTest, GetInstance_ReturnsSameInstance) {
    ViewManager viewManager;
    ModelManager modelManager;

    ControllerManager* instance1 = ControllerManager::getInstance(&viewManager, &modelManager);
    ControllerManager* instance2 = ControllerManager::getInstance(&viewManager, &modelManager);

    EXPECT_EQ(instance1, instance2);  // Cả hai phải trỏ đến cùng một instance (singleton)
}

// 🔹 Test: `getInstance` ném lỗi nếu truyền `nullptr`
TEST(ControllerManagerTest, GetInstance_ThrowsExceptionOnNull) {
    ControllerManager::resetInstance();
    EXPECT_THROW(ControllerManager::getInstance(nullptr, nullptr), std::runtime_error);
}

// 🔹 Test: `getViewManager` trả về đúng ViewManager đã truyền vào
TEST(ControllerManagerTest, GetViewManager_ReturnsCorrectPointer) {
    ViewManager viewManager;
    ModelManager modelManager;
    ControllerManager* instance = ControllerManager::getInstance(&viewManager, &modelManager);

    EXPECT_EQ(instance->getViewManager(), &viewManager);
}

// 🔹 Test: `getModelManager` trả về đúng ModelManager đã truyền vào
TEST(ControllerManagerTest, GetModelManager_ReturnsCorrectPointer) {
    ViewManager viewManager;
    ModelManager modelManager;
    ControllerManager* instance = ControllerManager::getInstance(&viewManager, &modelManager);

    EXPECT_EQ(instance->getModelManager(), &modelManager);
}

// 🔹 Test: Đảm bảo tất cả controllers được tạo và không null
TEST(ControllerManagerTest, GetControllers_ReturnValidPointers) {
    ViewManager viewManager;
    ModelManager modelManager;
    ControllerManager* instance = ControllerManager::getInstance(&viewManager, &modelManager);

    EXPECT_NE(instance->getMainMenuController(), nullptr);
    EXPECT_NE(instance->getScanfOptionController(), nullptr);
    EXPECT_NE(instance->getPlaylistController(), nullptr);
    EXPECT_NE(instance->getMediaFileController(), nullptr);
    EXPECT_NE(instance->getPlayingMediaController(), nullptr);
    EXPECT_NE(instance->getDetailedPlaylistController(), nullptr);
    EXPECT_NE(instance->getMetadataController(), nullptr);
}

// 🔹 Test: `getHardwareController` không gây lỗi dù không có cổng serial
TEST(ControllerManagerTest, GetHardwareController_DoesNotThrowError) {
    ViewManager viewManager;
    ModelManager modelManager;
    ControllerManager* instance = ControllerManager::getInstance(&viewManager, &modelManager);

    EXPECT_NO_THROW(instance->getHardwareController());
}

// 🔹 Test: Đảm bảo tất cả controllers trả về đúng kiểu dữ liệu
TEST(ControllerManagerTest, GetControllers_ReturnCorrectTypes) {
    ViewManager viewManager;
    ModelManager modelManager;
    ControllerManager* instance = ControllerManager::getInstance(&viewManager, &modelManager);

    EXPECT_TRUE(dynamic_cast<MainMenuController*>(instance->getMainMenuController()) != nullptr);
    EXPECT_TRUE(dynamic_cast<ScanfOptionController*>(instance->getScanfOptionController()) != nullptr);
    EXPECT_TRUE(dynamic_cast<PlaylistController*>(instance->getPlaylistController()) != nullptr);
    EXPECT_TRUE(dynamic_cast<MediaFileController*>(instance->getMediaFileController()) != nullptr);
    EXPECT_TRUE(dynamic_cast<PlayingMediaController*>(instance->getPlayingMediaController()) != nullptr);
    EXPECT_TRUE(dynamic_cast<DetailedPlaylistController*>(instance->getDetailedPlaylistController()) != nullptr);
    EXPECT_TRUE(dynamic_cast<MetadataController*>(instance->getMetadataController()) != nullptr);
}