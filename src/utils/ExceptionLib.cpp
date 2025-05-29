#include "ExceptionLib.hpp"

// Validates if a string contains only alphabetic characters and spaces.
void validateAlphaSring(const std::string& alString) {
    std::regex alStringTemplate(R"([a-zA-Z\s]+)"); 
    if (!std::regex_match(alString, alStringTemplate)) {
        throw InvalidAlphaSringException(); 
    }
}

// Validates if the year is in the correct format and within the valid range.
void validateYear(const std::string& year) {
    std::regex YearTemplate("\\d{4}$"); 
    if (!std::regex_match(year, YearTemplate)) {
        throw InvalidYearException();
    }
    size_t _year = std::stoi(year);
    if (_year > 2025) { 
        throw InvalidYearException();
    }
}

// Validates if a given value is a positive integer.
void validatePosInteger(const size_t& value) {
    if (value < 0) { 
        throw InvalidInputIntegerException();
    }
}

// Validates menu choice for the Scanf Menu.
void validateScanfMenu(const size_t& value) {
    if (value < 0 || value > 2) { 
        throw InvalidMenuChoiceException();
    }
}

// Validates menu choice for the Main Menu.
void validateMainMenu(const size_t& value) {
    if (value < 0 || value > 4) { 
        throw InvalidMenuChoiceException();
    }
}

// Validates menu choice for Media Files Menu.
void validateMediaFilesMenu(const size_t& value) {
    if (value < 0 || value > 4) { 
        throw InvalidMenuChoiceException();
    }
}

// Validates menu choice for Metadata Menu.
void validateMetadataMenu(const size_t& value) {
    if (value < 0 || value > 1) { 
        throw InvalidMenuChoiceException();
    }
}

// Validates menu choice for Edit Video Menu.
void validateEditVideoMenu(const size_t& value) {
    if (value < 0 || value > 1) { 
        throw InvalidMenuChoiceException();
    }
}

// Validates menu choice for Edit Audio Menu.
void validateEditAudioMenu(const size_t& value) {
    if (value < 0 || value > 6) { 
        throw InvalidMenuChoiceException();
    }
}

// Validates menu choice for Playlists Menu.
void validatePlaylistsMenu(const size_t& value) {
    if (value < 0 || value > 3) { 
        throw InvalidMenuChoiceException();
    }
}

// Validates menu choice for Detailed Playlist Menu.
void validateDetailedPlaylistMenu(const size_t& value) {
    if (value < 0 || value > 4) { 
        throw InvalidMenuChoiceException();
    }
}

// Validates menu choice for Playing Media Menu.
void validatePlayingMediaMenu(const size_t& value) {
    if (value < 0 || value > 4) { 
        throw InvalidMenuChoiceException();
    }
}

// Validates if the volume is within the valid range (0-128).
void validateVolume(const size_t& value) {
    if (value < 0 || value > 128) { 
        throw InvalidVolumeException();
    }
}

// Validates if an ID contains only digits.
void validateID(const std::string& ID) {
    std::regex idTemplate(R"(\d+)"); 
    if (!std::regex_match(ID, idTemplate)) {
        throw InvalidIDException();
    }
}

// Validates if a track value contains only digits.
void validateTrack(const std::string& value) {
    std::regex TrackTemplate(R"(\d+)"); 
    if (!std::regex_match(value, TrackTemplate)) {
        throw InvalidTrackException();
    }
}

namespace fs = std::filesystem;

// Validates if a given path exists and is a directory.
void validatePath(const std::string& path) {
    fs::path p(path);
    if (!fs::exists(p) || !fs::is_directory(p)) { 
        throw InvalidPathException();
    }
}

// Checks if a USB device exists.
bool isUSBDevicePresent(const std::string& device) {
    std::ifstream file(device); 
    return file.good(); 
}

// Exception handling wrapper for string inputs.
void Exception_Handler_1(const std::string& str, std::string& variable, void(*func)(const std::string&)) {
    while (1) {
        try {
            std::cout << str; 
            std::getline(std::cin, variable); 
            func(variable); 
            break;
        } catch (const std::exception& e) {
            std::cout << "Caught exception: " << e.what() << std::endl; // Print the error message.
        }
    }
}

// Exception handling wrapper for size_t inputs.
void Exception_Handler_2(const std::string& str, size_t& variable, void(*func)(const size_t&)) {
    while (1) {
        try {
            std::cout << str; 
            std::cin >> variable; 
            if (std::cin.fail()) { 
                throw InvalidInputIntegerException();
            }
            func(variable); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            break; 
        } catch (const std::exception& e) {
            std::cout << "Caught exception: " << e.what() << std::endl; 
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        }
    }
}

std::function<void(const std::string&, std::string&, void(*)(const std::string&))> Exception_Handler = Exception_Handler_1;
std::function<void(const std::string&, size_t& , void(*func)(const size_t&))> Exception_Handler_Int = Exception_Handler_2;
