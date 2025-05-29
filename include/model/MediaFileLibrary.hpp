#ifndef MEDIA_FILE_LIBRARY_H
#define MEDIA_FILE_LIBRARY_H

#include <vector>
#include <memory>
#include <string>
#include "MediaFile.hpp"

class MediaFileLibrary {
private:
    std::vector<std::shared_ptr<MediaFile>> mediaFiles; 

public:
    virtual const std::vector<std::shared_ptr<MediaFile>>& getAllMediaFiles() const;

    virtual void addMediaFile(const std::shared_ptr<MediaFile>& files);

    void removeMediaFile(const std::string& ID);

    void setAllMediaFiles(const std::vector<std::shared_ptr<MediaFile>>& files);

    virtual std::shared_ptr<MediaFile> getMediaFileByID(const std::string& ID) const;

    std::shared_ptr<MediaFile> getMediaFileByName(const std::string& fileName) const;

    virtual bool isValidMediaFileIDInLibrary(const std::string& ID);

    virtual bool isValidMediaFileNameInLibrary(const std::string& fileName);
    virtual ~MediaFileLibrary() = default;
};

#endif // MEDIA_FILE_LIBRARY_H
