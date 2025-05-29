#include "PlayingMedia.hpp"
#include <iostream>

// A simple audio packet queue
std::queue<AVPacket*> audioQueue;
std::mutex audioQueueMutex;

// Audio settings
int64_t audio_out_channel_layout = AV_CH_LAYOUT_STEREO;
int audio_out_channels = av_get_channel_layout_nb_channels(audio_out_channel_layout);
int audio_out_sample_rate = 44100;
AVSampleFormat audio_out_sample_fmt = AV_SAMPLE_FMT_S16;

// Get the currently playing media file
std::shared_ptr<MediaFile> PlayingMedia::getCurrentMediaFile() const {
    if (currentplaylist.empty() || currentTrackIndex < 0 || currentTrackIndex >= currentplaylist.size()) {
        return nullptr; // Return nullptr if no valid track is available
    }
    return currentplaylist[currentTrackIndex];
}

// Set the current media file for playback
void PlayingMedia::setCurrentMediaFile(const std::shared_ptr<MediaFile>& mediaFile) {
    if (!mediaFile) {
        std::cerr << "Error: Invalid media file." << std::endl;
        return;
    }
    if (!getCurrentMediaFile() || (mediaFile->getID() != getCurrentMediaFile()->getID())) {
        for (size_t i = 0; i < currentplaylist.size(); ++i) {
            if (currentplaylist[i]->getID() == mediaFile->getID()) {
                stop();
                {
                    std::unique_lock<std::recursive_mutex> lock(stateMutex);
                    currentTrackIndex = i;
                }
                play();
                return;
            }
        }
    }
}

// Set a new playlist for playback
void PlayingMedia::setPlaylist(const std::vector<std::shared_ptr<MediaFile>>& newPlaylist) {
    if (newPlaylist != currentplaylist) {
        stop(); 
        std::unique_lock<std::recursive_mutex> lock(stateMutex);
        currentplaylist = newPlaylist; 
    }
}
// Get the current playback time
size_t& PlayingMedia::getCurrentTime() {
    return currentTime;
}
// Get duration of the current mediafile with string type
std::string PlayingMedia::getDurationStringType() const {
    std::string duration = (totalTime / 60 < 10 ? "0" : "") + std::to_string(totalTime / 60) +
                            (totalTime % 60 < 10 ? "0" : "") + std::to_string(totalTime % 60);
    return duration;
};

// Get duration of the current mediafile
size_t PlayingMedia::getTotalTime() const {
    return totalTime;
}

// Set the current playback time
void PlayingMedia::setCurrentTime(size_t time) {
    currentTime = time;
}

// Constructor: Initialize the volume and playback settings
PlayingMedia::PlayingMedia() : volume(50) {
    Mix_VolumeMusic(volume);
}

// Destructor: Stop playback and clean up resources
PlayingMedia::~PlayingMedia() {
    stopPlaybackThread();
}

