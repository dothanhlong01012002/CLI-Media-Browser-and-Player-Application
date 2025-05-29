#ifndef METADATA_VIEW_H
#define METADATA_VIEW_H

#include "BaseView.hpp"
#include <vector>
#include <string>
#include "MediaFile.hpp"
#include <memory>

enum MetadataMenu {
    EDIT_METADATA,        
    BACK_FROM_METADATA    
};

enum AudioEdit {
    AUDIO_TITLE,
    AUDIO_ARTIST,
    AUDIO_ALBUM,
    AUDIO_YEAR,
    AUDIO_TRACK,
    AUDIO_GENRE,
    AUDIO_BACK
};

enum VideoEdit {
    VIDEO_TITLE,
    VIDEO_BACK
};

class MetadataView : public BaseView {
private:
    std::vector<std::string> menu_entries = {
        "Edit File Metadata",
        "Go Back"
    };
    int selected_option = 0;
    int selected_option_audio = 0;        
    int selected_option_video = 0;
public:
    void showMenu() override;

    virtual int getSelectedOption() const;
    virtual int getAuditoOption() const;
    virtual int getVideoOption() const;
    ~MetadataView() override = default;

    void hideMenu() override;

    virtual void showFileMetadata(const std::shared_ptr<MediaFile>& file);

    virtual void menuEditAudio();

    virtual void menuEditVideo();
};

#endif // METADATA_VIEW_H
