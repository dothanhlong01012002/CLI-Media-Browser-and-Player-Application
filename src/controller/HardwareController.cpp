#include "HardwareController.hpp"
#include "ControllerManager.hpp"
#include <iostream>
#include <stdexcept>

// Send the "Play" command
void HardwareController::sendPlayCommand() {
    uart.writeData(PLAY_MODE);
}

// Send the "Pause" command
void HardwareController::sendPauseCommand() {
    uart.writeData(PAUSE_MODE);
}

// Send the "Stop" command
// Sends a "ST" signal through the UART connection to trigger the Stop action on the hardware.
void HardwareController::sendStopCommand() {
    uart.writeData(STOP_MODE);
}

// Send a custom signal
// Sends a custom string signal through the UART connection for hardware-specific actions.
void HardwareController::sendSignal(const std::string& signal) {
    uart.writeData(signal);
}

// Constructor
HardwareController::HardwareController(const std::string& device, int baudRate)
    : uart(device, baudRate), running(false) {
    uart.setDataCallback([this](const std::string& data) {
        handleReceivedData(data);
    });
}

// Destructor
HardwareController::~HardwareController() {
    stopReading(); 
}

void HardwareController::handleReceivedData(const std::string& data) {
    if (data == PLAY_MODE_RECEIVE) {
        ControllerManager::getInstance()->getPlayingMediaController()->play();
    } else if (data == PAUSE_MODE_RECEIVE) {
        ControllerManager::getInstance()->getPlayingMediaController()->pause();
    } else if (data == NEXT_MODE_RECEIVE){
        ControllerManager::getInstance()->getPlayingMediaController()->skipToNext();
    } else if (data == PREV_MODE_RECEIVE){
        ControllerManager::getInstance()->getPlayingMediaController()->skipToPrevious();
    } else {
        ControllerManager::getInstance()->getModelManager()->getPlayingMedia()->setVolume(std::stoi(data));
    }
}

void HardwareController::startReading() {
    if (running) {
        return; 
    }
    running = true;

    readThread = std::thread([this]() {
        try {
            uart.readData(); 
        } catch (const std::exception& e) {
            std::cerr << "Error in reading thread: " << e.what() << std::endl;
        }
    });
}

void HardwareController::stopReading() {
    if (!running) {
        return; 
    }
    running = false;
    uart.stop(); 
    if (readThread.joinable()) {
        readThread.join(); 
    }
}
