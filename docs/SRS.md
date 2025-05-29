
# 🎵 Media Player Application

---

## **Use Case 1: View Media Files**
**Description:**  
The user can browse media files (audio and video) in the current directory or external USB storage.

### **Features:**
1. **Select Directory/USB Device ([UC01](#uc01-browse-media-files), [UC02](#uc02-browse-usb-media-files)):**  
   The user selects the directory or USB device to scan for media files.

2. **List Media Files ([UC01](#uc01-browse-media-files), [UC02](#uc02-browse-usb-media-files)):**  
   The system lists all media files in the selected directory and its subdirectories.

3. **Pagination ([UC01](#uc01-browse-media-files), [UC02](#uc02-browse-usb-media-files)):**  
   The user navigates through the list of media files using pagination (**25 files per page**).

---

## **Use Case 2: Manage Playlists**
**Description:**  
The user can manage playlists to organize media files.

### **Features:**
- **List Playlists ([UC03](#uc03-list-playlists)):**  
  The system displays all existing playlists.

- **View Playlist ([UC04](#uc04-view-playlist)):**  
  The user selects a playlist, and the system displays the media files in it.

- **Create Playlist ([UC05](#uc05-create-playlist)):**  
  The user provides a name and adds media files to create a new playlist.

- **Update Playlist ([UC06](#uc06-update-playlist)):**  
  The user adds or removes media files from an existing playlist.

- **Delete Playlist ([UC07](#uc07-delete-playlist)):**  
  The user selects a playlist to delete.

---

## **Use Case 3: View and Edit Metadata**
**Description:**  
The user can view and modify metadata of media files.

### **Features:**
- **View Metadata ([UC08](#uc08-view-metadata)):**  
  The user selects a media file, and the system displays metadata such as:  
  - Track Name  
  - Album  
  - Artist  
  - Duration  
  - Genre  
  - ...and more.

- **Edit Metadata ([UC09](#uc09-edit-metadata)):**  
  The user modifies existing metadata fields or adds new ones.

---

## **Use Case 4: Playback Media**
**Description:**  
The user can play media files with basic playback functionalities.

### **Features:**
- **Play/Pause ([UC10](#uc10-play-music), [UC11](#uc11-control-playback)):**  
  The user starts or pauses playback of a selected media file.

- **Stop ([UC10](#uc10-play-music)):**  
  The user stops playback.

- **Next/Previous ([UC11](#uc11-control-playback)):**  
  The user navigates to the next or previous media file.

- **Automatic Transition ([UC10](#uc10-play-music)):**  
  The system automatically transitions to the next song when the current one finishes.

---

## **Use Case 5: Control Volume**
**Description:**  
The user adjusts the playback volume using both software and hardware.

### **Features:**

- **Hardware Volume Control ([UC13](#uc13-control-volume-via-s32k144)):**  
  The user adjusts the volume using the **ADC on the S32K144 board**.

---

## **Use Case 6: Display Information on S32K144**
**Description:**  
The system communicates with the **S32K144** board to display song information and control playback.

### **Features:**
- **Display Song Duration ([UC15](#uc15-display-song-information-on-s32k144)):**  
  When a song is playing, duration is displayed on the S32K144 screen.

- **Hardware Control ([UC14](#uc14-control-playback-via-s32k144-buttons)):**  
  The user uses buttons on the S32K144 board to:  
  - Play/Pause  
  - Stop  
  - Next  
  - Previous

---


### **UC01: Browse Media Files**
[Back to Features](#use-case-1-view-media-files)
*   **Use Case ID:** UC01  
*   **Title:** Browse Media Files  
*   **Goal:** To view a paginated list of audio and video files within a specified directory and its subdirectories.  
*   **Preconditions:** The application is running. The user has navigated to a directory within the application.  
*   **Postconditions:** The application displays a paginated list of media files.  

### **Basic Flow:**
1. The user enters a command to view media files (e.g., `lsmedia`).
2. The application scans the current directory and all subdirectories for audio and video files.
3. The application displays a list of found files, 25 files per page.
4. The user can navigate through the pages using commands like `next`, `prev`, or by entering a page number.
5. For each file, the list displays a unique index, the filename, and optionally a short summary (e.g., duration for audio).

### **Alternative Flows:**
*   **A1: No media files found:**  
    The application displays a message indicating that no media files were found in the specified directory.  
*   **A2: Invalid page number:**  
    The user enters an invalid page number. The application displays an error message and re-displays the current page.  

### **Error Conditions:**
*   **E1: Permission denied:**  
    The application encounters a "permission denied" error while accessing a directory. The error is logged, and the application continues scanning other accessible directories.  

---

### **UC02: Browse USB Media Files**
[Back to Features](#use-case-1-view-media-files)
*   **Use Case ID:** UC02  
*   **Title:** Browse USB Media Files  
*   **Goal:** To view a paginated list of audio and video files on a connected USB drive.  
*   **Preconditions:** The application is running. A USB drive is connected to the system.  
*   **Postconditions:** The application displays a paginated list of media files from the USB drive.  

### **Basic Flow:**
1. The user enters a command to browse USB media (e.g., `lsusb`).
2. The application detects and mounts the connected USB drive.
3. The application scans the root directory of the mounted USB drive and all its subdirectories for audio and video files.
4. The application displays a list of found files, 25 files per page.
5. The user can navigate through the pages using commands like `next`, `prev`, or by entering a page number.
6. For each file, the list displays a unique index, the filename, and optionally a short summary (e.g., duration for audio).
7. After browsing, the application automatically unmounts the USB drive.

### **Alternative Flows:**
*   **A1: No USB drive connected:**  
    The application displays a message indicating that no USB drive was detected.  
*   **A2: USB mounting failed:**  
    The application encounters an error while mounting the USB drive. The error is displayed to the user.  
*   **A3: No media files found on USB:**  
    The application successfully mounts the USB drive but finds no media files. The application displays a message indicating that no media files were found.  
*   **A4: Invalid page number:**  
    The user enters an invalid page number. The application displays an error message and re-displays the current page.  

### **Error Conditions:**
*   **E1: Permission denied (USB):**  
    The application encounters a "permission denied" error while accessing a directory on the USB drive. The error is logged, and the application continues scanning other accessible directories.  

---

### **UC03: List Playlists**
[Back to Features](#use-case-2-manage-playlists)
*   **Use Case ID:** UC03  
*   **Title:** List Playlists  
*   **Goal:** To view a list of available playlists.  
*   **Preconditions:** The application is running. Playlists have been created.  
*   **Postconditions:** The application displays a list of playlist names.  

### **Basic Flow:**
1. The user enters a command to list playlists (e.g., `playlists`).
2. The application reads and displays the names of all saved playlists.
3. Each playlist name is displayed with a unique index.

### **Alternative Flows:**
*   **A1: No playlists exist:**  
    The application displays a message indicating that no playlists have been created yet.  

### **Error Conditions:**
*   **E1: Playlist data corrupted:**  
    The application encounters an error while reading the playlist data. An error message is displayed.  

---

### **UC04: View Playlist**
[Back to Features](#use-case-2-manage-playlists)
*   **Use Case ID:** UC04  
*   **Title:** View Playlist  
*   **Goal:** To view the contents of a specific playlist.  
*   **Preconditions:** The application is running. Playlists exist.  
*   **Postconditions:** The application displays the media files within the selected playlist.  

### **Basic Flow:**
1. The user enters a command to view a playlist, specifying the playlist name or index (e.g., `viewpl <playlist_name>` or `viewpl 1`).
2. The application retrieves and displays the list of media files in the specified playlist.
3. Each file is displayed with a unique index within the playlist and its filename.

### **Alternative Flows:**
*   **A1: Playlist not found:**  
    The user specifies a playlist that does not exist. The application displays an error message.  

### **Error Conditions:**
*   **E1: Playlist data corrupted:**  
    The application encounters an error while reading the specified playlist data. An error message is displayed.  

---
### **UC05: Create Playlist**
[Back to Features](#use-case-2-manage-playlists)
*   **Use Case ID:** UC05  
*   **Title:** Create Playlist  
*   **Goal:** To create a new playlist and add media files to it.  
*   **Preconditions:** The application is running. The user has a list of media files (either from browsing or knowing their paths).  
*   **Postconditions:** A new playlist is created with the specified name and media files.  

### **Basic Flow:**
1. The user enters a command to create a playlist, specifying the new playlist name (e.g., `createpl <new_playlist_name>`).
2. The application prompts the user to add media files to the playlist.
3. The user can add files by specifying their index from a previous media list or by providing the full path to the file.
4. The user enters a command to finish adding files.
5. The application saves the new playlist.

### **Alternative Flows:**
*   **A1: Playlist name already exists:**  
    The user attempts to create a playlist with a name that already exists. The application prompts the user to choose a different name or update the existing playlist.  
*   **A2: Invalid file path:**  
    The user provides an invalid file path. The application displays an error message and prompts for a valid file path.  

### **Error Conditions:**
*   **E1: Error saving playlist:**  
    The application encounters an error while saving the new playlist. An error message is displayed.  

---

### **UC06: Update Playlist**
[Back to Features](#use-case-2-manage-playlists)
*   **Use Case ID:** UC06  
*   **Title:** Update Playlist  
*   **Goal:** To modify an existing playlist by adding or removing media files.  
*   **Preconditions:** The application is running. Playlists exist.  
*   **Postconditions:** The specified playlist is updated with the added or removed media files.  

### **Basic Flow:**
1. The user enters a command to update a playlist, specifying the playlist name or index (e.g., `updatepl <playlist_name>`).
2. The application presents options to add or remove files.
3. **If adding:** The user can add files by specifying their index from a previous media list or by providing the full path to the file.
4. **If removing:** The user can remove files by specifying their index within the playlist.
5. The user enters a command to finish updating.
6. The application saves the updated playlist.

### **Alternative Flows:**
*   **A1: Playlist not found:**  
    The user specifies a playlist that does not exist. The application displays an error message.  
*   **A2: Invalid file path:**  
    The user provides an invalid file path when adding. The application displays an error message.  
*   **A3: Invalid playlist item index:**  
    The user provides an invalid index when removing. The application displays an error message.  

### **Error Conditions:**
*   **E1: Error saving playlist:**  
    The application encounters an error while saving the updated playlist. An error message is displayed.  

---

### **UC07: Delete Playlist**
[Back to Features](#use-case-2-manage-playlists)
*   **Use Case ID:** UC07  
*   **Title:** Delete Playlist  
*   **Goal:** To delete an existing playlist.  
*   **Preconditions:** The application is running. Playlists exist.  
*   **Postconditions:** The specified playlist is permanently deleted.  

### **Basic Flow:**
1. The user enters a command to delete a playlist, specifying the playlist name or index (e.g., `deletepl <playlist_name>`).
2. The application prompts the user to confirm the deletion.
3. Upon confirmation, the application deletes the playlist.

### **Alternative Flows:**
*   **A1: Playlist not found:**  
    The user specifies a playlist that does not exist. The application displays an error message.  
*   **A2: Deletion cancelled:**  
    The user chooses not to confirm the deletion. The playlist is not deleted.  

### **Error Conditions:**
*   **E1: Error deleting playlist:**  
    The application encounters an error while deleting the playlist data. An error message is displayed.  

---

### **UC08: View Metadata**
[Back to Features](#use-case-3-view-and-edit-metadata)
*   **Use Case ID:** UC08  
*   **Title:** View Metadata  
*   **Goal:** To view the metadata of a specific media file.  
*   **Preconditions:** The application is running. The user has selected a media file (either from a media list or a playlist).  
*   **Postconditions:** The application displays the metadata of the selected file.  

### **Basic Flow:**
1. The user enters a command to view metadata, specifying the file's index from a media list or playlist, or provides the full file path (e.g., `metadata <file_index>` or `metadata /path/to/file.mp3`).
2. The application reads the metadata of the specified file using Taglib.
3. The application displays the available metadata in a user-friendly format, including:  
    * **Audio:** Track name, Album, Artist, Duration, Genre, Publisher, Publish Year, etc.  
    * **Video:** Name, Size, Duration, Bitrate, Codec, etc.  

### **Alternative Flows:**
*   **A1: File not found:**  
    The specified file index or path is invalid. The application displays an error message.  
*   **A2: Unsupported file format:**  
    The application does not support the file format for metadata extraction. The application displays a message indicating the format is not supported.  
*   **A3: No metadata available:**  
    The file contains no metadata. The application displays a message indicating no metadata was found.  

### **Error Conditions:**
*   **E1: Error reading metadata:**  
    Taglib encounters an error while reading the file's metadata. An error message is displayed.  

---

### **UC09: Edit Metadata**
[Back to Features](#use-case-3-view-and-edit-metadata)
*   **Use Case ID:** UC09  
*   **Title:** Edit Metadata  
*   **Goal:** To modify the metadata of a specific media file.  
*   **Preconditions:** The application is running. The user has selected a media file (either from a media list or a playlist).  
*   **Postconditions:** The metadata of the selected file is updated.  

### **Basic Flow:**
1. The user enters a command to edit metadata, specifying the file's index or path (e.g., `editmeta <file_index>` or `editmeta /path/to/file.mp3`).
2. The application displays the current metadata of the file.
3. The application presents options to:  
    * Change the value of an existing metadata tag.  
    * Add a new metadata tag with a value.  
4. The user enters the tag name and the new value.
5. The application updates the metadata using Taglib.
6. The application confirms the changes to the user.

### **Alternative Flows:**
*   **A1: File not found:**  
    The specified file index or path is invalid. The application displays an error message.  
*   **A2: Unsupported file format:**  
    The application does not support metadata editing for the file format. The application displays a message indicating the format is not supported for editing.  
*   **A3: Invalid tag name:**  
    The user enters an invalid metadata tag name. The application displays an error message.  
*   **A4: Editing cancelled:**  
    The user chooses to cancel the metadata editing process.  

### **Error Conditions:**
*   **E1: Error writing metadata:**  
    Taglib encounters an error while writing the modified metadata to the file. An error message is displayed.  

---

### **UC10: Play Music**
[Back to Features](#use-case-4-playback-media)
*   **Use Case ID:** UC10  
*   **Title:** Play Music  
*   **Goal:** To play an audio file.  
*   **Preconditions:** The application is running. The user has selected an audio file (either from a media list or a playlist). SDL2 is initialized.  
*   **Postconditions:** The selected audio file starts playing. The application displays playback information.  

### **Basic Flow:**
1. The user enters a command to play music, specifying the file's index or path (e.g., `play <file_index>` or `play /path/to/audio.mp3`).
2. The application loads the audio file using SDL2 in a separate thread.
3. The audio playback begins.
4. The application displays the current time and the total duration of the song.

### **Alternative Flows:**
*   **A1: File not found:**  
    The specified file index or path is invalid. The application displays an error message.  
*   **A2: Unsupported audio format:**  
    SDL2 does not support the audio format. The application displays a message indicating the format is not supported.  
*   **A3: Audio playback failed:**  
    SDL2 encounters an error while playing the audio. An error message is displayed.  

### **Error Conditions:**
*   **E1: SDL2 initialization error:**  
    SDL2 failed to initialize properly. The application cannot play audio.  

---

### **UC11: Control Playback**
[Back to Features](#use-case-4-playback-media)
*   **Use Case ID:** UC11  
*   **Title:** Control Playback  
*   **Goal:** To control the playback of the currently playing audio file.  
*   **Preconditions:** Music is currently playing.  
*   **Postconditions:** The playback state is changed according to the user's command.  

### **Basic Flow:**
1. The user enters commands to:  
    * **Pause:** Pause the currently playing song (e.g., `pause`).  
    * **Play:** Resume playback of a paused song (e.g., `play`).  
    * **Next:** Move to the next song in the current playlist (if applicable) or the next song in the current directory listing (e.g., `next`).  
    * **Previous:** Move to the previous song in the current playlist or directory listing (e.g., `prev`).  
2. The application performs the requested action on the audio playback.
3. The application updates the displayed playback information (e.g., current time).

### **Alternative Flows:**
*   **A1: No music playing:**  
    The user attempts to control playback when no music is playing. The application displays a message indicating that no music is currently playing.  
*   **A2: End of playlist/directory:**  
    When using "Next" at the end of a playlist or directory, the application may either stop or loop back to the beginning (implementation choice). Similarly for "Previous" at the beginning.  

### **Error Conditions:**
*   **E1: SDL2 playback error:**  
    SDL2 encounters an error while attempting to control playback. An error message is displayed.  

---

### **UC12: Change Volume**
[Back to Features](#use-case-5-control-volume)
*   **Use Case ID:** UC12  
*   **Title:** Change Volume  
*   **Goal:** To adjust the volume of the audio output.  
*   **Preconditions:** The application is running. Audio playback is possible.  
*   **Postconditions:** The system volume is adjusted.  

### **Basic Flow:**
1. The user enters a command to change the volume, specifying the desired volume level (e.g., `volume up` or `volume 50`).
2. The application adjusts the system volume using SDL2's audio control functions.
3. The application displays the new volume level.

### **Alternative Flows:**
*   **A1: Invalid volume level:**  
    The user enters an invalid volume level (e.g., out of range). The application displays an error message.  

### **Error Conditions:**
*   **E1: SDL2 volume control error:**  
    SDL2 encounters an error while attempting to change the volume. An error message is displayed.  

---

### **UC13: Control Volume via S32K144** 
[Back to Features](#use-case-5-control-volume)
*   **Use Case ID:** UC13  
*   **Title:** Control Volume via S32K144  
*   **Primary Actor:** User (interacting with the S32K144 board).  
*   **Goal:** To control the audio volume using the ADC input on the connected S32K144 board.  
*   **Preconditions:** The application is running. The S32K144 board is connected and communicating with the application. Audio is playing or capable of playing.  
*   **Postconditions:** The audio volume is adjusted based on the ADC value from the S32K144.  

### **Basic Flow:**
1. The S32K144 board reads the value from its ADC.
2. The S32K144 sends the ADC value to the CLI application via a defined communication protocol.
3. The CLI application receives the ADC value.
4. The CLI application maps the ADC value to a volume level.
5. The CLI application adjusts the system volume using SDL2 based on the mapped value.

### **Alternative Flows:**
*   **A1: Communication error with S32K144:**  
    The CLI application fails to receive data from the S32K144. The application may retry or indicate an error.  

### **Error Conditions:**
*   **E1: SDL2 volume control error:**  
    SDL2 encounters an error while attempting to change the volume.  

---

### **UC14: Control Playback via S32K144 Buttons**
[Back to Features](#use-case-6-display-information-on-s32k144)
*   **Use Case ID:** UC14  
*   **Title:** Control Playback via S32K144 Buttons  
*   **Primary Actor:** User (interacting with the S32K144 board).  
*   **Goal:** To control playback (Play/Pause, Stop, Next, Previous) using buttons on the connected S32K144 board.  
*   **Preconditions:** The application is running. The S32K144 board is connected and communicating with the application. An audio file is loaded or playing.  
*   **Postconditions:** Playback is controlled based on the button press on the S32K144.  

### **Basic Flow:**
1. The user presses a button on the S32K144 board (Play/Pause, Stop, Next, Previous).
2. The S32K144 sends a signal indicating the button press to the CLI application via the communication protocol.
3. The CLI application receives the signal.
4. The CLI application performs the corresponding playback action (Play/Pause, Stop, Next, Previous).

### **Alternative Flows:**
*   **A1: Communication error with S32K144:**  
    The CLI application fails to receive data from the S32K144. The button press is ignored.  
*   **A2: No music loaded:**  
    If Play is pressed and no music is loaded, the application might do nothing or load the first song in the current context.  

### **Error Conditions:**
*   **E1: SDL2 playback error:**  
    SDL2 encounters an error while attempting to control playback.  

---

### **UC15: Display Song Information on S32K144**
[Back to Features](#use-case-6-display-information-on-s32k144)
*   **Use Case ID:** UC15  
*   **Title:** Display Song Information on S32K144  
*   **Primary Actor:** System  
*   **Goal:** To display the currently playing song's information on the screen of the connected S32K144 board.  
*   **Preconditions:** The application is running. The S32K144 board is connected and communicating with the application. An audio file is playing.  
*   **Postconditions:** The song information is displayed on the S32K144 board's screen.  

### **Basic Flow:**
1. When an audio file starts playing, the CLI application retrieves relevant information (e.g., track name, artist).
2. The CLI application sends this information to the S32K144 board via the communication protocol.
3. The S32K144 board receives the information and displays it on its screen.
4. The display is updated if the song changes.

### **Alternative Flows:**
*   **A1: Communication error with S32K144:**  
    The CLI application fails to send data to the S32K144. No information is displayed on the board.  
*   **A2: S32K144 display error:**  
    The S32K144 encounters an error while displaying the information.  

### **Error Conditions:**
*   None explicitly defined at this stage.  

---