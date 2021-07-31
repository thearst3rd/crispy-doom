//
// Copyright(C) 2005-2014 Simon Howard
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
// DESCRIPTION:
//       SDL Joystick code.
//


#ifndef __WIIU__
#include "SDL.h"
#include "SDL_joystick.h"
#endif // !__WIIU__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "doomtype.h"
#include "d_event.h"
#include "i_joystick.h"
#include "i_system.h"

#include "m_config.h"
#include "m_misc.h"

#ifdef __WIIU__
#include "wiiu_controller.h"
#endif // __WIIU__

// When an axis is within the dead zone, it is set to zero.
// This is 5% of the full range:

#ifdef __WIIU__
#define DEAD_ZONE (32768 / 10)
#else
#define DEAD_ZONE (32768 / 3)
#endif // __WIIU__

#ifndef __WIIU__
static SDL_Joystick *joystick = NULL;
#endif // !__WIIU__

// Configuration variables:

// Standard default.cfg Joystick enable/disable

#ifndef __WIIU__
static int usejoystick = 0;

// SDL GUID and index of the joystick to use.
static char *joystick_guid = "";
static int joystick_index = -1;
#endif // !__WIIU__

// Which joystick axis to use for horizontal movement, and whether to
// invert the direction:

#ifdef __WIIU__
static int joystick_x_axis = 2; // Right stick x axis
#else
static int joystick_x_axis = 0;
#endif // __WIIU__
static int joystick_x_invert = 0;

// Which joystick axis to use for vertical movement, and whether to
// invert the direction:

static int joystick_y_axis = 1;
static int joystick_y_invert = 0;

// Which joystick axis to use for strafing?

#ifdef __WIIU__
static int joystick_strafe_axis = 0; // Left stick x axis
#else
static int joystick_strafe_axis = -1;
#endif // __WIIU__
static int joystick_strafe_invert = 0;

// Which joystick axis to use for looking?

#ifdef __WIIU__
static int joystick_look_axis = 3; // Right stick y axis
#else
static int joystick_look_axis = -1;
#endif // __WIIU__
static int joystick_look_invert = 0;

// Virtual to physical button joystick button mapping. By default this
// is a straight mapping.
#ifdef __WIIU__
static int joystick_physical_buttons[NUM_VIRTUAL_BUTTONS] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23
};
#else
static int joystick_physical_buttons[NUM_VIRTUAL_BUTTONS] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
};
#endif // __WIIU__

#ifdef BETTER_JOYWAIT
event_t currentJoystick = {0};
extern event_t prevJoystick;
extern unsigned int joywait;
extern unsigned int joywaitDiff;
#endif // BETTER_JOYWAIT

void I_ShutdownJoystick(void)
{
#ifdef __WIIU__
    WiiU_ShutdownJoystick();
#else
    if (joystick != NULL)
    {
        SDL_JoystickClose(joystick);
        joystick = NULL;
        SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
    }
#endif // __WIIU__
}

static boolean IsValidAxis(int axis)
{
    int num_axes;

    if (axis < 0)
    {
        return true;
    }

    if (IS_BUTTON_AXIS(axis))
    {
        return true;
    }

    if (IS_HAT_AXIS(axis))
    {
#ifdef __WIIU__
        return HAT_AXIS_HAT(axis) < 0; // same as comment below
#else
        return HAT_AXIS_HAT(axis) < SDL_JoystickNumHats(joystick);
#endif // __WIIU__
    }

#ifdef __WIIU__
    num_axes = 0; // TODO: check if this really should be 0. Looks like it, based on the wiiu SDL2 code
#else
    num_axes = SDL_JoystickNumAxes(joystick);
#endif // __WIIU__

    return axis < num_axes;
}

#ifndef __WIIU__
static int DeviceIndex(void)
{
    SDL_JoystickGUID guid, dev_guid;
    int i;

    guid = SDL_JoystickGetGUIDFromString(joystick_guid);

    // GUID identifies a class of device rather than a specific device.
    // Check if joystick_index has the expected GUID, as this can act
    // as a tie-breaker in case there are multiple identical devices.
    if (joystick_index >= 0 && joystick_index < SDL_NumJoysticks())
    {
        dev_guid = SDL_JoystickGetDeviceGUID(joystick_index);
        if (!memcmp(&guid, &dev_guid, sizeof(SDL_JoystickGUID)))
        {
            return joystick_index;
        }
    }

    // Check all devices to look for one with the expected GUID.
    for (i = 0; i < SDL_NumJoysticks(); ++i)
    {
        dev_guid = SDL_JoystickGetDeviceGUID(i);
        if (!memcmp(&guid, &dev_guid, sizeof(SDL_JoystickGUID)))
        {
            printf("I_InitJoystick: Joystick moved to index %d.\n", i);
            return i;
        }
    }

    // No joystick found with the expected GUID.
    return -1;
}
#endif // !__WIIU__

