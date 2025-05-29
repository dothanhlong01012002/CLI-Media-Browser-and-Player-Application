#ifndef MEDIA_FILE_H
#define MEDIA_FILE_H

#include <string>
#include <map>
#include <vector>
#include "Metadata.hpp"
#include <queue>

enum MediaType {
    AUDIO,  
    VIDEO,   
    UNKNOWN 
};

class MediaFile {
private:
    static size_t mediaFileNextID;           
    static std::queue<int> mediaFileFreeIDs; 
    std::string ID;                          
    std::string name;                       
    std::string path;                       
    size_t duration;                         
    MediaType type;                          
    Metadata metadata;                      

public:
    MediaFile(const std::string& name = "", const std::string& path = "", MediaType type = AUDIO, const Metadata& metadata = Metadata());

    virtual void setName(const std::string& name);

    virtual void setPath(const std::string& path);

    virtual void setDuration(const size_t& duration);

    std::string getName() const;

    std::string getID() const;

    size_t getDuration() const;

    std::string getPath() const;

    MediaType getType() const;

    virtual Metadata& getMetadata();

    virtual void setMetadata(const Metadata& metadata);

    virtual void setType(MediaType type);

    virtual ~MediaFile();
};

#endif // MEDIA_FILE_H
