#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

#include "MediaFileLibrary.hpp"
#include "PlaylistLibrary.hpp"
#include "PlayingMedia.hpp"
#include "Playlist.hpp"
#include <memory>

class ModelManager {
private:
    std::unique_ptr<MediaFileLibrary> mediaLibrary;

    std::unique_ptr<PlaylistLibrary> playlistLibrary;

    std::unique_ptr<PlayingMedia> playingMedia;

public:
    ModelManager();

    virtual MediaFileLibrary* getMediaLibrary() const;

    virtual PlaylistLibrary* getPlaylistLibrary() const;

    virtual PlayingMedia* getPlayingMedia() const;

    virtual ~ModelManager() = default;
};

#endif // MODEL_MANAGER_H
