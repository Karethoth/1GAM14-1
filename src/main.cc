#include "main.hh"
#include "../deb/glm/gtc/matrix_transform.hpp"
#include "../deb/glm/gtx/quaternion.hpp"


// Global managers, at least for now
MeshManager          meshManager;
TextureManager       textureManager;
ShaderProgramManager shaderManager;

// The scene stack
SceneStack           sceneStack;


// We want to keep track of possible joystick
std::unique_ptr<Joystick> joystick;
bool joystickYInverted = false;

// FreeType library
FT_Library ftLibrary;


// Callbacks for GLFW events
static void GLFWErrorCallback( int error, const char* description )
{
	std::cerr << "Error: " << description << "\n";
}


static void GLFWKeyCallback( GLFWwindow* window,
                             int key, int scancode,
                             int action, int mods )
{
	// TODO: Get rid of this ESCAPE key handling here, and
	//       let Scene handle it by itself as it sees fit
	if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		glfwSetWindowShouldClose( window, GL_TRUE );

	InputEvent event;
	event.type = KEYBOARD_INPUT;

	event.subType     = action ? KEY_DOWN : KEY_UP;
	event.index       = key;
	event.scancode    = scancode;
	event.buttonValue = action;
	event.mods        = mods;

	auto scene = sceneStack.Top();
	if( scene )
	{
		scene->HandleInputEvent( event );
	}
}


static void GLFWFrameBufferSizeCallback( GLFWwindow* window, int width, int height )
{
	glViewport( 0, 0, width, height );

	WindowEvent event;
	event.type = WINDOW_FRAMEBUFFER_RESIZE;
	event.point = glm::vec2( width, height );

	// TODO: Send the vent o the current scene
	/*gui.SetSize( static_cast<float>( width ),
	             static_cast<float>( height ) );
	*/
}


static void GLFWMouseButtonCallback( GLFWwindow* window, int button, int action, int mods )
{
	InputEvent event;

	event.type    = MOUSE_INPUT;
	event.subType = action? MOUSE_BUTTON_DOWN : MOUSE_BUTTON_UP;

	double xPos;
	double yPos;
	glfwGetCursorPos( window, &xPos, &yPos );
	event.point = glm::vec2( static_cast<float>( xPos ),
	                         static_cast<float>( yPos ) );

	event.index       = button;
	event.buttonValue = action;
	event.mods        = mods;

	auto scene = sceneStack.Top();
	if( scene )
	{
		scene->HandleInputEvent( event );
	}
}


static void GLFWMouseMoveCallback( GLFWwindow* window, double xPos, double yPos )
{
	InputEvent event;
	event.type    = MOUSE_INPUT;
	event.subType = MOUSE_MOVE;

	event.point = glm::vec2( static_cast<float>( xPos ),
	                         static_cast<float>( yPos ) );

	auto scene = sceneStack.Top();
	if( scene )
	{
		scene->HandleInputEvent( event );
	}
}


static void GLFWMouseScrollCallback( GLFWwindow* window, double xOffset, double yOffset )
{
	InputEvent event;

	event.type    = MOUSE_INPUT;
	event.subType = MOUSE_SCROLL;
	event.scroll = glm::vec2( static_cast<float>( xOffset ),
	                          static_cast<float>( yOffset ) );

	double xPos;
	double yPos;
	glfwGetCursorPos( window, &xPos, &yPos );
	event.point = glm::vec2( static_cast<float>( xPos ),
	                         static_cast<float>( yPos ) );

	auto scene = sceneStack.Top();
	if( scene )
	{
		scene->HandleInputEvent( event );
	}
}



