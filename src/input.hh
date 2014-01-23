#pragma once
#ifndef _INPUT_HH_
#define _INPUT_HH_

#include "../deb/glm/glm.hpp"



enum InputEventType
{
	MOUSE_INPUT,
	KEYBOARD_INPUT,
	JOYSTICK_INPUT
};


enum InputEventSubType
{
	UNDEF_INPUT_SUB_TYPE,
	MOUSE_BUTTON_DOWN,
	MOUSE_BUTTON_UP,
	MOUSE_MOVE,
	MOUSE_SCROLL,
	KEY_DOWN,
	KEY_UP,
	JOYSTICK_AXIS_CHANGE,
	JOYSTICK_BUTTON_PRESS,
	JOYSTICK_BUTTON_RELEASE,
	JOYSTICK_CONNECTED,
	JOYSTICK_DISCONNECTED
};



typedef struct sInputEvent
{
	// Event type info
	InputEventType    type;
	InputEventSubType subType;

	// Common
	int index;
	int mods;
	int buttonValue;

	// Mouse
	glm::vec2 point;
	glm::vec2 scroll;

	// Key
	int scancode;

	// Joystick
	float axisValue;

} InputEvent;


#endif

