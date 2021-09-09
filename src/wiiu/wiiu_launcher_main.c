//
// Copyright(C) 2021 Terry Hearst
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
//
//	Wii U Launcher - Main screen
//

#include "wiiu_launcher_main.h"

#ifdef __WIIU__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "config.h"

#include "wiiu_launcher.h"
#include "wiiu_controller.h"

extern int launcherRunning;
extern launcherState state;

// Scanned WADs
char **foundWads = NULL;
int foundWadsCount = 0;
int menuHoverIndex = 0;

// Selected WADs
int *selectedWads = NULL;
int selectedWadsCount = 0;

void addWad(char *wadname)
{
    foundWadsCount++;
    foundWads = realloc(foundWads, foundWadsCount * sizeof(char *));
    // Insert alphabetically
    bool added = false;
    for (int i = 0; i < foundWadsCount - 1; i++)
    {
        if (strcmp(wadname, foundWads[i]) < 0)
        {
            // Shift all other entries down
            for (int j = foundWadsCount - 2; j >= i; j--)
            {
                foundWads[j + 1] = foundWads[j];
            }
            foundWads[i] = wadname;
            added = true;
            break;
        }
    }
    // If we made it all the way to the end and it still hasn't been added,
    // added, make it the last thing in the list
    if (!added)
    {
        foundWads[foundWadsCount - 1] = wadname;
    }
}

char *joinDirs(char *dirname, char *filename)
{
    size_t dirlen = strlen(dirname);
    if (dirlen == 0)
        return strdup(filename);
    // + 2 for slash and null terminator
    size_t joinedLen = dirlen + strlen(filename) + 2;
    char *joined = (char *) malloc(joinedLen * sizeof(char));
    strcpy(joined, dirname);
    joined[dirlen] = '/';
    strcpy(joined + dirlen + 1, filename);
    return joined;
}

void addWadsInDir(char *dirname, char *relDirname)
{
    struct dirent *files;
    DIR *dir = opendir(dirname);
    if (dir != NULL)
    {
        while ((files = readdir(dir)) != NULL)
        {
            char *filename = joinDirs(dirname, files->d_name);
            char *relFilename = joinDirs(relDirname, files->d_name);

            // Check if file or directory
            struct stat path_stat;
            stat(filename, &path_stat);
            if (S_ISREG(path_stat.st_mode))
            {
                addWad(relFilename);
            }
            else
            {
                if (S_ISDIR(path_stat.st_mode))
                {
                    addWadsInDir(filename, relFilename);
                }
                free(relFilename);
            }
            free(filename);
        }
        closedir(dir);
    }
}

void launcherMainInit()
{
    // Scan for WADs
    addWadsInDir(HOMEBREW_APP_PATH "/wads", "");

    if (foundWadsCount == 0)
    {
        state = LAUNCHER_NOWADS;
        return;
    }

    selectedWads = (int *) malloc(foundWadsCount * sizeof(int));
}

// Gets if this WAD is selected. If so, return the index (0 means IWAD)
// If not, return -1
int getSelectedIndex(int wadIndex)
{
    for (int i = 0; i < selectedWadsCount; i++)
    {
        if (selectedWads[i] == wadIndex)
            return i;
    }
    return -1;
}

// Selects or deselects the given WAD
void selectWad(int wadIndex)
{
    int selected = getSelectedIndex(wadIndex);
    if (selected >= 0)
    {
        selectedWadsCount--;
        for (int i = selected; i < selectedWadsCount; i++)
        {
            selectedWads[i] = selectedWads[i + 1];
        }
        return;
    }
    selectedWads[selectedWadsCount] = wadIndex;
    selectedWadsCount++;
}

void launcherMainUpdate()
{
    if (WiiU_JoystickGetButtonPressed(13) || WiiU_JoystickGetButtonPressed(17))	// Up
        menuHoverIndex--;
    if (WiiU_JoystickGetButtonPressed(15) || WiiU_JoystickGetButtonPressed(19))	// Down
        menuHoverIndex++;

    if (menuHoverIndex < 0)
        menuHoverIndex = foundWadsCount - 1;
    else if (menuHoverIndex >= foundWadsCount)
        menuHoverIndex = 0;

    if (WiiU_JoystickGetButtonPressed(0))	// A
        selectWad(menuHoverIndex);

    if (WiiU_JoystickGetButtonPressed(10) && (selectedWadsCount > 0))	// Plus
        launcherRunning = 0;
}

void launcherMainDraw(OSScreenID screenID)
{
    OSScreenPutFontEx(screenID, 0, 0, "Crispy Doom U");

    if (selectedWadsCount == 0)
    {
        OSScreenPutFontEx(screenID, 0, 2, "Press A to select an IWAD");
    }
    else
    {
        OSScreenPutFontEx(screenID, 0, 2, "Press + to start playing");
        OSScreenPutFontEx(screenID, 0, 3, "Optionally, press A to select/deselect additional PWADs");
    }

    // Handle scrolling if there are too many WADs to show on screen
    int startIndex = menuHoverIndex - (NUM_WADS_VISIBLE / 2);
    if (startIndex + NUM_WADS_VISIBLE > foundWadsCount)
        startIndex = foundWadsCount - NUM_WADS_VISIBLE;
    if (startIndex < 0)
        startIndex = 0;
    int endIndex = startIndex + NUM_WADS_VISIBLE;
    if (endIndex >= foundWadsCount)
        endIndex = foundWadsCount;

    // Draw scrollbars if needed
    if (startIndex != 0)
        OSScreenPutFontEx(screenID, 12, WADS_STARTING_Y - 1, "^ ^ ^ ^ ^ ^ ^");
    if (endIndex != foundWadsCount)
        OSScreenPutFontEx(screenID, 12, WADS_STARTING_Y + NUM_WADS_VISIBLE, "v v v v v v v");

    for (int i = startIndex; i < endIndex; i++)
    {
        int y = WADS_STARTING_Y + i - startIndex;
        OSScreenPutFontEx(screenID, 9, y, foundWads[i]);
        if (menuHoverIndex == i)
            OSScreenPutFontEx(screenID, 6, y, ">>");

        int selectedIndex = getSelectedIndex(i);
        if (selectedIndex == 0)
        {
            OSScreenPutFontEx(screenID, 1, y, "IWAD");
        }
        else if (selectedIndex > 0)
        {
            char buf[11];
            sprintf(buf, "%4d", selectedIndex);
            OSScreenPutFontEx(screenID, 1, y, buf);
        }
    }
}

void launcherMainCleanup()
{
    if (foundWadsCount > 0)
    {
        // Free found WADs
        for (int i = 0; i < foundWadsCount; i++)
        {
            free(foundWads[i]);
        }
        free(foundWads);
        free(selectedWads);
    }
}

#endif // __WIIU__
