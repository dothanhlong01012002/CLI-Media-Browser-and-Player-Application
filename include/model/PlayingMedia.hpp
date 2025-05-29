#ifndef PLAYING_MEDIA_H
#define PLAYING_MEDIA_H

#include "MediaFile.hpp"
#include <memory>
#include <filesystem>
#include <atomic>
#include <mutex>
#include <stdint.h>
#include <thread>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/imgutils.h>
#include <libavutil/time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
}

namespace fs = std::filesystem;

class PlayingMedia {
private:
    std::vector<std::shared_ptr<MediaFile>> currentplaylist; 
    int volume;                                        
    size_t currentTime = 0;                                 
    size_t totalTime = 0;                                   
    size_t currentTrackIndex = -1;                          
    std::atomic<bool> paused{false};                        
    std::atomic<bool> playing{false};                        
    std::thread playbackThread;                              
    std::recursive_mutex stateMutex;                        

public:
    PlayingMedia();

    virtual std::shared_ptr<MediaFile> getCurrentMediaFile() const;

    virtual void setCurrentMediaFile(const std::shared_ptr<MediaFile>& mediaFile);
    virtual size_t& getCurrentTime();
    virtual std::string getDurationStringType() const;
    virtual size_t getTotalTime() const;

    virtual void setCurrentTime(size_t time);

    void playAudio(const char* filePath);
    void playVideo(const char* videoFilePath, const char* wavPath);
    bool isPlaying();

    void play();

    virtual void pauseMusic();

    virtual void resumeMusic();

    void stop();

    virtual void nextTrack();

    virtual void previousTrack();

    bool hasNextTrack() const;

    bool hasPrevTrack() const;

    void playCurrentTrack();

    virtual void setPlaylist(const std::vector<std::shared_ptr<MediaFile>>& newPlaylist);

    virtual void adjustVolume(size_t newVolume);
    virtual void setVolume(const int &value);
    virtual int& getVolume();
    void stopPlaybackThread();
    std::string extractAudio(const std::string &videoPath);
    virtual ~PlayingMedia();
};

#endif // PLAYING_MEDIA_H
