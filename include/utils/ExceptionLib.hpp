#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <bits/stdc++.h>

// Exception for invalid alpha string input
class InvalidAlphaSringException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid input. Only alpha characters are allowed.";
    }
};

// Exception for invalid year input
class InvalidYearException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Year must have 4 numeric characters.";
    }
};

// Exception for invalid integer input
class InvalidInputIntegerException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid input. Input must be a positive integer value.";
    }
};

// Exception for invalid menu choice
class InvalidMenuChoiceException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid choice. Please enter a valid option from the menu.";
    }
};

// Exception for invalid ID input
class InvalidIDException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid ID. ID must contain only digits.";
    }
};

// Exception for invalid track number
class InvalidTrackException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid track number. Track number must contain only digits.";
    }
};

// Exception for invalid path
class InvalidPathException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid path. The path does not exist or is not a valid directory.";
    }
};

// Exception for invalid volume input
class InvalidVolumeException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid volume. The volume must be between 0 and 128.";
    }
};

// Validation functions
void validateVolume(const size_t& value);
void validateScanfMenu(const size_t& value);
void validateMainMenu(const size_t& value);
void validateMediaFilesMenu(const size_t& value);
void validateMetadataMenu(const size_t& value);
void validateEditVideoMenu(const size_t& value);
void validateEditAudioMenu(const size_t& value);
void validatePlaylistsMenu(const size_t& value);
void validateDetailedPlaylistMenu(const size_t& value);
void validatePlayingMediaMenu(const size_t& value);

void validateID(const std::string& ID);
void validateTrack(const std::string& value); 
void validatePath(const std::string& path);
bool isUSBDevicePresent(const std::string& device);

void validateAlphaSring(const std::string& alString); 
void validateYear(const std::string& year); 
void validatePosInteger(const size_t& value); 

// Exception handlers for strings and integers
void Exception_Handler_1(const std::string& str, std::string& variable, void(*func)(const std::string&));
void Exception_Handler_2(const std::string& str, size_t& variable, void(*func)(const size_t&));

extern std::function<void(const std::string&, std::string&, void(*)(const std::string&))> Exception_Handler;
extern std::function<void(const std::string&, size_t& , void(*func)(const size_t&))> Exception_Handler_Int;
#endif // EXCEPTION_H
