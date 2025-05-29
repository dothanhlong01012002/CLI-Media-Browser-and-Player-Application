#include "PlaylistView.hpp"
#include <iostream>
#include <iomanip>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

void PlaylistView::showMenu() {
    BaseView::showMenu(); 

    auto menu = Menu(&menu_entries, &selected_option);

    auto renderer = Renderer(menu, [&] {
        return vbox({
                   text("===== Playlist Menu ====="),
                   menu->Render(), 
                   text("========================="),
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

int PlaylistView::getSelectedOption() const {
    return selected_option;
}

void PlaylistView::hideMenu() {
    BaseView::hideMenu(); 
    selected_option = 0;  
    std::system("clear"); 
}

void PlaylistView::showPlaylistList(const std::vector<std::shared_ptr<Playlist>>& playlists) {
    using namespace ftxui;

    Elements items;
    for (const auto& playlist : playlists) {
        items.push_back(hbox({
            text("[" + playlist->getID() + "] ") | size(WIDTH, EQUAL, 10),
            text(playlist->getName()) | size(WIDTH, GREATER_THAN, 30)     
        }));
    }

    auto header = text("============= Playlist List =============") | bold | hcenter;

    auto footer = text("Total Playlists: " + std::to_string(playlists.size())) | hcenter;

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

void PlaylistView::displayStatusMessage(PlaylistStatus& status) {
    // Display appropriate status messages based on the current status
    switch (status) {
        case PlaylistStatus::PLAYLIST_SELECT_STATUS:
            std::cout << "Error: The playlist ID was not found in the library.\n";
            status = PlaylistStatus::PLAYLIST_NORMAL;  
            break;
        case PlaylistStatus::PLAYLIST_ADD_STATUS:
            std::cout << "Error: Could not add playlist. Please try again.\n";
            status = PlaylistStatus::PLAYLIST_NORMAL;  
            break;
        case PlaylistStatus::PLAYLIST_REMOVE_STATUS:
            std::cout << "Error: The playlist ID was not found in the library. Could not remove.\n";
            status = PlaylistStatus::PLAYLIST_NORMAL;  
            break;
        case PlaylistStatus::PLAYLIST_NORMAL:
        default:
            break;
    }
}
