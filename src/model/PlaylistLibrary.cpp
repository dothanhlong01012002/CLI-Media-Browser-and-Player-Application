#include "PlaylistLibrary.hpp"
#include <algorithm>
#include <filesystem>

// Returns a reference to the list of all playlists.
const std::vector<std::shared_ptr<Playlist>>& PlaylistLibrary::getAllPlaylists() const {
    return playlists; 
}

// Adds a new playlist to the library.
void PlaylistLibrary::addPlaylist(const std::shared_ptr<Playlist>& playlist) {
    playlists.push_back(playlist); 
}

// Removes a playlist from the library by its ID.
void PlaylistLibrary::removePlaylist(const std::string& ID) {
    auto it = std::remove_if(playlists.begin(), playlists.end(),
        [&ID](const std::shared_ptr<Playlist>& playlist) {
            return playlist->getID() == ID; 
        });
    if (it != playlists.end()) {
        playlists.erase(it, playlists.end());
    }
    for (size_t i = std::stoi(ID) - 1; i < playlists.size(); i++) {
        playlists[i]->setID(std::to_string(i + 1));
    }
}

// Retrieves a playlist by its ID.
std::shared_ptr<Playlist> PlaylistLibrary::getPlaylistByID(const std::string& ID) const {
    for (const auto& playlist : playlists) {
        if (playlist->getID() == ID) {
            return playlist; 
        }
    }
    return nullptr; 
}

// Checks if a playlist with the given ID exists in the library.
bool PlaylistLibrary::isValidPlaylistIDInLibrary(const std::string& ID) {
    for (const auto& playlist : playlists) {
        if (playlist->getID() == ID) {
            return true; 
        }
    }
    return false; 
}

// Saves all playlists to a file.
void PlaylistLibrary::saveToFile(const std::string& filePath) const {
    std::ofstream outFile(filePath, std::ios::out); 
    if (!outFile) {
        std::cerr << "Failed to open file for writing: " << filePath << "\n"; 
        return;
    }
    for (const auto& playlist : playlists) {
        outFile << playlist->getName() << "\n";
        for (const auto& song : playlist->getSongs()) {
            outFile << song->getPath() << "\n";
        }
        outFile << "\n";
    }
    outFile.close(); 
    std::cout << "Playlists saved to " << filePath << "\n"; 
}
