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
//	Wii U Launcher/WAD picker implementation
//

#ifdef __WIIU__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include <dirent.h>

#include <vpad/input.h>
#include <coreinit/screen.h>
#include <coreinit/cache.h>
#include <whb/proc.h>

#include "wiiu-config.h"
#include "i_system.h"
#include "m_argv.h"

// Global variables
bool launcherRunning = true;

char **foundWads = NULL;
int foundWadsCount = 0;
int selectedWadIndex = 0;

void launcherUpdate(VPADStatus status)
{
    if (status.trigger & VPAD_BUTTON_PLUS)
        launcherRunning = false;

    if (foundWadsCount > 0)
    {
        if (status.trigger & (VPAD_BUTTON_UP | VPAD_STICK_L_EMULATION_UP))
            selectedWadIndex--;
        if (status.trigger & (VPAD_BUTTON_DOWN | VPAD_STICK_L_EMULATION_DOWN))
            selectedWadIndex++;

        if (selectedWadIndex < 0)
            selectedWadIndex = foundWadsCount - 1;
        else if (selectedWadIndex >= foundWadsCount)
            selectedWadIndex = 0;
    }
}

void launcherDraw(OSScreenID screenID)
{
    OSScreenPutFontEx(screenID, 0, 0, "Crispy Doom");

    if (foundWadsCount > 0)
    {
        OSScreenPutFontEx(screenID, 0, 2, "Press up and down to select a WAD");
        OSScreenPutFontEx(screenID, 0, 3, "Press + to start playing");
        for (int i = 0; i < foundWadsCount; i++)
        {
            int y = i + 5;
            OSScreenPutFontEx(screenID, 5, y, foundWads[i]);
            if (selectedWadIndex == i)
                OSScreenPutFontEx(screenID, 2, y, ">>");
        }
    }
    else
    {
        OSScreenPutFontEx(screenID, 5, 2, "No WAD files found!");
        OSScreenPutFontEx(screenID, 5, 3, "Put your WADs in: sd:/" HOMEBREW_APP_PATH "/wads");
        OSScreenPutFontEx(screenID, 0, 5, "Press + to exit");
    }
}

void launcherRun()
{
    // Init launcher
    OSScreenInit();

    size_t tvBufferSize = OSScreenGetBufferSizeEx(SCREEN_TV);
    size_t drcBufferSize = OSScreenGetBufferSizeEx(SCREEN_DRC);

    void *tvBuffer = memalign(0x100, tvBufferSize);
    void *drcBuffer = memalign(0x100, drcBufferSize);

    if (!tvBuffer || !drcBuffer)
    {
        OSScreenShutdown();
        if (tvBuffer)
            free(tvBuffer);
        if (drcBuffer) // shouldn't happen?
            free(drcBuffer);
        I_Error("Error starting launcher, couldn't create OSScreen Buffers");
    }

    OSScreenSetBufferEx(SCREEN_TV, tvBuffer);
    OSScreenSetBufferEx(SCREEN_DRC, drcBuffer);
    OSScreenEnableEx(SCREEN_TV, true);
    OSScreenEnableEx(SCREEN_DRC, true);

    VPADStatus status;
    VPADReadError error;

    // Scan for WADs
    foundWads = NULL;
    foundWadsCount = 0;
    struct dirent *files;
    DIR *dir = opendir(HOMEBREW_APP_PATH "/wads");
    if (dir != NULL)
    {
        while ((files = readdir(dir)) != NULL)
        {
            foundWadsCount++;
            foundWads = realloc(foundWads, foundWadsCount * sizeof(char *));
            foundWads[foundWadsCount - 1] = strdup(files->d_name);
        }
        closedir(dir);
    }

    // I need this variable because with out it, WHBProcIsRunning becomes true
    // again before exiting, causing a crash
    int wbhRunning = true;

    while (launcherRunning && (wbhRunning = WHBProcIsRunning()))
    {
        // Poll input
        VPADRead(VPAD_CHAN_0, &status, 1, &error);
        switch(error)
        {
            case VPAD_READ_SUCCESS:
                break;

            case VPAD_READ_NO_SAMPLES:
                continue;

            case VPAD_READ_INVALID_CONTROLLER:
                status = (VPADStatus) {0};
                break;

            default:
                I_Error("Unknown VPAD status error: %d", error);
        }

        launcherUpdate(status);

        // Draw screen
        OSScreenClearBufferEx(SCREEN_TV, 0x00000000);
        OSScreenClearBufferEx(SCREEN_DRC, 0x00000000);

        launcherDraw(SCREEN_TV);
        launcherDraw(SCREEN_DRC);

        DCFlushRange(tvBuffer, tvBufferSize);
        DCFlushRange(drcBuffer, drcBufferSize);

        OSScreenFlipBuffersEx(SCREEN_TV);
        OSScreenFlipBuffersEx(SCREEN_DRC);
    }

    // Cleanup launcher
    if (tvBuffer)
        free(tvBuffer);
    if (drcBuffer)
        free(drcBuffer);
    OSScreenShutdown();

    if (foundWadsCount > 0)
    {
        if (wbhRunning)
        {
            // Build up "myargc" and "myargv" from stuff user selected
            char *origArgv0 = myargv[0];

            myargc = 3;
            myargv = malloc(myargc * sizeof(char *));

            myargv[0] = origArgv0;
            myargv[1] = "-iwad";
            myargv[2] = strdup(foundWads[selectedWadIndex]);
        }

        // Free found WADs
        for (int i = 0; i < foundWadsCount; i++)
        {
            free(foundWads[i]);
        }
        free(foundWads);
    }

    if (!wbhRunning || foundWadsCount == 0)
    {
        WHBProcShutdown();
        exit(0);
    }
}

#endif // __WIIU__
