#include "BaseView.hpp"

// Constructor: Initializes the view with a hidden status
BaseView::BaseView():statusView(false) {}

// Show the menu: Sets the view status to visible
void BaseView::showMenu() {
    statusView = true;
}

// Hide the menu: Sets the view status to hidden
void BaseView::hideMenu() {
    statusView = false;
}

// Check if the menu is shown: Returns the visibility status of the view
bool BaseView::isShown() const {
    return statusView;
}

// Destructor: Ensures proper cleanup of the BaseView
BaseView::~BaseView() {}
