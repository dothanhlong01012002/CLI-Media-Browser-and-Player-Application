#ifndef MAIN_MENU_CONTROLLER_H
#define MAIN_MENU_CONTROLLER_H

#include "BaseController.hpp"

class MainMenuController : public BaseController {
public:
    void handleInput();

    void back();
};

#endif // MAIN_MENU_CONTROLLER_H