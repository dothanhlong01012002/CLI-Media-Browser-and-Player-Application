#include "MediaFileView.hpp"
#include <iostream>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>
#include <vector>
#include <string>
#include <iomanip>

using namespace ftxui;

void MediaFileView::showMediaFilesPage(const std::vector<std::shared_ptr<MediaFile>>& files, size_t currentPage, size_t firstSong, size_t lastSong) {
    using namespace ftxui;

    Elements items;
    for (size_t i = firstSong; i <= lastSong && i < files.size(); ++i) {
        items.push_back(hbox({
            text("[" + files[i]->getID() + "] ") | size(WIDTH, EQUAL, 10), 
            text(files[i]->getName()) | size(WIDTH, GREATER_THAN, 30)      
        }));
    }

    std::string footer = "Page " + std::to_string(currentPage);

    auto document = vbox({
        text("Media Files") | bold | hcenter,   
        separator(),
        vbox(std::move(items)) | border,       
        separator(),
        text(footer) | hcenter                 
    });

    auto screen = Screen::Create(
        Dimension::Full(),         
        Dimension::Fit(document)   
    );
    Render(screen, document);

    std::cout << screen.ToString() << std::endl;
}

void MediaFileView::showMenu() {
    BaseView::showMenu();
    auto menu = Menu(&menu_entries, &selected_option);

    auto renderer = Renderer(menu, [&] {
        return vbox({
                   text("==== Media Player Menu ===="),
                   menu->Render(),
                   text("==========================="),
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
    std::system("clear");
}

// Get the selected menu option
int MediaFileView::getSelectedOption() const {
    return selected_option;
}

// Hide the menu and reset the selected option
void MediaFileView::hideMenu() {
    BaseView::hideMenu();
    selected_option = 0;
    std::system("clear");
}

void MediaFileView::displayStatusMessage(MediaFileStatus& status) {
    switch (status) {
        case MediaFileStatus::MEDIAFILE_PLAY_STATUS:
            std::cout << "Song ID not found in media library. Cannot play.\n";
            status = MediaFileStatus::MEDIAFILE_NORMAL; 
            break;
        case MediaFileStatus::MEDIAFILE_DETAIL_STATUS:
            std::cout << "Song ID not found in media library. Cannot show details.\n";
            status = MediaFileStatus::MEDIAFILE_NORMAL; 
            break;
        case MediaFileStatus::MEDIAFILE_NEXT_PAGE_STATUS:
            std::cout << "This is the last page. Cannot go next.\n";
            status = MediaFileStatus::MEDIAFILE_NORMAL;  
            break;
        case MediaFileStatus::MEDIAFILE_PREV_PAGE_STATUS:
            std::cout << "This is the first page. Cannot go back.\n";
            status = MediaFileStatus::MEDIAFILE_NORMAL;  
            break;
        case MediaFileStatus::MEDIAFILE_NORMAL:
        default:
            break;
    }
}
