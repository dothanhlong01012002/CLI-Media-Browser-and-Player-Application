#include <iomanip>
#include "DetailedPlaylistView.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>
#include "MediaFile.hpp"
#include "Playlist.hpp"

using namespace ftxui;

void DetailedPlaylistView::showMenu() {
    BaseView::showMenu();
    auto menu = Menu(&menu_entries, &selected_option);

    auto renderer = Renderer(menu, [&] {
        return vbox({
                   text("===== Detailed Playlist Menu ====="),
                   menu->Render(),
                   text("==============================="),
                   text("Use arrow keys or mouse to navigate, press Enter or click to select."),
               }) |
               border;
    });

    auto screen = ScreenInteractive::TerminalOutput();

    auto event_handler = CatchEvent(renderer, [&](Event event) {
        if (event.is_mouse()) {
            if (event.mouse().button == Mouse::Left && menu->OnEvent(event)) {
                screen.ExitLoopClosure()(); 
                return true;
            }          
        }
        if (event == Event::Return) {
            if (menu->OnEvent(event)) {
            screen.ExitLoopClosure()(); 
            return true;
            }
        } 
        if (event == Event::Escape || event == Event::Character('q')) {
            screen.ExitLoopClosure()(); 
            return true;
        }
        return menu->OnEvent(event);

    });

    screen.Loop(event_handler);
}

// Get the selected menu option
int DetailedPlaylistView::getSelectedOption() const {
    return selected_option;
}

// Hide the menu and reset the selected option
void DetailedPlaylistView::hideMenu() {
    BaseView::hideMenu();
    selected_option = 0;
    std::system("clear");
}

// Show detailed information about a playlist
void DetailedPlaylistView::showPlaylistDetails(const std::shared_ptr<Playlist>& playlist) {
    using namespace ftxui;

    std::string name = playlist->getName();
    size_t numberOfSongs = playlist->getSongs().size();

    auto document = vbox({
        text("Playlist Details") | bold | hcenter,          
        separator(),
        hbox({
            text("Name: ") | bold,                             
            text(name) | color(Color::Yellow)
        }),
        hbox({
            text("Number of Songs: ") | bold,                 
            text(std::to_string(numberOfSongs)) | color(Color::Green)
        }),
        separator(),
        text("Press any key to continue...") | dim | hcenter    
    });

    auto screen = Screen::Create(
        Dimension::Full(),        
        Dimension::Fit(document)   
    );
    Render(screen, document);

    std::cout << screen.ToString() << std::endl;

}

// Display a list of songs in the playlist
void DetailedPlaylistView::showListOfSongs(const std::shared_ptr<Playlist>& playlist) {
    using namespace ftxui;

    std::vector<std::shared_ptr<MediaFile>> songs = playlist->getSongs();

    Elements items;
    for (const auto& song : songs) {
        items.push_back(hbox({
            text("[" + song->getID() + "] ") | size(WIDTH, EQUAL, 10),   
            text(song->getName()) | size(WIDTH, GREATER_THAN, 30)      
        }));
    }

    auto header = hbox({
        text("Songs in Playlist: ") | bold,
        text(playlist->getName()) | color(Color::Yellow)
    }) | hcenter;

    auto footer = text("Total Songs: " + std::to_string(songs.size())) | hcenter;

    auto document = vbox({
        header,
        separator(),
        vbox(std::move(items)) | border, 
        separator(),
        footer
    });

    auto screen = Screen::Create(
        Dimension::Full(),         
        Dimension::Fit(document)   
    );
    Render(screen, document);

    std::cout << screen.ToString() << std::endl;

}

// Display appropriate status messages based on the status code
void DetailedPlaylistView::displayStatusMessage(DetailedPlaylistStatus& status) {
    switch (status) {
        case DetailedPlaylistStatus::DETAILED_ADD_STATUS:
            status = DetailedPlaylistStatus::DETAILED_NORMAL;
            std::cout << "Error: Song ID not found in media library or Song ID exists in playlist. Cannot add.\n";
            break;
        case DetailedPlaylistStatus::DETAILED_PLAY_STATUS:
            status = DetailedPlaylistStatus::DETAILED_NORMAL;
            std::cout << "Error: Song ID not found in playlist. Cannot play.\n";
            break;
        case DetailedPlaylistStatus::DETAILED_DELETE_STATUS:
            status = DetailedPlaylistStatus::DETAILED_NORMAL;
            std::cout << "Error: Song ID not found in playlist. Cannot delete.\n";
            break;
        case DetailedPlaylistStatus::DETAILED_SHOW_STATUS:
            status = DetailedPlaylistStatus::DETAILED_NORMAL;
            std::cout << "Error: Song ID not found in playlist. Cannot show.\n";
            break;
        case DetailedPlaylistStatus::DETAILED_NORMAL:
        default:
            break;
    }
}
