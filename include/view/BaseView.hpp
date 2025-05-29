#ifndef BASE_VIEW_H
#define BASE_VIEW_H

#include <string>

class BaseView {
protected:
    bool statusView; 

public:
    BaseView();

    virtual void showMenu();

    virtual void hideMenu();

    bool isShown() const;

    virtual ~BaseView();
};

#endif // BASE_VIEW_H