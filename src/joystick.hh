#pragma once
#ifndef _JOYSTICK_HH_
#define _JOYSTICK_HH_

#include <GLFW/glfw3.h>
#include <vector>
#include <string>


enum JoysticEventType
{
	UNDEFINED_JOYSTICK_EVENT = 0,
	AXIS_CHANGE,
	BUTTON_STATE_CHANGE,
	PLUGGED_IN,
	PLUGGED_OUT
};


enum JoysticButtonState
{
	RELEASE = 0,
	PRESS
};


typedef struct SJoysticEvent
{
	JoysticEventType   type;
	JoysticButtonState buttonState;
	int                index;
	float              axisValue;
} JoystickEvent;



class Joystick;
typedef void (*JoystickEventHandler)( const Joystick& joystick,
                                       const JoystickEvent& event );



class Joystick
{
 public:
	Joystick( int joystickId );
	~Joystick();

	void Update();
	bool Validate();

	int GetId();
	std::string GetName();

	std::vector<float>         GetAxes();
	std::vector<unsigned char> GetButtons();

	float         GetAxis( int index );
	unsigned char GetButton( int index );

	void AddEventHandler( JoystickEventHandler handler );


 protected:
	void DispatchEvent( const JoystickEvent& event );


 private:
	std::string name;
	int id;
	int axisCount;
	int buttonCount;
	std::vector<float>         axes;
	std::vector<unsigned char> buttons;
	std::vector<JoystickEventHandler> eventHandlers;
};

#endif

