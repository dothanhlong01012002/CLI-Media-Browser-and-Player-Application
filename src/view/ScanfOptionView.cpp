#include "ScanfOptionView.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp> 
#include <iostream>
using namespace ftxui;

void ScanfOptionView::showMenu() {
    BaseView::showMenu(); 
    auto menu = Menu(&menu_entries, &selected_option);

    auto renderer = Renderer(menu, [&] {
        return vbox({
                   text("===== Scanf Option Menu ====="),
                   menu->Render(), 
                   text("============================="),
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

int ScanfOptionView::getSelectedOption() const {
    // Return the currently selected menu option index
    return selected_option;
}

void ScanfOptionView::hideMenu() {
    BaseView::hideMenu(); 
    selected_option = 0;  
    std::system("clear"); 
}

void ScanfOptionView::displayStatusMessage(ScanStatus& status) {
    switch (status) {
        case ScanStatus::SCAN_DIRECTORY_SUCCESS:
            std::cout << "Directory scan successful!\n";
            status = ScanStatus::SCAN_NORMAL;  
            break;
        case ScanStatus::SCAN_DIRECTORY_FAILURE:
            std::cout << "Directory scan failed! Please check the path and try again.\n";
            status = ScanStatus::SCAN_NORMAL;  
            break;
        case ScanStatus::SCAN_USB_SUCCESS:
            std::cout << "USB device scan successful!\n";
            status = ScanStatus::SCAN_NORMAL;  
            break;
        case ScanStatus::SCAN_USB_FAILURE:
            std::cout << "USB device scan failed! Device not found.\n";
            status = ScanStatus::SCAN_NORMAL;  
            break;
        case ScanStatus::SCAN_NORMAL:
        default:
            break;
    }
}
