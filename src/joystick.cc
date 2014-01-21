#include "joystick.hh"
#include <iostream>


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

	JoystickEvent event;
	event.type = AXIS_CHANGE;
	for( int ind=0; ind < axisCountNow; ++ind )
	{
		if( axes[ind] != axisValues[ind] )
		{
			event.index = ind;
			event.axisValue = axisValues[ind];
			DispatchEvent( event );
		}
		axes[ind] = axisValues[ind];
	}

	event.type = BUTTON_STATE_CHANGE;
	event.axisValue = 0.f;
	for( int ind=0; ind < buttonCountNow; ++ind )
	{
		if( buttons[ind] != buttonValues[ind] )
		{
			event.index = ind;
			event.buttonState = buttonValues[ind] ? PRESS : RELEASE;
			DispatchEvent( event );
		}
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



int Joystick::GetId() const
{
	return id;
}


std::string Joystick::GetName() const
{
	return name;
}


std::vector<float> Joystick::GetAxes() const
{
	return axes;
}


std::vector<unsigned char> Joystick::GetButtons() const
{
	return buttons;
}


float Joystick::GetAxis( int index ) const
{
	if( static_cast<unsigned int>( index ) >= axes.size() )
	{
		return 0.f;
	}

	return axes[index];
}


unsigned char Joystick::GetButton( int index ) const
{
	if( static_cast<unsigned int>( index ) >= buttons.size() )
	{
		return GL_FALSE;
	}

	return buttons[index];
}



void Joystick::AddEventHandler( JoystickEventHandler handler )
{
	eventHandlers.push_back( handler );
}



void Joystick::DispatchEvent( const JoystickEvent& event )
{
	for( auto& handler : eventHandlers )
	{
		handler( *this, event );
	}
}

