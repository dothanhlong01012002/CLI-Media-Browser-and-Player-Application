#include "MetadataView.hpp"
#include <iostream>
#include <iomanip>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp> 

using namespace ftxui;

void MetadataView::showMenu() {
    BaseView::showMenu();

    auto menu = Menu(&menu_entries, &selected_option);

    auto renderer = Renderer(menu, [&] {
        return vbox({
                   text("===== Metadata Menu ====="),
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
    std::system("clear");
}

void MetadataView::hideMenu() {
    BaseView::hideMenu();
    selected_option = 0;
    selected_option_audio = 0;
    selected_option_video = 0;
    std::system("clear");
}

// Get the selected menu option
int MetadataView::getSelectedOption() const {
    return selected_option;
}

void MetadataView::showFileMetadata(const std::shared_ptr<MediaFile>& file) {
    using namespace ftxui;

    auto metadata = file->getMetadata();

    auto header = hbox({
        text("METADATA OF: ") | bold,
        text(file->getName()) | color(Color::Yellow)
    }) | hcenter;

    Elements rows;
    for (const auto& entry : metadata.getMetadata()) {
        rows.push_back(hbox({
            text(entry.first + ":") | size(WIDTH, EQUAL, 15),  
            text(entry.second) | size(WIDTH, GREATER_THAN, 25) 
        }));
    }

    auto content = vbox(std::move(rows)) | border;

    auto document = vbox({
        header,
        separator(),
        content,
        separator(),
        text("Metadata Editing") | dim | hcenter
    });

    auto screen = Screen::Create(
        Dimension::Full(),        
        Dimension::Fit(document)   
    );
    Render(screen, document);

    std::cout << screen.ToString() << std::endl;
}

void MetadataView::menuEditAudio() {
    BaseView::showMenu();

    std::vector<std::string> audio_edit_entries = {
        "Edit Title",
        "Edit Artist",
        "Edit Album",
        "Edit Year",
        "Edit Track",
        "Edit Genre",
        "Back"
    };

    auto menu_audio = Menu(&audio_edit_entries, &selected_option_audio);

    auto renderer_audio = Renderer(menu_audio, [&] {
        return vbox({
            text("===== Audio Edit Menu =====") | bold | center,
            menu_audio->Render(),
            text("==========================="),
            text("Use arrow keys or mouse to navigate, press Enter or click to select."),
        }) |
        border;
    });

    auto screen = ScreenInteractive::TerminalOutput();

    auto event_handler = CatchEvent(renderer_audio, [&](Event event) {
        if (event.is_mouse()) {
            if (event.mouse().button == Mouse::Left && menu_audio->OnEvent(event)) {
                std::cout << "Audio Option " << selected_option_audio << " selected via mouse!" << std::endl;
                screen.ExitLoopClosure()();  
                return true;
            }
        }

        if (event == Event::Return) { 
            if (menu_audio->OnEvent(event)) {
                screen.ExitLoopClosure()();
                return true;
            }
        }

        if (event == Event::Escape || event == Event::Character('q')) { 
            std::cout << "Exiting Audio Edit menu..." << std::endl;
            screen.ExitLoopClosure()();
            return true;
        }

        return menu_audio->OnEvent(event); 
    });

    screen.Loop(event_handler);
}

void MetadataView::menuEditVideo() {
    BaseView::showMenu();

    std::vector<std::string> video_edit_entries = {
        "Edit Title",
        "Back"
    };

    auto menu_video = Menu(&video_edit_entries, &selected_option_video);

    auto renderer_video = Renderer(menu_video, [&] {
        return vbox({
            text("===== Video Edit Menu =====") | bold | hcenter,
            menu_video->Render(),
            text("==========================="),
            text("Use arrow keys or mouse to navigate, press Enter or click to select."),
        }) |
        border;
    });

    auto screen = ScreenInteractive::TerminalOutput();

    auto event_handler = CatchEvent(renderer_video, [&](Event event) {
        if (event.is_mouse()) {
            if (event.mouse().button == Mouse::Left && menu_video->OnEvent(event)) {
                std::cout << "Video Option " << selected_option_video << " selected via mouse!" << std::endl;
                screen.ExitLoopClosure()();  
                return true;
            }
        }

        if (event == Event::Return) {
            if (menu_video->OnEvent(event)) {
                screen.ExitLoopClosure()();
                return true;
            }
        }

        if (event == Event::Escape || event == Event::Character('q')) { 
            std::cout << "Exiting Video Edit menu..." << std::endl;
            screen.ExitLoopClosure()();
            return true;
        }

        return menu_video->OnEvent(event); 
    });

    screen.Loop(event_handler);
}

// Return the selected option for the Audio Edit menu
int MetadataView::getAuditoOption() const {
    return selected_option_audio;
}

// Return the selected option for the Video Edit menu
int MetadataView::getVideoOption() const {
    return selected_option_video;
}






