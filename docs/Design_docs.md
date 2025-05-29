**Design Document for CLI Media Player Application**

---

## 1. Overview

This document outlines the design of a Command-Line Interface (CLI) Media Player Application, which supports browsing, playing, and managing media files (audio and video). It integrates with the S32K144 board for hardware control functionalities, ensuring seamless software and hardware interaction. The application follows the Model-View-Controller (MVC) architecture.

---

## 2. Architecture Design

### 2.1 MVC Pattern

The application is designed using the MVC pattern:

- **Model:** Manages the data and logic related to media files, playlists, and metadata.
- **View:** Handles CLI interactions and communicates with the S32K144 board for hardware feedback.
- **Controller:** Implements core logic, coordinating data and user interactions.

---

## 3. High-Level Components

### 3.1 Models

**MediaFile:** Represents an individual media file.

- Attributes:
  - `name`: File name.
  - `path`: File path.
  - `type`: Media type (audio/video).
  - `size`: File size.
  - `metadata`: Key-value pairs storing metadata.

**Playlist:** Represents a collection of media files.

- Attributes:
  - `name`: Playlist name.
  - `files`: List of MediaFile objects.

### 3.2 Views

**CLIView:** Handles CLI-based user interactions.

- Functions:
  - Display lists (media files, playlists, metadata).
  - Accept user inputs for actions (e.g., play, pause, next).

**DeviceView:** Interfaces with the S32K144 board.

- Functions:
  - Display song details and volume on the board screen.
  - Handle volume adjustments and playback controls via hardware buttons.

### 3.3 Controllers

**FileController:** Manages file scanning.

- Functions:
  - `scanDirectory`: List media files in a directory.
  - `scanUSBDevice`: List media files on USB devices.

**PlaybackController:** Controls media playback.

- Attributes:
  - `currentFileIndex`: Index of the currently playing file.
  - `volume`: Playback volume level.
- Functions:
  - `play`, `pause`, `stop`, `next`, `previous`.

**MetadataController:** Manages metadata operations.

- Functions:
  - `getMetadata`: Retrieve metadata of a file.
  - `updateMetadata`: Edit or add metadata fields.

**PlaylistController:** Handles playlist management.

- Functions:
  - `createPlaylist`, `updatePlaylist`, `deletePlaylist`, `viewPlaylist`.

**DeviceController:** Interfaces with the S32K144 board.

- Functions:
  - `controlVolume`: Adjust volume using ADC.
  - `controlPlayback`: Handle playback actions via hardware buttons.

---

## 4. Interaction Flow

### Example: Playing a Song

1. User selects a media file.
2. The **PlaybackController** invokes SDL2 for playback.
3. The **DeviceView** displays the song details on the S32K144 board.
4. User controls playback or adjusts volume via CLI or board buttons.

---

## 5. Class Diagram

```
Models:
--------
+-------------------+
|   MediaFile       |
+-------------------+
| - name: string    |
| - path: string    |
| - type: string    |
| - size: long      |
| - metadata: map<> |
+-------------------+
| + getters/setters |
+-------------------+

+-------------------+
|   Playlist        |
+-------------------+
| - name: string    |
| - files: vector<> |
+-------------------+
| + getters/setters |
| + add/removeFile  |
+-------------------+

Views:
-------
+-------------------+
|   CLIView         |
+-------------------+
| + displayFiles    |
| + displayPlaylists|
| + displayMetadata |
| + showMessage     |
+-------------------+

+-------------------+
|   DeviceView      |
+-------------------+
| + displayNowPlaying|
| + displayVolume   |
+-------------------+

Controllers:
------------
+------------------------+
|   FileController       |
+------------------------+
| + scanDirectory()      |
| + scanUSBDevice()      |
+------------------------+

+------------------------+
|   PlaybackController   |
+------------------------+
| - currentFileIndex     |
| - volume               |
| + play()               |
| + pause()              |
| + stop()               |
| + next()               |
| + previous()           |
+------------------------+

+------------------------+
|   MetadataController   |
+------------------------+
| + getMetadata()        |
| + updateMetadata()     |
+------------------------+

+------------------------+
|   PlaylistController   |
+------------------------+
| + createPlaylist()     |
| + updatePlaylist()     |
| + deletePlaylist()     |
+------------------------+

+------------------------+
|   DeviceController     |
+------------------------+
| + controlVolume()      |
| + controlPlayback()    |
+------------------------+
```

---

## 6. Implementation Steps

### Step 1: Scan Media Files

- Use `std::filesystem` for recursive file listing.
- Filter media files based on extensions (.mp3, .mp4).

### Step 2: Manage Playlists

- Store playlists in JSON files for persistence.
- Provide options for creating, updating, deleting playlists.

### Step 3: Metadata Management

- Use TagLib to extract and edit audio metadata.
- Implement similar logic for video files (e.g., using FFmpeg).

### Step 4: Playback Functionality

- Use SDL2 for audio playback.
- Run playback in a separate thread to ensure responsiveness.

### Step 5: S32K144 Hardware Integration

- Use UART for communication with the board.
- Implement ADC-based volume control and button handling.

---

## 7. Tools and Libraries

### Software

- **C++ Libraries:**
  - `std::filesystem`: File handling.
  - `TagLib`: Metadata extraction and editing.
  - `SDL2`: Audio playback.
- **Hardware Communication:**
  - UART for S32K144 communication.
- **Unit Testing:**
  - Google Test (gTest).

### Hardware

- S32K144 board.
- External USB storage device for testing media files.

---

## 8. Summary

This design document provides a comprehensive blueprint for developing the CLI Media Player Application. It covers architectural decisions, key components, and implementation details, ensuring efficient development and testing.

