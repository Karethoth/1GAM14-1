#include "joystick.hh"


Joystick::Joystick( int joystickId ) : id( joystickId )
{
	name = std::string( glfwGetJoystickName( id ) );

	// Get input counts
	glfwGetJoystickAxes( id, &axisCount );
	glfwGetJoystickButtons( id, &buttonCount );

	axes.resize( axisCount );
	buttons.resize( buttonCount );

	// Get the start values
	Update();
}


Joystick::~Joystick()
{
	axes.clear();
	buttons.clear();
}



void Joystick::Update()
{
	int axisCountNow;
	int buttonCountNow;
	auto axisValues   = glfwGetJoystickAxes( id, &axisCountNow );
	auto buttonValues = glfwGetJoystickButtons( id, &buttonCountNow );

	if( axisCountNow > axisCount )
	{
		axisCountNow = axisCount;
	}
	if( buttonCountNow > buttonCount )
	{
		buttonCountNow = buttonCount;
	}

	for( int ind=0; ind < axisCountNow; ++ind )
	{
		axes[ind] = axisValues[ind];
	}

	for( int ind=0; ind < buttonCountNow; ++ind )
	{
		buttons[ind] = buttonValues[ind];
	}
}



bool Joystick::Validate()
{
	// Check if the joystick's still present and has the same name
	if( !glfwJoystickPresent( id ) )
	{
		return false;
	}

	if( std::string( glfwGetJoystickName( id ) ).compare( name ) )
	{
		return false;
	}

	// Check if the input counts match up
	int axisCountNow;
	int buttonCountNow;
	glfwGetJoystickAxes( id, &axisCountNow );
	glfwGetJoystickButtons( id, &buttonCountNow );

	if( axisCountNow   != axisCount ||
		buttonCountNow != buttonCount )
	{
		return false;
	}

	return true;
}



std::vector<float> Joystick::GetAxes()
{
	return axes;
}


std::vector<unsigned char> Joystick::GetButtons()
{
	return buttons;
}


float Joystick::GetAxis( int index )
{
	if( static_cast<unsigned int>( index ) >= axes.size() )
		return 0.f;

	return axes[index];
}


unsigned char Joystick::GetButton( int index )
{
	if( static_cast<unsigned int>( index ) >= buttons.size() )
		return GL_FALSE;

	return buttons[index];
}

