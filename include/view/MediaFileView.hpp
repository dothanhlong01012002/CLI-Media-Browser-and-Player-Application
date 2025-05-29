#ifndef MEDIA_FILE_VIEW_H
#define MEDIA_FILE_VIEW_H

#include "BaseView.hpp"
#include "MediaFile.hpp"
#include <vector>
#include <string>
#include <memory>

enum MediaFileMenu {
    PLAY_SONG_FROM_FILES,   
    SHOW_DETAIL,          
    NEXT_PAGE,               
    PREV_PAGE,              
    BACK_FROM_MEDIA          
};

enum MediaFileStatus {
    MEDIAFILE_NORMAL,            
    MEDIAFILE_PLAY_STATUS,        
    MEDIAFILE_DETAIL_STATUS,      
    MEDIAFILE_NEXT_PAGE_STATUS,  
    MEDIAFILE_PREV_PAGE_STATUS    
};

class MediaFileView : public BaseView {
private:
    std::vector<std::string> menu_entries = {
        "Play",
        "Show Media File Details",
        "Show Next Page",
        "Show Previous Page",
        "Go Back"
    };
    int selected_option = 0;

public:
    void showMenu() override;

    virtual int getSelectedOption() const;

    ~MediaFileView() override = default;

    virtual void showMediaFilesPage(const std::vector<std::shared_ptr<MediaFile>>& files, size_t currentPage, size_t firstSong, size_t lastSong);
    
    void hideMenu() override;

    virtual void displayStatusMessage(MediaFileStatus& status);
};

#endif // MEDIA_FILE_VIEW_H
