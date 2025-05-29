#ifndef CONTROLLER_MANAGER_H
#define CONTROLLER_MANAGER_H

#include <iostream>
#include <vector>
#include <memory>
#include "ScanfOptionController.hpp"
#include "PlaylistController.hpp"
#include "MediaFileController.hpp"
#include "PlayingMediaController.hpp"
#include "DetailedPlaylistController.hpp"
#include "MetadataController.hpp"
#include "BaseController.hpp"
#include "HardwareController.hpp"
#include "ViewManager.hpp"
#include "ModelManager.hpp"
#include "ExceptionLib.hpp"
#include "MainMenuController.hpp"

enum modeController {
    CT_MAIN,             
    CT_SCANF,             
    CT_PLAYLIST,          
    CT_MEDIA_FILES,        
    CT_PLAYING_MEDIA,     
    CT_DETAILED_PLAYLIST,  
    CT_METADATA,           
    CT_HARDWARE           
};

class ControllerManager {
private:
    static ControllerManager* instance; 
    std::vector<std::unique_ptr<BaseController>> controllers;
    ViewManager* views; 
    ModelManager* model; 
    ControllerManager(ViewManager* viewManager, ModelManager* modelManager);

public:
    friend class MockControllerManager;
    
    ControllerManager(const ControllerManager&) = delete;
    
    ControllerManager& operator=(const ControllerManager&) = delete;

    static ControllerManager* getInstance(ViewManager* viewManager = nullptr, ModelManager* modelManager = nullptr);

    virtual MainMenuController* getMainMenuController() const; 

    virtual ScanfOptionController* getScanfOptionController() const;  

    virtual PlaylistController* getPlaylistController() const; 

    virtual MediaFileController* getMediaFileController() const; 

    virtual PlayingMediaController* getPlayingMediaController() const;      

    virtual DetailedPlaylistController* getDetailedPlaylistController() const; 

    virtual MetadataController* getMetadataController() const;   

    virtual ViewManager* getViewManager() const;   
                             
    virtual ModelManager* getModelManager() const;  

    virtual HardwareController* getHardwareController() const;    

    static void resetInstance();

    virtual ~ControllerManager() = default;            
};

#endif // CONTROLLER_MANAGER_H