static void JoystickCallback( const Joystick& joystick, const JoystickEvent& event )
{
	if( event.type == BUTTON_STATE_CHANGE &&
		event.buttonState == PRESS )
	{
		// Xbox 360 controller, left stick press
		if( event.index == 0x08 )
		{
			joystickYInverted = !joystickYInverted;
		}
	}


	InputEvent newEvent;
	newEvent.type = JOYSTICK_INPUT;

	switch( event.type )
	{
	 case AXIS_CHANGE:
		newEvent.subType   = JOYSTICK_AXIS_CHANGE;
		newEvent.index     = event.index;
		newEvent.axisValue = event.axisValue;
		break;

	 case BUTTON_STATE_CHANGE:
		newEvent.subType = event.buttonState == PRESS ? JOYSTICK_BUTTON_PRESS
		                                              : JOYSTICK_BUTTON_RELEASE;
		newEvent.index       = event.index;
		newEvent.buttonValue = event.buttonState;
		break;

	 case PLUGGED_IN:
		newEvent.subType = JOYSTICK_CONNECTED;
		break;

	 case PLUGGED_OUT:
		newEvent.subType = JOYSTICK_DISCONNECTED;
		break;

	 default:
		newEvent.subType = UNDEF_INPUT_SUB_TYPE;
		break;
	}

	auto scene = sceneStack.Top();
	if( scene )
	{
		scene->HandleInputEvent( newEvent );
	}
}



void CheckJoystick()
{
	if( joystick )
	{
		// If we got a joystick that's fine, return
		if( joystick->Validate() )
		{
			return;
		}

		JoystickEvent event;
		event.type = PLUGGED_OUT;
		JoystickCallback( *joystick, event );
		joystick.release();
	}


	// Check for a supported joystick
	for( int joystickId = 0; joystickId <= 15; ++joystickId )
	{
		if( !glfwJoystickPresent( joystickId ) )
		{
			continue;
		}

		Joystick* tmpJoystick = new Joystick( joystickId );
		if( tmpJoystick->GetAxes().size() == 5 )
		{
			joystick = std::unique_ptr<Joystick>( tmpJoystick );
			break;
		}
		else
		{
			delete tmpJoystick;
		}
	}

	// If we found a supported joystick, notify about it
	if( joystick )
	{
		joystick->AddEventHandler( JoystickCallback );
		JoystickEvent event;
		event.type = PLUGGED_IN;
		JoystickCallback( *joystick, event );
	}
}



// Loader and initializer functions
bool LoadShaders()
{
	// Compile and link the default 3D shader
	Shader vShader( GL_VERTEX_SHADER );
	Shader fShader( GL_FRAGMENT_SHADER );

	if( !vShader.LoadFromFile( "data/shaders/test.vshader" ) )
	{
		std::cerr << "Failed to load vertex shader!\n";
		return false;
	}

	if( !fShader.LoadFromFile( "data/shaders/test.fshader" ) )
	{
		std::cerr << "Failed to load fragment shader!\n";
		return false;
	}

	// Construct list of attributes and their locations
	std::map<std::string, GLuint> attributes;
	attributes["vertexPosition"] = 0;
	attributes["vertexNormal"]   = 1;
	attributes["textureCoord"]   = 2;


	// Create the shader and pass the shaders and the attribute list to it.
	auto shaderProgram = std::make_shared<ShaderProgram>();
	if( !shaderProgram->Load( vShader, fShader, attributes ) )
	{
		std::cerr << "Error: Compiling or linking the shader program has failed.\n";
		return false;
	}

	// Give the shaderProgram to the manager
	shaderManager.Add( "DefaultShader", shaderProgram );



	// Repeat the process for the GUI shader
	Shader guiVShader( GL_VERTEX_SHADER );
	Shader guiFShader( GL_FRAGMENT_SHADER );

	if( !guiVShader.LoadFromFile( "data/shaders/gui.vshader" ) )
	{
		std::cerr << "Failed to load vertex shader!\n";
		return false;
	}

	if( !guiFShader.LoadFromFile( "data/shaders/gui.fshader" ) )
	{
		std::cerr << "Failed to load fragment shader!\n";
		return false;
	}


	// We'll use the same attributes as previously

	// Create the shader and pass the shaders and the attribute list to it.
	auto guiShaderProgram = std::make_shared<ShaderProgram>();
	if( !guiShaderProgram->Load( guiVShader, guiFShader, attributes ) )
	{
		std::cerr << "Error: Compiling or linking the shader program has failed.\n";
		return false;
	}

	// Give the shaderProgram to the manager
	shaderManager.Add( "DefaultGUIShader", guiShaderProgram );

	return true;
}



