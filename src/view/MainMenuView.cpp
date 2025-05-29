#include "MainMenuView.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp> // For using text, vbox, border
#include <iostream>
using namespace ftxui;

void MainMenuView::showMenu() {
    BaseView::showMenu();
    auto menu = Menu(&menu_entries, &selected_option);

    auto renderer = Renderer(menu, [&] {
        return vbox({
                   text("===== Main Menu ====="),
                   menu->Render(),
                   text("====================="),
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
int MainMenuView::getSelectedOption() const {
    return selected_option;
}

// Hide the menu and reset the selected option
void MainMenuView::hideMenu() {
    BaseView::hideMenu();
    selected_option = 0;
    std::system("clear");
}
