#include "ModelManager.hpp"

// Constructor: Initialize the model components
ModelManager::ModelManager() {
    mediaLibrary = std::make_unique<MediaFileLibrary>();  
    playlistLibrary = std::make_unique<PlaylistLibrary>(); 
    playingMedia = std::make_unique<PlayingMedia>(); 
}

// Retrieve the MediaFileLibrary instance
MediaFileLibrary* ModelManager::getMediaLibrary() const {
    if (mediaLibrary) {
        return mediaLibrary.get(); 
    } else {
        return nullptr; 
    }
}

// Retrieve the PlaylistLibrary instance
PlaylistLibrary* ModelManager::getPlaylistLibrary() const {
    if (playlistLibrary) {
        return playlistLibrary.get();
    } else {
        return nullptr; 
    }
}

// Retrieve the PlayingMedia instance
PlayingMedia* ModelManager::getPlayingMedia() const {
    if (playingMedia) {
        return playingMedia.get(); 
    } else {
        return nullptr; 
    }
}
