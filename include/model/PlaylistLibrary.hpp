#ifndef PLAYLIST_LIBRARY_H
#define PLAYLIST_LIBRARY_H

#include <vector>
#include "Playlist.hpp"
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

class PlaylistLibrary {
private:
    std::vector<std::shared_ptr<Playlist>> playlists;

public:
    virtual const std::vector<std::shared_ptr<Playlist>>& getAllPlaylists() const;

    virtual void addPlaylist(const std::shared_ptr<Playlist>& playlist);

    virtual void removePlaylist(const std::string& ID);

    virtual std::shared_ptr<Playlist> getPlaylistByID(const std::string& ID) const;

    virtual bool isValidPlaylistIDInLibrary(const std::string& ID);

    virtual void saveToFile(const std::string& filePath) const;

    virtual ~PlaylistLibrary() = default;
};

#endif // PLAYLISTLIBRARY_H
