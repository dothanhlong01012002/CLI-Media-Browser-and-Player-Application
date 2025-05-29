#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "PlayingMediaController.hpp"
#include "ControllerManager.hpp"
#include "ViewManager.hpp"
#include "ModelManager.hpp"
#include "HardwareController.hpp"
#include "MediaFile.hpp"
#include "PlayingMedia.hpp"
#include "DetailedPlaylistController.hpp"

// Mock classes
class MockPlayingMedia : public PlayingMedia {
public:
    MOCK_METHOD(void, setPlaylist, (const std::vector<std::shared_ptr<MediaFile>>&), (override));
    MOCK_METHOD(void, setCurrentMediaFile, (const std::shared_ptr<MediaFile>& mediaFile), (override));
    MOCK_METHOD(void, adjustVolume, (size_t), (override));
    MOCK_METHOD(size_t, getTotalTime, (), (const, override));
    MOCK_METHOD(std::string, getDurationStringType, (), (const, override));
    MOCK_METHOD(void, setCurrentTime, (size_t), (override));
    MOCK_METHOD(int&, getVolume, (), (override));
    MOCK_METHOD(size_t&, getCurrentTime, (), (override));
    MOCK_METHOD(void, resumeMusic, (), (override));
    MOCK_METHOD(void, pauseMusic, (), (override));
    MOCK_METHOD(void, nextTrack, (), (override));
    MOCK_METHOD(void, previousTrack, (), (override));
    MOCK_METHOD(void, setVolume, (const int &value), (override));
    MOCK_METHOD(std::shared_ptr<MediaFile>, getCurrentMediaFile, (), (const, override));
};

class MockMediaFile : public MediaFile {
public:
    MockMediaFile(const std::string& name, const std::string& id) : MediaFile(name, id) {}
    MOCK_METHOD(void, setName, (const std::string&), (override));
    MOCK_METHOD(void, setPath, (const std::string&), (override));
    MOCK_METHOD(void, setType, (MediaType), (override));
    MOCK_METHOD(void, setDuration, (const size_t& ), (override));
    MOCK_METHOD(void, setMetadata, (const Metadata&), (override));
    MOCK_METHOD(Metadata&, getMetadata, (), (override));
};

class MockMetadata : public Metadata {
public:
    MOCK_METHOD(void, setValue, (const std::string&, const std::string&), (override));
};

class MockMediaFileLibrary : public MediaFileLibrary {
public:
    MOCK_METHOD(const std::vector<std::shared_ptr<MediaFile>>&, getAllMediaFiles, (), (const, override));
    MOCK_METHOD(void, addMediaFile, (const std::shared_ptr<MediaFile>&), (override));
    MOCK_METHOD(std::shared_ptr<MediaFile>, getMediaFileByID, (const std::string&), (const, override));
    MOCK_METHOD(bool, isValidMediaFileNameInLibrary, (const std::string&), (override));
    MOCK_METHOD(bool, isValidMediaFileIDInLibrary, (const std::string&), (override));
};

class MockPlaylistLibrary : public PlaylistLibrary {
public:
    MOCK_METHOD(bool, isValidPlaylistIDInLibrary, (const std::string& ID), ( override));
    MOCK_METHOD(std::shared_ptr<Playlist>, getPlaylistByID, (const std::string&), (const, override));
    MOCK_METHOD(const std::vector<std::shared_ptr<Playlist>>&, getAllPlaylists, (), (const, override));
    MOCK_METHOD(void ,addPlaylist,(const std::shared_ptr<Playlist>& playlist),(override)  );
    MOCK_METHOD(void ,removePlaylist,(const std::string& ID), (override));
    MOCK_METHOD(void ,saveToFile,(const std::string& filePath), (const, override));
};

class MockPlayingMediaView : public PlayingMediaView {
public:
    MOCK_METHOD(void, showPlayingMedia, (PlayingMedia* player, size_t& currentTime, size_t totalTime, int& volume), (override));
    MOCK_METHOD(int, getSelectedOption, (), (const, override));
};

