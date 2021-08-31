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
//	Wii U Launcher - No WADs screen
//

#include "wiiu_launcher_nowads.h"

#ifdef __WIIU__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include <dirent.h>

#include "config.h"

#include "wiiu_launcher.h"
#include "wiiu_controller.h"

extern int launcherRunning;
extern launcherState state;

void launcherNoWadsInit()
{
    // NOP
}

void launcherNoWadsUpdate()
{
    if (WiiU_JoystickGetButtonPressed(10))	// Plus
        launcherRunning = -1; // Quit
}

void launcherNoWadsDraw(OSScreenID screenID)
{
    OSScreenPutFontEx(screenID, 0, 0, "Crispy Doom U");

    OSScreenPutFontEx(screenID, 5, 2, "No WAD files found!");
    OSScreenPutFontEx(screenID, 5, 3, "Put your WADs in: sd:/" HOMEBREW_APP_PATH "/wads");
    OSScreenPutFontEx(screenID, 0, 5, "Press + to exit");
}

void launcherNoWadsCleanup()
{
    // NOP
}

#endif // __WIIU__
