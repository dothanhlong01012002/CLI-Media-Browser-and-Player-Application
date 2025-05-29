#ifndef PLAYLIST_CONTROLLER_H
#define PLAYLIST_CONTROLLER_H

#include <vector>
#include "Playlist.hpp"
#include "MediaFile.hpp"
#include "BaseController.hpp"
#include "ModelManager.hpp"
#include "ViewManager.hpp"
#include <memory>

class PlaylistController : public BaseController {
public:
    virtual void handleInput();

    void createPlaylist(const std::string& name);

    void back();

    void deletePlaylist(const std::string& Id);

    void showAllPlaylists(const std::vector<std::shared_ptr<Playlist>>& lists);
};

#endif // PLAYLIST_CONTROLLER