class MockMediaFileView : public MediaFileView {
public:
    MOCK_METHOD(void, showMediaFilesPage, 
        (const std::vector<std::shared_ptr<MediaFile>>&, size_t, size_t, size_t), 
        (override));
    MOCK_METHOD(void, displayStatusMessage, (MediaFileStatus&), (override));
    MOCK_METHOD(int, getSelectedOption, (), (const, override));
};

class MockScanfOptionView : public ScanfOptionView {
public:
    MOCK_METHOD(void, displayStatusMessage, (ScanStatus&), (override));
    MOCK_METHOD(int, getSelectedOption, (), (const, override));
};

class MockMetadataView : public MetadataView {
public:
    MOCK_METHOD(int, getSelectedOption, (), (const, override));  // Mock getSelectedOption
    MOCK_METHOD(int, getAuditoOption, (), (const, override));  // Mock getAudioOption
    MOCK_METHOD(int, getVideoOption, (), (const, override));  // Mock getVideoOption
    MOCK_METHOD(void, showFileMetadata, (const std::shared_ptr<MediaFile>&), (override));  // Mock showFileMetadata
    MOCK_METHOD(void, menuEditAudio, (), (override));  // Mock menuEditAudio
    MOCK_METHOD(void, menuEditVideo, (), (override));  // Mock menuEditVideo
};

class MockMainMenuView : public MainMenuView {
public:
    MOCK_METHOD(int, getSelectedOption, (), (const, override)); 
};

class MockModelManager : public ModelManager {
public:
    MOCK_METHOD(MockPlayingMedia*, getPlayingMedia, (), (const, override));
    MOCK_METHOD(MockMediaFileLibrary*, getMediaLibrary, (), (const, override));
    MOCK_METHOD(MockPlaylistLibrary*, getPlaylistLibrary, (), (const, override));
};

class MockDetailedPlaylistView : public DetailedPlaylistView {
public:
    MOCK_METHOD(void, displayStatusMessage, (DetailedPlaylistStatus&), (override));
    MOCK_METHOD(int, getSelectedOption, (), (const, override));
    MOCK_METHOD(void, showListOfSongs, (const std::shared_ptr<Playlist>&), (override));
    MOCK_METHOD(void, showPlaylistDetails, (const std::shared_ptr<Playlist>& playlist), (override));
};

class MockPlaylistView : public PlaylistView {
public:
    MOCK_METHOD(void, showMenu, (), (override));
    MOCK_METHOD(int, getSelectedOption, (), (const, override));
    MOCK_METHOD(void, hideMenu, (), (override));
    MOCK_METHOD(void, showPlaylistList, (const std::vector<std::shared_ptr<Playlist>>& playlists), (override));
    MOCK_METHOD(void, displayStatusMessage, (PlaylistStatus& status), (override));
};

class MockViewManager : public ViewManager {
public:
    MOCK_METHOD(void, hideCurrentView, (), (override));
    MOCK_METHOD(void, showCurrentView, (), (override));
    MOCK_METHOD(void, switchView, (SwitchView), (override));
    MOCK_METHOD(MockPlayingMediaView*, getPlayingMediaView, (), (const, override));
    MOCK_METHOD(MockScanfOptionView*, getScanfOptionView, (), (const, override));
    MOCK_METHOD(MockDetailedPlaylistView*, getDetailedPlaylistView, (), (const, override)); // Check this line
    MOCK_METHOD(MockPlaylistView*, getPlaylistView, (),(const, override));
    MOCK_METHOD(MockMediaFileView*, getMediaFileView, (),(const, override));
    MOCK_METHOD(MockMetadataView*, getMetadataView, (),(const, override));
    MOCK_METHOD(MockMainMenuView*, getMainMenuView, (),(const, override));
};

