#ifndef SCANF_OPTION_VIEW_H
#define SCANF_OPTION_VIEW_H

#include "BaseView.hpp"
#include <vector>
#include <string>

enum ScanStatus {
    SCAN_NORMAL,              
    SCAN_DIRECTORY_SUCCESS,  
    SCAN_DIRECTORY_FAILURE,   
    SCAN_USB_SUCCESS,         
    SCAN_USB_FAILURE         
};

enum ScanfMenu {
    SCANF_DIRECTORY,          
    SCANF_USB,              
    BACK_FROM_SCAN           
};

class ScanfOptionView : public BaseView {
public:
    void showMenu() override;

    virtual int getSelectedOption() const;

    ~ScanfOptionView() override = default;

    void hideMenu() override;

    virtual void displayStatusMessage(ScanStatus& status);

private:
    int selected_option = 0;                     
    std::vector<std::string> menu_entries = {    
        "Scan Directory",
        "Scan USB Device",
        "Go Back"
    };
};

#endif // SCANF_OPTION_VIEW_H