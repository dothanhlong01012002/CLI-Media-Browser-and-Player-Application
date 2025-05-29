#include "PlayingMediaView.hpp"
#include <iomanip>
#include <mutex>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <thread>
#include <atomic>
#include <chrono>

using namespace ftxui;

void PlayingMediaView::showPlayingMedia(PlayingMedia* player, size_t& currentTime, size_t totalTime, int& volume) {
    BaseView::showMenu();
    
    bool running = true;

    auto menu = Menu(&menu_entries, &selected_option);

    auto screen = ScreenInteractive::TerminalOutput();

    auto renderer = Renderer(menu, [&] {
        std::string current = (currentTime / 60 < 10 ? "0" : "") + std::to_string(currentTime / 60) + ":" +
                              (currentTime % 60 < 10 ? "0" : "") + std::to_string(currentTime % 60);
        std::string total = (player->getTotalTime() / 60 < 10 ? "0" : "") + std::to_string(player->getTotalTime() / 60) + ":" +
                            (player->getTotalTime() % 60 < 10 ? "0" : "") + std::to_string(player->getTotalTime() % 60);

        float progress = static_cast<float>(currentTime) / player->getTotalTime();
        int barWidth = 50;
        int pos = static_cast<int>(barWidth * progress);
        std::vector<Element> progress_bar_elements(barWidth, text("="));
        std::fill(progress_bar_elements.begin(), progress_bar_elements.begin() + pos, text("#"));
        auto progress_bar = hbox({
            text("["), 
            hbox(progress_bar_elements), 
            text("]")
        });

        int volumeBarWidth = 30;
        int volumePos = static_cast<int>(volumeBarWidth * (volume / 128.0));
        std::vector<Element> volume_bar_elements(volumeBarWidth, text("="));
        std::fill(volume_bar_elements.begin(), volume_bar_elements.begin() + volumePos, text("#"));
        auto volume_bar = hbox({
            text("["), 
            hbox(volume_bar_elements), 
            text("]")
        });

        std::string volumePercent = std::to_string(static_cast<int>((volume / 128.0) * 100)) + "%";

        return vbox({
                text("===== Now Playing ====="),
                text("Song: " + player->getCurrentMediaFile()->getName() + " - " + player->getCurrentMediaFile()->getMetadata().getMetadata()["Artist"]) | bold | color(Color::Green),
                progress_bar | color(Color::Yellow),
                text(current + " / " + total) | color(Color::Red),
                separator(),
                hbox({
                    text("Volume: "),
                    volume_bar,
                    text(" " + volumePercent) | color(Color::White)
                }) | color(Color::Cyan),
                menu->Render(),
                text("======================="),
                text("Use arrow keys to navigate, press Enter to select.") | color(Color::Red),
            }) |
            border;
    });

    std::thread refresh_thread([&] {
        size_t lastTime = currentTime;
        int lastVolume = volume;
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (lastTime != currentTime || lastVolume != volume) {
                lastTime = currentTime;
                lastVolume = volume;
                screen.PostEvent(Event::Custom); 
            }
        }
    });

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

    running = false;
    if (refresh_thread.joinable()) {
        refresh_thread.join();
    }

    std::system("clear");
}

int PlayingMediaView::getSelectedOption() const {
    return selected_option;
}

void PlayingMediaView::hideMenu() {
    BaseView::hideMenu();
    selected_option = 0;
    std::system("clear");
}

void PlayingMediaView::showMenu() {
    BaseView::showMenu();
}
