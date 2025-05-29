#include "MetadataController.hpp"
#include "ControllerManager.hpp"
#include <iostream>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>
#include <filesystem>

namespace fs = std::filesystem;

// Handle user input for editing metadata
void MetadataController::handleInput(const std::string& ID) {
    MediaType type = ControllerManager::getInstance()->getModelManager()->getMediaLibrary()->getMediaFileByID(ID)->getType();
    do {
        // Hide the current view and display metadata of the selected file
        ControllerManager::getInstance()->getViewManager()->hideCurrentView();
        getMediaFileMetadata(ControllerManager::getInstance()->getModelManager()->getMediaLibrary()->getMediaFileByID(ID));

        // Switch to the metadata view
        ControllerManager::getInstance()->getViewManager()->switchView(SwitchView::SW_METADATA_VIEW);

        // Handle user input based on the selected metadata menu option
        switch (ControllerManager::getInstance()->getViewManager()->getMetadataView()->getSelectedOption()) {
            case MetadataMenu::BACK_FROM_METADATA: {
                back();
                break;
            }
            case MetadataMenu::EDIT_METADATA: {
                if (type == AUDIO) {
                    handleEditAudio(ControllerManager::getInstance()->getModelManager()->getMediaLibrary()->getMediaFileByID(ID)->getPath(), ID);
                } else {
                    handleEditVideo(ControllerManager::getInstance()->getModelManager()->getMediaLibrary()->getMediaFileByID(ID)->getPath(), ID);
                }
                break;
            }
        }
    } while (ControllerManager::getInstance()->getViewManager()->getMetadataView()->getSelectedOption() != MetadataMenu::BACK_FROM_METADATA);
}

// Retrieve and display metadata of the given media file
void MetadataController::getMediaFileMetadata(const std::shared_ptr<MediaFile>& file) const {
    ControllerManager::getInstance()->getViewManager()->getMetadataView()->showFileMetadata(file);
}

// Update the metadata of the specified media file
void MetadataController::updateMediaFileMetadata(const std::string& key, const std::string& value, const std::string& ID) {
    ControllerManager::getInstance()->getModelManager()->getMediaLibrary()->getMediaFileByID(ID)->getMetadata().setValue(key, value);
}

// Handle the back navigation logic
void MetadataController::back() {
}

void MetadataController::handleEditAudio(const std::string& filepath, const std::string& ID) {
    std::string newValue;
    TagLib::FileRef f(filepath.c_str()); 

    if (!f.isNull() && f.tag() && f.audioProperties()) {
        TagLib::Tag* tag = f.tag();
        do {
            ControllerManager::getInstance()->getViewManager()->hideCurrentView();
            getMediaFileMetadata(ControllerManager::getInstance()->getModelManager()->getMediaLibrary()->getMediaFileByID(ID));
            ControllerManager::getInstance()->getViewManager()->getMetadataView()->menuEditAudio();

            // Handle user input for different audio metadata fields.
            switch (ControllerManager::getInstance()->getViewManager()->getMetadataView()->getAuditoOption()) {
                case AudioEdit::AUDIO_BACK: {
                    back(); 
                    break;
                }
                case AudioEdit::AUDIO_TITLE: {
                    Exception_Handler("Enter new title: ", newValue, validateAlphaSring); 
                    tag->setTitle(TagLib::String(newValue)); 
                    updateMediaFileMetadata("Title", newValue, ID); 
                    break;
                }
                case AudioEdit::AUDIO_ARTIST: {
                    Exception_Handler("Enter new artist: ", newValue, validateAlphaSring); 
                    tag->setArtist(TagLib::String(newValue)); 
                    updateMediaFileMetadata("Artist", newValue, ID); 
                    break;
                }
                case AudioEdit::AUDIO_ALBUM: {
                    Exception_Handler("Enter new album: ", newValue, validateAlphaSring); 
                    tag->setAlbum(TagLib::String(newValue)); 
                    updateMediaFileMetadata("Album", newValue, ID); 
                    break;
                }
                case AudioEdit::AUDIO_YEAR: {
                    Exception_Handler("Enter new year: ", newValue, validateYear); 
                    tag->setYear(std::stoi(newValue)); 
                    updateMediaFileMetadata("Year", newValue, ID); 
                    break;
                }
                case AudioEdit::AUDIO_TRACK: {
                    Exception_Handler("Enter new track: ", newValue, validateTrack); 
                    tag->setTrack(std::stoi(newValue)); 
                    updateMediaFileMetadata("Track", newValue, ID); 
                    break;
                }
                case AudioEdit::AUDIO_GENRE: {
                    Exception_Handler("Enter new genre: ", newValue, validateAlphaSring); 
                    tag->setGenre(TagLib::String(newValue)); 
                    updateMediaFileMetadata("Genre", newValue, ID); 
                    break;
                }
            }
        } while (ControllerManager::getInstance()->getViewManager()->getMetadataView()->getAuditoOption() != AudioEdit::AUDIO_BACK);
        f.save(); 
    } else {
        std::cerr << "Error: Could not open file or retrieve metadata for " << filepath << std::endl;
    }
}

void MetadataController::handleEditVideo(const std::string& filepath, const std::string& ID) {
    std::string newValue;
    TagLib::FileRef f(filepath.c_str()); 

    if (!f.isNull() && f.tag() && f.audioProperties()) {
        TagLib::Tag* tag = f.tag(); 
        do {
            ControllerManager::getInstance()->getViewManager()->hideCurrentView();
            getMediaFileMetadata(ControllerManager::getInstance()->getModelManager()->getMediaLibrary()->getMediaFileByID(ID));
            ControllerManager::getInstance()->getViewManager()->getMetadataView()->menuEditVideo();

            // Handle user input for different video metadata fields.
            switch (ControllerManager::getInstance()->getViewManager()->getMetadataView()->getVideoOption()) {
                case VideoEdit::VIDEO_BACK: {
                    back(); 
                    break;
                }
                case VideoEdit::VIDEO_TITLE: {
                    Exception_Handler("Enter new title: ", newValue, validateAlphaSring); 
                    tag->setTitle(TagLib::String(newValue)); 
                    updateMediaFileMetadata("Title", newValue, ID); 
                    break;
                }
            }
        } while (ControllerManager::getInstance()->getViewManager()->getMetadataView()->getVideoOption() != VideoEdit::VIDEO_BACK);
        f.save(); 
    } else {
        std::cerr << "Error: Could not open file or retrieve metadata for " << filepath << std::endl;
    }
}
