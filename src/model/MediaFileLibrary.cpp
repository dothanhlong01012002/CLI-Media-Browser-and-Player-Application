#include "MediaFileLibrary.hpp"
#include <algorithm>

// Get all media files stored in the library
const std::vector<std::shared_ptr<MediaFile>>& MediaFileLibrary::getAllMediaFiles() const {
    return mediaFiles;
}

// Add a media file to the library if its ID is not already present
void MediaFileLibrary::addMediaFile(const std::shared_ptr<MediaFile>& file) {
    if (!isValidMediaFileIDInLibrary(file->getID())) {
        mediaFiles.push_back(file);
    }
}

// Remove a media file from the library by its ID
void MediaFileLibrary::removeMediaFile(const std::string& ID) {
    for (auto it = mediaFiles.begin(); it != mediaFiles.end(); ++it) {
        if (it->get()->getID() == ID) {
            mediaFiles.erase(it); 
            return;
        }
    }
}

// Check if a media file ID exists in the library
bool MediaFileLibrary::isValidMediaFileIDInLibrary(const std::string& ID) {
    for (const auto& mediaFile : mediaFiles) {
        if (mediaFile->getID() == ID) {
            return true;
        }
    }
    return false;
}

// Check if a media file name exists in the library
bool MediaFileLibrary::isValidMediaFileNameInLibrary(const std::string& fileName) {
    for (const auto& mediaFile : mediaFiles) {
        if (mediaFile->getName() == fileName) {
            return true;
        }
    }
    return false;
}

// Set all media files in the library
void MediaFileLibrary::setAllMediaFiles(const std::vector<std::shared_ptr<MediaFile>>& files) {
    mediaFiles = files;
}

// Retrieve a media file by its ID
std::shared_ptr<MediaFile> MediaFileLibrary::getMediaFileByID(const std::string& ID) const {
    for (const auto& mediaFile : mediaFiles) {
        if (mediaFile->getID() == ID) {
            return mediaFile;
        }
    }
    return nullptr; 
}

// Retrieve a media file by its name
std::shared_ptr<MediaFile> MediaFileLibrary::getMediaFileByName(const std::string& fileName) const {
    for (const auto& mediaFile : mediaFiles) {
        if (mediaFile->getName() == fileName) {
            return mediaFile;
        }
    }
    return nullptr; 
}