// Play an audio file
void PlayingMedia::playAudio(const char* filePath) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        throw std::runtime_error("Failed to initialize SDL_mixer: " + std::string(Mix_GetError()));
    }

    Mix_Music* music = Mix_LoadMUS(filePath);
    if (!music) {
        Mix_CloseAudio();
        throw std::runtime_error("Failed to load audio file: " + std::string(Mix_GetError()));
    }

    playing = true;
    Mix_PlayMusic(music, 1);

    while (Mix_PlayingMusic()) {
        {
            std::unique_lock<std::recursive_mutex> lock(stateMutex);
            Mix_VolumeMusic(volume);
            if (!playing) {
                Mix_HaltMusic();
                break;
            }
            if (paused) {
                Mix_PauseMusic();
                while (paused && playing) {
                    lock.unlock();
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    lock.lock();
                }
                Mix_ResumeMusic();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    Mix_FreeMusic(music);
    Mix_CloseAudio();
}

void PlayingMedia::play() {
    // Check if the playlist is empty
    if (currentplaylist.empty()) {
        return;
    }

    stopPlaybackThread(); 

    {
        std::unique_lock<std::recursive_mutex> lock(stateMutex);
        currentTime = 0;
        totalTime = currentplaylist[currentTrackIndex]->getDuration();
    }
    playbackThread = std::thread(&PlayingMedia::playCurrentTrack, this);
}

void PlayingMedia::pauseMusic() {
    // Lock the state and check if playback can be paused
    std::unique_lock<std::recursive_mutex> lock(stateMutex);
    if (!playing || paused) {
        return;
    }
    paused = true;
    Mix_PauseMusic(); 
}

void PlayingMedia::resumeMusic() {
    // Lock the state and check if playback can be resumed
    std::unique_lock<std::recursive_mutex> lock(stateMutex);
    if (!playing || !paused) {
        return;
    }
    paused = false;
    Mix_ResumeMusic();
}

void PlayingMedia::stop() {
    // Stop the playback thread and reset the state
    stopPlaybackThread();
}

bool PlayingMedia::isPlaying() {
    // Check if SDL_mixer is currently playing music
    return Mix_PlayingMusic();
}

void PlayingMedia::adjustVolume(size_t newVolume) {
    // Adjust the playback volume and apply it to SDL_mixer
    volume = newVolume;
    Mix_VolumeMusic(volume);
}

void PlayingMedia::stopPlaybackThread() {
    // Check if the playback thread is running and join it
    if (playbackThread.joinable()) {
        {
            std::unique_lock<std::recursive_mutex> lock(stateMutex);
            playing = false;
        }
        playbackThread.join();
    }
}

void PlayingMedia::playCurrentTrack() {
    {
        std::unique_lock<std::recursive_mutex> lock(stateMutex);
        playing = true;
        paused = false;
    }

    if (!currentplaylist.empty()) {
        std::string currentTrackPath = currentplaylist[currentTrackIndex]->getPath();
        if (currentplaylist[currentTrackIndex]->getType() == MediaType::AUDIO) {
            playAudio(currentTrackPath.c_str()); 
        } else {
            std::string wavPath = extractAudio(currentTrackPath);
            playVideo(currentTrackPath.c_str(),wavPath.c_str()); 
        }
    } else {
        std::cerr << "No tracks available to play.\n";
    }

    {
        std::unique_lock<std::recursive_mutex> lock(stateMutex);
        playing = false;
    }
}

void PlayingMedia::nextTrack() {
    // Stop current playback and move to the next track
    stop();
    {
        std::unique_lock<std::recursive_mutex> lock(stateMutex);
        currentTrackIndex = (currentTrackIndex + 1) % currentplaylist.size();
    }
    play(); 
}

void PlayingMedia::previousTrack() {
    // Stop current playback and move to the previous track
    stop();
    {
        std::unique_lock<std::recursive_mutex> lock(stateMutex);
        currentTrackIndex = (currentTrackIndex == 0) ? currentplaylist.size() - 1 : currentTrackIndex - 1;
    }
    play(); 
}

bool PlayingMedia::hasNextTrack() const {
    // Check if there is a next track available in the playlist
    return currentTrackIndex >= 0 && currentTrackIndex < currentplaylist.size() - 1;
}

bool PlayingMedia::hasPrevTrack() const {
    // Check if there is a previous track available in the playlist
    return currentTrackIndex > 0 && currentTrackIndex < currentplaylist.size();
}

std::string PlayingMedia::extractAudio(const std::string &videoPath) {
    std::string outputFolderPath = "./resources/musicfrommp4";
    std::string outputAudioPath = outputFolderPath + "/" + fs::path(videoPath).stem().string() + ".wav";
    std::string command = "ffmpeg -i \"" + videoPath + "\" -q:a 0 -map a \"" + outputAudioPath + "\" -y";

    if(!fs::exists(outputFolderPath)){
        fs::create_directories(outputFolderPath);
    }

    if (fs::exists(outputAudioPath)) {
        return outputAudioPath;
    }
    
    int result = system(command.c_str());
    if(result != 0){
        std::cerr << "Failed to extract audio using FFmpeg.\n";
        return "";
    }

    std::cout <<  "Audio extracted to: " << outputAudioPath << "\n";
    return outputAudioPath;
} 

void PlayingMedia::playVideo(const char* videoFilePath, const char* wavPath) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        throw std::runtime_error("Failed to initialize SDL: " + std::string(SDL_GetError()));
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << "\n";
        SDL_Quit();
        return;
    }

    // Load the music
    Mix_Music* music = Mix_LoadMUS(wavPath);
    if (!music) {
        std::cerr << "Failed to load music: " << Mix_GetError() << "\n";
        Mix_CloseAudio();
        SDL_Quit();
        return;
    }

    // Play the music
    if (Mix_PlayMusic(music, -1) == -1) {
        std::cerr << "Failed to play music: " << Mix_GetError() << "\n";
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        SDL_Quit();
        return;
    }

    // Video playback logic
    avformat_network_init();
    AVFormatContext* formatContext = nullptr;

    if (avformat_open_input(&formatContext, videoFilePath, nullptr, nullptr) != 0) {
        std::cerr << "Failed to open video file: " << videoFilePath << "\n";
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        SDL_Quit();
        return;
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        avformat_close_input(&formatContext);
        std::cerr << "Failed to retrieve stream info.\n";
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        SDL_Quit();
        return;
    }

    int videoStreamIndex = -1;
    for (unsigned i = 0; i < formatContext->nb_streams; ++i) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && videoStreamIndex == -1) {
            videoStreamIndex = i;
        }
    }

    if (videoStreamIndex == -1) {
        avformat_close_input(&formatContext);
        std::cerr << "No video stream found.\n";
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        SDL_Quit();
        return;
    }

    AVCodecParameters* videoCodecParams = formatContext->streams[videoStreamIndex]->codecpar;
    AVCodec* videoCodec = avcodec_find_decoder(videoCodecParams->codec_id);
    if (!videoCodec) {
        avformat_close_input(&formatContext);
        std::cerr << "Unsupported video codec.\n";
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        SDL_Quit();
        return;
    }

    AVCodecContext* videoCodecContext = avcodec_alloc_context3(videoCodec);
    avcodec_parameters_to_context(videoCodecContext, videoCodecParams);
    if (avcodec_open2(videoCodecContext, videoCodec, nullptr) < 0) {
        avcodec_free_context(&videoCodecContext);
        avformat_close_input(&formatContext);
        std::cerr << "Failed to open video codec.\n";
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        SDL_Quit();
        return;
    }

    SDL_Window* window = SDL_CreateWindow(getCurrentMediaFile()->getName().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          videoCodecContext->width, videoCodecContext->height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING,
                                             videoCodecContext->width, videoCodecContext->height);

    SwsContext* swsContext = sws_getContext(videoCodecContext->width, videoCodecContext->height, videoCodecContext->pix_fmt,
                                            videoCodecContext->width, videoCodecContext->height, AV_PIX_FMT_YUV420P,
                                            SWS_BILINEAR, nullptr, nullptr, nullptr);

    AVFrame* frame = av_frame_alloc();
    AVFrame* frameYUV = av_frame_alloc();
    int bufferSize = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, videoCodecContext->width, videoCodecContext->height, 1);
    uint8_t* buffer = (uint8_t*)av_malloc(bufferSize);
    av_image_fill_arrays(frameYUV->data, frameYUV->linesize, buffer, AV_PIX_FMT_YUV420P,
                         videoCodecContext->width, videoCodecContext->height, 1);

    AVPacket packet;
    bool quit = false;

    AVRational timeBase = formatContext->streams[videoStreamIndex]->time_base;

    // Đồng bộ hóa âm thanh và video dựa trên PTS
    auto getCurrentTimeMs = []() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::steady_clock::now().time_since_epoch())
            .count();
    };

    int64_t startTime = getCurrentTimeMs();

    while (av_read_frame(formatContext, &packet) >= 0 && !quit) {
        if (packet.stream_index == videoStreamIndex) {
            if (avcodec_send_packet(videoCodecContext, &packet) == 0) {
                while (avcodec_receive_frame(videoCodecContext, frame) == 0) {
                    int64_t ptsMs = av_rescale_q(frame->pts, timeBase, AVRational{1, 1000});

                    while (getCurrentTimeMs() < startTime + ptsMs) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    }

                    sws_scale(swsContext, frame->data, frame->linesize, 0, videoCodecContext->height,
                              frameYUV->data, frameYUV->linesize);

                    SDL_UpdateYUVTexture(texture, nullptr,
                                         frameYUV->data[0], frameYUV->linesize[0],
                                         frameYUV->data[1], frameYUV->linesize[1],
                                         frameYUV->data[2], frameYUV->linesize[2]);

                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
                    SDL_RenderPresent(renderer);

                    SDL_Event event;
                    while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                            quit = true;
                        }
                    }
                    {
                        std::unique_lock<std::recursive_mutex> lock(stateMutex);
                        Mix_VolumeMusic(volume);
                        while (paused) {
                            lock.unlock(); 
                            std::this_thread::sleep_for(std::chrono::milliseconds(100));
                            lock.lock();

                            if (!playing) {
                                quit = true;
                                break;
                            }
                        }
                        if (!playing || quit) {
                            quit = true;
                            break;
                        }
                    }
                }
            }
        }
        av_packet_unref(&packet);
        {
            std::unique_lock<std::recursive_mutex> lock(stateMutex);
            if (!playing || quit) {
                quit = true;
                break;
            }
        }
    }

    av_free(buffer);
    av_frame_free(&frameYUV);
    av_frame_free(&frame);
    sws_freeContext(swsContext);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_HaltMusic();
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();
    avcodec_free_context(&videoCodecContext);
    avformat_close_input(&formatContext);
}

void PlayingMedia::setVolume(const int &value){
    volume = value;
}

int& PlayingMedia::getVolume() {
    return volume;
}


