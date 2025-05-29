#ifndef SCANF_OPTION_CONTROLLER_H
#define SCANF_OPTION_CONTROLLER_H

#include <vector>
#include "MediaFile.hpp"
#include "BaseController.hpp"
#include <memory>
#include "Playlist.hpp"

class ScanfOptionController : public BaseController {
public:
    virtual void handleInput();

    void scanDirectory(const std::string& folderPath);

    void scanUSBDevice(const std::string& device);

    void back();

    std::shared_ptr<MediaFile> scanfFilePath(const std::string& filePath);

    virtual void scanPlaylistsFromTxt(const std::string& filePath);
    
    std::vector<std::string> scanUSB();
};

#endif // SCANF_OPTION_CONTROLLER_H
