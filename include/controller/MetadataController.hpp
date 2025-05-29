#ifndef METADATA_CONTROLLER_H
#define METADATA_CONTROLLER_H

#include <map>
#include "MediaFile.hpp"
#include "BaseController.hpp"
#include <memory>

class MetadataController : public BaseController {
public:
    virtual void handleInput(const std::string& ID);

    void getMediaFileMetadata(const std::shared_ptr<MediaFile>& file) const;

    void updateMediaFileMetadata(const std::string& key, const std::string& value, const std::string& ID);

    void back();

    void handleEditAudio(const std::string& filepath, const std::string& ID);

    void handleEditVideo(const std::string& filepath, const std::string& ID);
};

#endif // METADATA_CONTROLLER_H
