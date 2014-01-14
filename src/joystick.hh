#pragma once
#ifndef _JOYSTICK_HH_
#define _JOYSTICK_HH_

#include <GLFW/glfw3.h>
#include <vector>
#include <string>


class Joystick
{
 public:
	Joystick( int joystickId );
	~Joystick();

	void Update();
	bool Validate();

	std::vector<float>         GetAxes();
	std::vector<unsigned char> GetButtons();

	float         GetAxis( int index );
	unsigned char GetButton( int index );


 private:
	 std::string name;
	 int id;
	 int axisCount;
	 int buttonCount;
	 std::vector<float>         axes;
	 std::vector<unsigned char> buttons;
};

#endif

