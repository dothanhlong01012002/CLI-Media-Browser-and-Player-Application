#ifndef MEDIA_FILE_CONTROLLER_H
#define MEDIA_FILE_CONTROLLER_H

#include <vector>
#include <iostream>
#include "MediaFile.hpp"
#include "BaseController.hpp"
#include "memory"

class MediaFileController : public BaseController {
private:
    size_t currentPage = 1;
public:
    virtual void handleInput();

    void back();

    void displayMediaFilesWithPagination(const std::vector<std::shared_ptr<MediaFile>>& files, size_t pageSize = 25);
};

#endif // MEDIA_FILE_CONTROLLER_H
