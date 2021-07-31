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
//	Wii U Controller implementation
//
//	Loosely based on:
//		https://github.com/aboood40091/sm64ex/blob/master/src/pc/controller/controller_wiiu.c
//		https://github.com/devkitPro/SDL/blob/wiiu-sdl2-2.0.9/src/joystick/wiiu/SDL_wiiujoystick.c
//

#include "wiiu_controller.h"

#include <vpad/input.h>
#include <padscore/wpad.h>
#include <padscore/kpad.h>

// Controller state variables

uint32_t buttons;
int32_t stickX;
int32_t stickY;
int32_t rStickX;
int32_t rStickY;

void WiiU_InitJoystick()
{
    VPADInit();
    KPADInit();
    WPADEnableURCC(1);
    WPADEnableWiiRemote(1);
}

void WiiU_ShutdownJoystick()
{
    // Anything to do here?
}

static void read_vpad()
{
    VPADStatus status;
    VPADReadError err;

    VPADRead(VPAD_CHAN_0, &status, 1, &err);

    if (err != VPAD_READ_SUCCESS)
    {
        // heck
        return;
    }

    for (int i = 0; i < SIZEOF_ARR(vpad_button_map); i++)
    {
        if (status.hold & vpad_button_map[i])
            buttons |= (1 << i);
    }

    stickX = status.leftStick.x * 0x7ff0;
    stickY = -status.leftStick.y * 0x7ff0;

    rStickX = status.rightStick.x * 0x7ff0;
    rStickY = -status.rightStick.y * 0x7ff0;
}

static void read_wpad()
{
    // todo
}

void WiiU_PollJoystick()
{
    buttons = 0;
    stickX = 0;
    stickY = 0;
    rStickX = 0;
    rStickY = 0;

    read_vpad();
    read_wpad();
}

int WiiU_JoystickGetButton(int button)
{
    return !!(buttons & (1 << button));
}

int WiiU_JoystickGetAxis(int axis)
{
    switch (axis)
    {
        case 0:
            return stickX;
        case 1:
            return stickY;
        case 2:
            return rStickX;
        case 3:
            return rStickY;
        default:
            return 0;
    }
}
