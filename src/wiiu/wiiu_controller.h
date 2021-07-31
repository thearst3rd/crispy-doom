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

// Button maps, taken directly from
// https://github.com/devkitPro/SDL/blob/wiiu-sdl2-2.0.9/src/joystick/wiiu/SDL_wiiujoystick.h

#define SIZEOF_ARR(arr) (sizeof(arr) / sizeof(arr[0]))

static VPADButtons vpad_button_map[] =
{
    VPAD_BUTTON_A, VPAD_BUTTON_B, VPAD_BUTTON_X, VPAD_BUTTON_Y,
    VPAD_BUTTON_STICK_L, VPAD_BUTTON_STICK_R,
    VPAD_BUTTON_L, VPAD_BUTTON_R,
    VPAD_BUTTON_ZL, VPAD_BUTTON_ZR,
    VPAD_BUTTON_PLUS, VPAD_BUTTON_MINUS,
    VPAD_BUTTON_LEFT, VPAD_BUTTON_UP, VPAD_BUTTON_RIGHT, VPAD_BUTTON_DOWN,
    VPAD_STICK_L_EMULATION_LEFT, VPAD_STICK_L_EMULATION_UP, VPAD_STICK_L_EMULATION_RIGHT, VPAD_STICK_L_EMULATION_DOWN,
    VPAD_STICK_R_EMULATION_LEFT, VPAD_STICK_R_EMULATION_UP, VPAD_STICK_R_EMULATION_RIGHT, VPAD_STICK_R_EMULATION_DOWN
};

static WPADButton wiimote_button_map[] = {
    WPAD_BUTTON_A, WPAD_BUTTON_B, WPAD_BUTTON_Z, WPAD_BUTTON_C,
    0, 0,
    WPAD_BUTTON_1, WPAD_BUTTON_2,
    0, 0,
    WPAD_BUTTON_PLUS, WPAD_BUTTON_MINUS,
    WPAD_BUTTON_LEFT, WPAD_BUTTON_UP, WPAD_BUTTON_RIGHT, WPAD_BUTTON_DOWN,
};

static WPADClassicButton classic_button_map[] = {
    WPAD_CLASSIC_BUTTON_A, WPAD_CLASSIC_BUTTON_B, WPAD_CLASSIC_BUTTON_X,
    WPAD_CLASSIC_BUTTON_Y,
    0, 0,
    WPAD_CLASSIC_BUTTON_L, WPAD_CLASSIC_BUTTON_R,
    WPAD_CLASSIC_BUTTON_ZL, WPAD_CLASSIC_BUTTON_ZR,
    WPAD_CLASSIC_BUTTON_PLUS, WPAD_CLASSIC_BUTTON_MINUS,
    WPAD_CLASSIC_BUTTON_LEFT, WPAD_CLASSIC_BUTTON_UP, WPAD_CLASSIC_BUTTON_RIGHT,
    WPAD_CLASSIC_BUTTON_DOWN,
};

static WPADProButton pro_button_map[] = {
    WPAD_PRO_BUTTON_A, WPAD_PRO_BUTTON_B, WPAD_PRO_BUTTON_X, WPAD_PRO_BUTTON_Y,
    WPAD_PRO_BUTTON_STICK_L, WPAD_PRO_BUTTON_STICK_R,
    WPAD_PRO_TRIGGER_L, WPAD_PRO_TRIGGER_R,
    WPAD_PRO_TRIGGER_ZL, WPAD_PRO_TRIGGER_ZR,
    WPAD_PRO_BUTTON_PLUS, WPAD_PRO_BUTTON_MINUS,
    WPAD_PRO_BUTTON_LEFT, WPAD_PRO_BUTTON_UP,
    WPAD_PRO_BUTTON_RIGHT, WPAD_PRO_BUTTON_DOWN,
    WPAD_PRO_STICK_L_EMULATION_LEFT, WPAD_PRO_STICK_L_EMULATION_UP,
    WPAD_PRO_STICK_L_EMULATION_RIGHT, WPAD_PRO_STICK_L_EMULATION_DOWN,
    WPAD_PRO_STICK_R_EMULATION_LEFT, WPAD_PRO_STICK_R_EMULATION_UP,
    WPAD_PRO_STICK_R_EMULATION_RIGHT, WPAD_PRO_STICK_R_EMULATION_DOWN
};

void WiiU_InitJoystick();
void WiiU_ShutdownJoystick();

void WiiU_PollJoystick();
int WiiU_JoystickGetButton(int button);
int WiiU_JoystickGetAxis(int axis);