void I_InitJoystick(void)
{
#ifdef __WIIU__
    // We're initing our joysticks in the launcher, do nothing here
#else
    int index;

    if (!usejoystick || !strcmp(joystick_guid, ""))
    {
        return;
    }

    if (SDL_Init(SDL_INIT_JOYSTICK) < 0)
    {
        return;
    }

    index = DeviceIndex();

    if (index < 0)
    {
        printf("I_InitJoystick: Couldn't find joystick with GUID \"%s\": "
               "device not found or not connected?\n",
               joystick_guid);
        SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
        return;
    }

    // Open the joystick

    joystick = SDL_JoystickOpen(index);

    if (joystick == NULL)
    {
        printf("I_InitJoystick: Failed to open joystick #%i\n", index);
        SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
        return;
    }

    if (!IsValidAxis(joystick_x_axis)
     || !IsValidAxis(joystick_y_axis)
     || !IsValidAxis(joystick_strafe_axis)
     || !IsValidAxis(joystick_look_axis))
    {
        printf("I_InitJoystick: Invalid joystick axis for configured joystick "
               "(run joystick setup again)\n");

        SDL_JoystickClose(joystick);
        joystick = NULL;
        SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
    }

    SDL_JoystickEventState(SDL_ENABLE);

    // Initialized okay!

    printf("I_InitJoystick: %s\n", SDL_JoystickName(joystick));
#endif // __WIIU__

    I_AtExit(I_ShutdownJoystick, true);
}

static boolean IsAxisButton(int physbutton)
{
    if (IS_BUTTON_AXIS(joystick_x_axis))
    {
        if (physbutton == BUTTON_AXIS_NEG(joystick_x_axis)
         || physbutton == BUTTON_AXIS_POS(joystick_x_axis))
        {
            return true;
        }
    }
    if (IS_BUTTON_AXIS(joystick_y_axis))
    {
        if (physbutton == BUTTON_AXIS_NEG(joystick_y_axis)
         || physbutton == BUTTON_AXIS_POS(joystick_y_axis))
        {
            return true;
        }
    }
    if (IS_BUTTON_AXIS(joystick_strafe_axis))
    {
        if (physbutton == BUTTON_AXIS_NEG(joystick_strafe_axis)
         || physbutton == BUTTON_AXIS_POS(joystick_strafe_axis))
        {
            return true;
        }
    }
    if (IS_BUTTON_AXIS(joystick_look_axis))
    {
        if (physbutton == BUTTON_AXIS_NEG(joystick_look_axis)
         || physbutton == BUTTON_AXIS_POS(joystick_look_axis))
        {
            return true;
        }
    }

    return false;
}

// Get the state of the given virtual button.

static int ReadButtonState(int vbutton)
{
    int physbutton;

    // Map from virtual button to physical (SDL) button.
    if (vbutton < NUM_VIRTUAL_BUTTONS)
    {
        physbutton = joystick_physical_buttons[vbutton];
    }
    else
    {
        physbutton = vbutton;
    }

    // Never read axis buttons as buttons.
    if (IsAxisButton(physbutton))
    {
        return 0;
    }

#ifdef __WIIU__
    return WiiU_JoystickGetButton(physbutton);
#else
    return SDL_JoystickGetButton(joystick, physbutton);
#endif // __WIIU__
}

// Get a bitmask of all currently-pressed buttons

static int GetButtonsState(void)
{
    int i;
    int result;

    result = 0;

    for (i = 0; i < 20; ++i)
    {
        if (ReadButtonState(i))
        {
            result |= 1 << i;
        }
    }

    return result;
}

// Read the state of an axis, inverting if necessary.