class MockPlayingMediaController : public PlayingMediaController {
public:
    MOCK_METHOD(void, handleInput, (const std::string&), (override));
    MOCK_METHOD(void, startUpdateThread, (), (override));
    MOCK_METHOD(void, stopUpdateThread, (), (override));
    MOCK_METHOD(void, play, (), (override));
    MOCK_METHOD(void, pause, (), (override));
    MOCK_METHOD(void, skipToNext, (), (override));
    MOCK_METHOD(void, skipToPrevious, (), (override));
};

class MockScanfOptionController : public ScanfOptionController {
public:
    MOCK_METHOD(void, handleInput, (), (override));
    MOCK_METHOD(void, scanPlaylistsFromTxt, (const std::string&), (override));
};

class MockMediaFileController : public MediaFileController {
public:
    MOCK_METHOD(void, handleInput, (), (override));
};

class MockPlaylistController : public PlaylistController {
public:
    MOCK_METHOD(void, handleInput, (), (override));
};

class MockHardwareController : public HardwareController {
public:
    MockHardwareController(const std::string& device, int baudRate)
        : HardwareController(device, baudRate) {}

    MOCK_METHOD(void, sendPlayCommand, (), (override));
    MOCK_METHOD(void, sendPauseCommand, (), (override));
    MOCK_METHOD(void, sendStopCommand, (), (override));
    MOCK_METHOD(void, sendSignal, (const std::string& signal), (override));
    MOCK_METHOD(void, startReading, (), (override));
    MOCK_METHOD(void, stopReading, (), (override));
};

class MockControllerManager : public ControllerManager {
public:
    MockControllerManager() : ControllerManager(nullptr, nullptr) {}

    MOCK_METHOD(ViewManager*, getViewManager, (), (const, override));
    MOCK_METHOD(ModelManager*, getModelManager, (), (const, override));
    MOCK_METHOD(MainMenuController*, getMainMenuController, (), (const, override));
    MOCK_METHOD(ScanfOptionController*, getScanfOptionController, (), (const, override));
    MOCK_METHOD(PlaylistController*, getPlaylistController, (), (const, override));
    MOCK_METHOD(MediaFileController*, getMediaFileController, (), (const, override));
    MOCK_METHOD(PlayingMediaController*, getPlayingMediaController, (), (const, override));
    MOCK_METHOD(DetailedPlaylistController*, getDetailedPlaylistController, (), (const, override));
    MOCK_METHOD(MetadataController*, getMetadataController, (), (const, override));
    MOCK_METHOD(HardwareController*, getHardwareController, (), (const, override));

    static void SetMockInstance(MockControllerManager* mockInstance) {
        instance = mockInstance;
    }
};


class MockPlaylist : public Playlist {
public:
    explicit MockPlaylist(const std::string& name) : Playlist(name) {}
    MOCK_METHOD((const std::vector<std::shared_ptr<MediaFile>>&), getSongs, (), (const, override));
    MOCK_METHOD(void, addSong, (const std::shared_ptr<MediaFile>&), (override));
    MOCK_METHOD(void, removeSong, (const std::string&), (override));
    MOCK_METHOD(bool, isValidMediaFileIDInPlaylist, (const std::string&), (override));
                    
};

// Mock MetadataController
class MockMetadataController : public MetadataController {
public:
    MOCK_METHOD(void, handleInput, (const std::string&), (override));
};

class MockDetailedPlaylistController : public DetailedPlaylistController {
public:
    MOCK_METHOD(void, handleInput, (std::string listId), (override));
    MOCK_METHOD(void, addSongToPlaylist, (const std::string& PlaylistId, const std::string& songID), (override));
    MOCK_METHOD(void, removeSongFromPlaylist, (const std::string& PlaylistId, const std::string& songID), (override));
    MOCK_METHOD(void, back, (), (override));
    MOCK_METHOD(void, showMediafileInList, (const std::string& listId), (override));
};

