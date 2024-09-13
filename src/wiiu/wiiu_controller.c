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

#include <string.h>

#include <vpad/input.h>
#include <padscore/wpad.h>
#include <padscore/kpad.h>

#define MAX(value1, value2) (value1 > value2 ? value1 : value2)
#define MIN(value1, value2) (value1 < value2 ? value1 : value2)
#define CLAMP(value, min, max) (MAX(MIN(value, max), min))

// Button maps, taken directly (with minor changes) from
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
    // Inverting these two seems more consistent to me... Though it goes against the labels. TODO: make configurable?
    WPAD_CLASSIC_BUTTON_ZL, WPAD_CLASSIC_BUTTON_ZR,
    WPAD_CLASSIC_BUTTON_L, WPAD_CLASSIC_BUTTON_R,
/*
    WPAD_CLASSIC_BUTTON_L, WPAD_CLASSIC_BUTTON_R,
    WPAD_CLASSIC_BUTTON_ZL, WPAD_CLASSIC_BUTTON_ZR,
*/
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

// Controller state variables

uint32_t buttons;
uint32_t buttonsPressed;
int32_t stickX;
int32_t stickY;
int32_t rStickX;
int32_t rStickY;
int32_t gyroX;
int32_t gyroY;


KPADStatus lastKpad[4] = {{0}, {0}, {0}, {0}};
int kpadTimeout[4] = {10, 10, 10, 10};

void WiiU_InitJoystick()
{
    VPADInit();
    VPADEnableGyroAccRevise(VPAD_CHAN_0);
    KPADInit();
    WPADEnableURCC(1);
    WPADEnableWiiRemote(1);
}

void WiiU_ShutdownJoystick()
{
    KPADShutdown();
    VPADShutdown();
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
        if (status.trigger & vpad_button_map[i])
            buttonsPressed |= (1 << i);
    }

    stickX += status.leftStick.x * 0x7ff0;
    stickY -= status.leftStick.y * 0x7ff0;

    rStickX += status.rightStick.x * 0x7ff0;
    rStickY -= status.rightStick.y * 0x7ff0;

    gyroX = -0x20000 * status.gyro.z;
    gyroY = 0x20000 * status.gyro.x;
}

static void read_wpad_chan(WPADChan chan)
{
    WPADExtensionType ext;
    int res = WPADProbe(chan, &ext);
    if (res != 0)
        return;

    KPADStatus status;
    int err;
    int read = KPADReadEx(chan, &status, 1, &err);
    if (read == 0)
    {
        kpadTimeout[chan]--;
        if (kpadTimeout <= 0)
        {
            WPADDisconnect(chan);
            memset(&lastKpad[chan], 0, sizeof(KPADStatus));
            return;
        }
        status = lastKpad[chan];
    }
    else
    {
        kpadTimeout[chan] = 10;
        lastKpad[chan] = status;
    }

    if (status.extensionType == WPAD_EXT_NUNCHUK || status.extensionType == WPAD_EXT_MPLUS_NUNCHUK)
    {
        // Probably won't be very fun...
        // TODO: find a mapping that will make this playable somehow??
        for (int i = 0; i < SIZEOF_ARR(wiimote_button_map); i++)
        {
            if (status.hold & wiimote_button_map[i])
                buttons |= (1 << i);
            if (status.trigger & wiimote_button_map[i])
                buttonsPressed |= (1 << i);
        }

        stickX += status.nunchuk.stick.x * 0x7ff0;
        stickY -= status.nunchuk.stick.y * 0x7ff0;
        // no rStick...
    }
    else if (status.extensionType == WPAD_EXT_CLASSIC || status.extensionType == WPAD_EXT_MPLUS_CLASSIC)
    {
        for (int i = 0; i < SIZEOF_ARR(classic_button_map); i++)
        {
            if (status.classic.hold & classic_button_map[i])
                buttons |= (1 << i);
            if (status.classic.trigger & classic_button_map[i])
                buttonsPressed |= (1 << i);
        }

        stickX += status.classic.leftStick.x * 0x7ff0;
        stickY -= status.classic.leftStick.y * 0x7ff0;

        rStickX += status.classic.rightStick.x * 0x7ff0;
        rStickY -= status.classic.rightStick.y * 0x7ff0;
    }
    else if (status.extensionType == WPAD_EXT_PRO_CONTROLLER)
    {
        for (int i = 0; i < SIZEOF_ARR(pro_button_map); i++)
        {
            if (status.pro.hold & pro_button_map[i])
                buttons |= (1 << i);
            if (status.pro.trigger & pro_button_map[i])
                buttonsPressed |= (1 << i);
        }

        stickX += status.pro.leftStick.x * 0x7ff0;
        stickY -= status.pro.leftStick.y * 0x7ff0;

        rStickX += status.pro.rightStick.x * 0x7ff0;
        rStickY -= status.pro.rightStick.y * 0x7ff0;
    }
}

static void read_wpad()
{
    read_wpad_chan(WPAD_CHAN_0);
    read_wpad_chan(WPAD_CHAN_1);
    read_wpad_chan(WPAD_CHAN_2);
    read_wpad_chan(WPAD_CHAN_3);
}

void WiiU_PollJoystick()
{
    buttons = 0;
    buttonsPressed = 0;
    stickX = 0;
    stickY = 0;
    rStickX = 0;
    rStickY = 0;
    gyroX = 0;
    gyroY = 0;

    read_vpad();
    read_wpad();

    // TODO: Properly implement gyro and make it mappable
    /*
    if (buttons & (1 << 8))
    {
        rStickX += gyroX;
        rStickY += gyroY;
    }
    */

    stickX = CLAMP(stickX, -0x7ff0, 0x7ff0);
    stickY = CLAMP(stickY, -0x7ff0, 0x7ff0);
    rStickX = CLAMP(rStickX, -0x7ff0, 0x7ff0);
    rStickY = CLAMP(rStickY, -0x7ff0, 0x7ff0);
}

int WiiU_JoystickGetButton(int button)
{
    return !!(buttons & (1 << button));
}

int WiiU_JoystickGetButtonPressed(int button)
{
    return !!(buttonsPressed & (1 << button));
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