static int GetAxisState(int axis, int invert)
{
    int result;

    // Axis -1 means disabled.

    if (axis < 0)
    {
        return 0;
    }

    // Is this a button axis, or a hat axis?
    // If so, we need to handle it specially.

    result = 0;

    if (IS_BUTTON_AXIS(axis))
    {
#ifdef __WIIU__
        if (WiiU_JoystickGetButton(BUTTON_AXIS_NEG(axis)))
        {
            result -= 32767;
        }
        if (WiiU_JoystickGetButton(BUTTON_AXIS_POS(axis)))
        {
            result += 32767;
        }
#else
        if (SDL_JoystickGetButton(joystick, BUTTON_AXIS_NEG(axis)))
        {
            result -= 32767;
        }
        if (SDL_JoystickGetButton(joystick, BUTTON_AXIS_POS(axis)))
        {
            result += 32767;
        }
#endif __WIIU__
    }
    else if (IS_HAT_AXIS(axis))
    {
        int direction = HAT_AXIS_DIRECTION(axis);
        // WiiU TODO: maybe implement
#ifndef __WIIU__
        int hatval = SDL_JoystickGetHat(joystick, HAT_AXIS_HAT(axis));

        if (direction == HAT_AXIS_HORIZONTAL)
        {
            if ((hatval & SDL_HAT_LEFT) != 0)
            {
                result -= 32767;
            }
            else if ((hatval & SDL_HAT_RIGHT) != 0)
            {
                result += 32767;
            }
        }
        else if (direction == HAT_AXIS_VERTICAL)
        {
            if ((hatval & SDL_HAT_UP) != 0)
            {
                result -= 32767;
            }
            else if ((hatval & SDL_HAT_DOWN) != 0)
            {
                result += 32767;
            }
        }
#endif // !__WIIU__
    }
    else
    {
#ifdef __WIIU__
        result = WiiU_JoystickGetAxis(axis);
#else
        result = SDL_JoystickGetAxis(joystick, axis);
#endif // __WIIU__

        if (result < DEAD_ZONE && result > -DEAD_ZONE)
        {
            result = 0;
        }
    }

    if (invert)
    {
        result = -result;
    }

    return result;
}

void I_UpdateJoystick(void)
{
#ifndef __WIIU__
    if (joystick != NULL)
#endif // !__WIIU__
    {
        event_t ev;

#ifdef __WIIU__
        WiiU_PollJoystick();
#endif // __WIIU__

        ev.type = ev_joystick;
        ev.data1 = GetButtonsState();
        ev.data2 = GetAxisState(joystick_x_axis, joystick_x_invert);
        ev.data3 = GetAxisState(joystick_y_axis, joystick_y_invert);
        ev.data4 = GetAxisState(joystick_strafe_axis, joystick_strafe_invert);
        ev.data5 = GetAxisState(joystick_look_axis, joystick_look_invert);

#ifdef BETTER_JOYWAIT
        prevJoystick = currentJoystick;
        currentJoystick = ev;

        joywaitDiff = I_GetTime() - joywait;
#endif // BETTER_JOYWAIT

        D_PostEvent(&ev);
    }
}

void I_BindJoystickVariables(void)
{
#ifndef __WIIU__
    int i;

    M_BindIntVariable("use_joystick",          &usejoystick);
    M_BindStringVariable("joystick_guid",      &joystick_guid);
    M_BindIntVariable("joystick_index",        &joystick_index);
#endif // !__WIIU__
    M_BindIntVariable("joystick_x_axis",       &joystick_x_axis);
    M_BindIntVariable("joystick_y_axis",       &joystick_y_axis);
    M_BindIntVariable("joystick_strafe_axis",  &joystick_strafe_axis);
    M_BindIntVariable("joystick_x_invert",     &joystick_x_invert);
    M_BindIntVariable("joystick_y_invert",     &joystick_y_invert);
    M_BindIntVariable("joystick_strafe_invert",&joystick_strafe_invert);
    M_BindIntVariable("joystick_look_axis",    &joystick_look_axis);
    M_BindIntVariable("joystick_look_invert",  &joystick_look_invert);

#ifndef __WIIU__
    for (i = 0; i < NUM_VIRTUAL_BUTTONS; ++i)
    {
        char name[32];
        M_snprintf(name, sizeof(name), "joystick_physical_button%i", i);
        M_BindIntVariable(name, &joystick_physical_buttons[i]);
    }
#endif // !__WIIU__
}

