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
//	Wii U Controller header
//

#include <vpad/input.h>
#include <padscore/wpad.h>

void WiiU_InitJoystick();
void WiiU_ShutdownJoystick();

void WiiU_PollJoystick();
int WiiU_JoystickGetButton(int button);
int WiiU_JoystickGetButtonPressed(int button);
int WiiU_JoystickGetAxis(int axis);
