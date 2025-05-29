#include "MediaFile.hpp"

// Initialize static members
size_t MediaFile::mediaFileNextID = 1;
std::queue<int> MediaFile::mediaFileFreeIDs;

// Constructor: Create a MediaFile object with specified properties
MediaFile::MediaFile(const std::string& name, const std::string& path, MediaType type, const Metadata& metadata) 
    : name(name), path(path), type(type), metadata(metadata) {
    if (!mediaFileFreeIDs.empty()) {
        ID = std::to_string(mediaFileFreeIDs.front());
        mediaFileFreeIDs.pop();
    } else {
        ID = std::to_string(mediaFileNextID++);
    }
}

// Set the name of the media file
void MediaFile::setName(const std::string& name) {
    this->name = name;
}

// Set the file path of the media file
void MediaFile::setPath(const std::string& path) {
    this->path = path;
}

// Set the duration of the media file (in seconds)
void MediaFile::setDuration(const size_t& duration) {
    this->duration = duration;
}

// Get the name of the media file
std::string MediaFile::getName() const {
    return name;
}

// Get the unique ID of the media file
std::string MediaFile::getID() const {
    return ID;
}

// Get the duration of the media file (in seconds)
size_t MediaFile::getDuration() const {
    return duration;
}

// Get the file path of the media file
std::string MediaFile::getPath() const {
    return path;
}

// Get the type of the media file (e.g., AUDIO, VIDEO)
MediaType MediaFile::getType() const {
    return type;
}

// Get the metadata associated with the media file
Metadata& MediaFile::getMetadata() {
    return metadata;
}

// Set new metadata for the media file
void MediaFile::setMetadata(const Metadata& newMetadata) {
    metadata.setMetadata(newMetadata.getMetadata());
}

// Set the type of the media file (e.g., AUDIO, VIDEO)
void MediaFile::setType(MediaType newType) {
    type = newType;
}

// Destructor: Recycle the ID for future use
MediaFile::~MediaFile() {
    mediaFileFreeIDs.push(std::stoi(ID));
}
