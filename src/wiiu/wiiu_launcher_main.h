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

#ifdef __WIIU__

#include <vpad/input.h>
#include <coreinit/screen.h>

#define WADS_STARTING_Y 5
#define NUM_WADS_VISIBLE 12

void launcherMainInit();
void launcherMainUpdate();
void launcherMainDraw(OSScreenID screenID);
void launcherMainCleanup();

#endif // __WIIU__