bool LoadTextures()
{
	if( !textureManager.Load( "data/images/pebbles_Diffuse.png", "pebbles" ) )
	{
		return false;
	}

	if( !textureManager.Load( "data/images/tree.png", "tree" ) )
	{
		return false;
	}

	if( !textureManager.Load( "data/images/redbrick_Diffuse.png", "bricks" ) )
	{
		return false;
	}

	if( !textureManager.Load( "data/images/defaultHead.png", "DefaultHead" ) )
	{
		return false;
	}

	if( !textureManager.Load( "data/images/defaultTorso.png", "DefaultTorso" ) )
	{
		return false;
	}


	// Generate the default texture
	auto pixelArray = NoiseArray( 3, 9*9 );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	auto texture = std::make_shared<Texture>( "DefaultTexture" );

	glBindTexture( GL_TEXTURE_2D, texture->textureId );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 9, 9, 0, GL_RGB,
					GL_FLOAT, &pixelArray.get()[0][0] );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	textureManager.Add( "DefaultTexture", texture );


	return true;
}



bool InitializeFreeType()
{
	if( FT_Init_FreeType( &ftLibrary ) )
	{
		return false;
	}

	return true;
}



int main( int argc, char **argv )
{
	GLFWwindow* window;

	glfwSetErrorCallback( GLFWErrorCallback );
	if( !glfwInit() )
	{
		std::cerr << "Couldn't initialize GLFW\n";
		exit( EXIT_FAILURE );
	}

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 1 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
	//glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE );
	glfwWindowHint( GLFW_SAMPLES, 4 );


	window = glfwCreateWindow( 600,
	                           400,
	                           "1GAM14-1", NULL, NULL );

	if( !window )
	{
		glfwTerminate();
		std::cerr << "Couldn't create window\n";
		exit( EXIT_FAILURE );
	}

	glfwMakeContextCurrent( window );
	glfwSetMouseButtonCallback( window, GLFWMouseButtonCallback );
	glfwSetCursorPosCallback( window, GLFWMouseMoveCallback );
	glfwSetScrollCallback( window, GLFWMouseScrollCallback );
	glfwSetKeyCallback( window, GLFWKeyCallback );
	glfwSetFramebufferSizeCallback( window, GLFWFrameBufferSizeCallback );

	glewExperimental = GL_TRUE;
	if( glewInit() != GLEW_OK )
	{
		std::cerr << "Couldn't initialize GLEW\n";
		exit( EXIT_FAILURE );
	}


	if( !InitializeFreeType() )
	{
		std::cerr << "Failed to initialize FreeType!\n";
		return -1;
	}


	// Load Shaders
	if( !LoadShaders() )
	{
		std::cerr << "Failed to load shaders!\n";
		return -1;
	}

	// Load textures
	if( !LoadTextures() )
	{
		std::cerr << "Failed to load textures!\n";
		return -1;
	}

	// Generate the scene stack and a scene.
	auto testScene = std::make_shared<TestScene>();
	testScene->Create();
	sceneStack.Push( testScene );



	// Set some GL settings up
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	//glLineWidth( 2.0 );
	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );


	// We need to know how long has been since the last update
	auto time = glfwGetTime();
	auto deltaTime = time;


	// Set the clear color
	glClearColor( 0.3f, 0.3f, 1.0f, 1.0f );

	// Counter for checking joystick
	float joystickCheckTime = 0.f;

	/* Main loop */
	while( !glfwWindowShouldClose( window ) )
	{
		deltaTime = glfwGetTime()-time;
		time = glfwGetTime();

		// Check joystick when it's time
		joystickCheckTime -= static_cast<float>( deltaTime );
		if( joystickCheckTime <= 0.f )
		{
			std::async( std::launch::async, CheckJoystick );
			joystickCheckTime = 5.f;
		}


		auto scene = sceneStack.Top();
		if( !scene )
		{
			break;
		}

		scene->Tick( deltaTime );

		glfwSwapBuffers( window );
		glfwPollEvents();
	}


    //glDisableVertexAttribArray( shaderProgram->GetAttribute( "textureCoord" ) );
    //glDisableVertexAttribArray( shaderProgram->GetAttribute( "vertexNormal" ) );
    //glDisableVertexAttribArray( shaderProgram->GetAttribute( "vertexPosition" ) );

	glfwDestroyWindow( window );
	glfwTerminate();
	return 0;
}

