#pragma once
#ifndef _JOYSTICK_HH_
#define _JOYSTICK_HH_

#include <GLFW/glfw3.h>
#include <vector>
#include <string>


class Joystick;


enum JoystickEventType
{
	UNDEFINED_JOYSTICK_EVENT = 0,
	AXIS_CHANGE,
	BUTTON_STATE_CHANGE,
	PLUGGED_IN,
	PLUGGED_OUT
};


enum JoystickButtonState
{
	RELEASE = 0,
	PRESS
};


struct JoystickEvent
{
	JoystickEventType   type;
	JoystickButtonState buttonState;
	int                index;
	float              axisValue;
};



typedef void (*JoystickEventHandler)( const Joystick& joystick,
                                      const JoystickEvent& event );



class Joystick
{
 public:
	Joystick( int joystickId );
	~Joystick();

	void Update();
	bool Validate();

	int GetId() const;
	std::string GetName() const;

	std::vector<float>         GetAxes() const;
	std::vector<unsigned char> GetButtons() const;

	float         GetAxis( int index ) const;
	unsigned char GetButton( int index ) const;

	void AddEventHandler( JoystickEventHandler handler );
	void RemoveEventHandler( JoystickEventHandler handler );


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

