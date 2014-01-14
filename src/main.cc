#include "main.hh"
#include "../deb/glm/gtc/matrix_transform.hpp"
#include "../deb/glm/gtx/quaternion.hpp"


// Global to hold window information.
struct WindowInfo
{
	int   width;
	int   height;
	float ratio;

	WindowInfo()
	{
		width  = 600;
		height = 400;
	}

	void UpdateRatio()
	{
		ratio = (float)width/(float)height;
	}
} windowInfo;


// Important nodes and entities
Node rootNode;
std::shared_ptr<Node>      light;
std::shared_ptr<Node>      world;
std::shared_ptr<Node>      worldCenter;
std::shared_ptr<Camera>    camera;
std::shared_ptr<Character> player;


// Global managers, at least for now
MeshManager meshManager;
TextureManager textureManager;
ShaderProgramManager shaderManager;

struct
{
	bool up;
	bool down;
	bool left;
	bool right;
} keysDown = {false, false, false, false};

// Callbacks for GLFW events
static void GLFWErrorCallback( int error, const char* description )
{
	std::cerr << "Error: " << description << "\n";
}


static void GLFWKeyCallback( GLFWwindow* window,
                             int key, int scancode,
                             int action, int mods )
{
	if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		glfwSetWindowShouldClose( window, GL_TRUE );

	if( key == GLFW_KEY_UP )
	{
		keysDown.up = !(!action);
	}
	if( key == GLFW_KEY_DOWN )
	{
		keysDown.down = !(!action);
	}
	if( key == GLFW_KEY_LEFT )
	{
		keysDown.left = !(!action);
	}
	if( key == GLFW_KEY_RIGHT )
	{
		keysDown.right = !(!action);
	}
}


static void GLFWFrameBufferSizeCallback( GLFWwindow* window, int width, int height )
{
	windowInfo.width  = width;
	windowInfo.height = height;

	glViewport( 0, 0, width, height );
	windowInfo.UpdateRatio();
}


// Loader and initializer functions
bool LoadShaders()
{
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



bool CreateScene()
{
	// Create the world node
	world = std::make_shared<Node>( "World" );
	rootNode.AddChild( world );

	// Create the camera
	camera = std::make_shared<Camera>();
	camera->SetPosition( glm::vec3( 0.f, 10.f, 15.f ) );
	camera->SetTarget( glm::vec3( 0.0, 0.0, 0.0 ) );
	camera->SetRatio( windowInfo.ratio );
	camera->SetFOV( 45.f );


	// Generate Node that acts as the "center" of the world.
	// (vertices that are affected by it, rotate around it)
	worldCenter = std::make_shared<Node>( "WorldCenter" );
	worldCenter->SetPosition( glm::vec3( 0.0, -20.0, 0.0 ) );


	// Generate ground surface
	Surface groundSurface( 40, 40 );
	glm::vec2 corners[] =
	{
		glm::vec2( 2.0, 3.0 ),
		glm::vec2( 0.0, 3.0 ),
		glm::vec2( 2.0, 0.0 ),
		glm::vec2( 0.0, 0.0 )
	};
	groundSurface.SetTextureCorners( corners );
	auto groundSurfaceMesh = groundSurface.GenerateMesh( 10, 10 );

	// Give name to the mesh and generate
	// the OpenGL arrays and buffers
	groundSurfaceMesh->name = "GroundSurfaceMesh";
	groundSurfaceMesh->GenerateGLBuffers( "DefaultShader" );

	// Give the mesh to the mesh manager
	meshManager.Add( "GroundSurfaceMesh", groundSurfaceMesh );

	// Generate entity and set it to use the ground surface mesh
	// and the wanted shader
	auto ground = std::make_shared<Entity>( "GroundSurfaceEntity" );
	ground->SetMeshName( "GroundSurfaceMesh" );
	ground->SetTextureName( "pebbles" );
	ground->SetShaderName( "DefaultShader" );
	ground->SetPosition( glm::vec3( -20.0, 0.0, -20.0 ) );


	// Give the entity to the world node
	world->AddChild( ground );


	// Repeat process for a wall
	Surface wallSurface( 40, 10 );
	corners[0] = glm::vec2( 0.0, 0.0 );
	corners[1] = glm::vec2( 5.0, 0.0 );
	corners[2] = glm::vec2( 0.0, 1.0 );
	corners[3] = glm::vec2( 5.0, 1.0 );
	wallSurface.SetTextureCorners( corners );
	auto wallMesh = wallSurface.GenerateMesh( 10, 5 );
	wallMesh->name = "WallMesh";
	wallMesh->GenerateGLBuffers( "DefaultShader" );
	meshManager.Add( "WallMesh", wallMesh );
	auto wall = std::make_shared<Entity>( "WallEntity" );
	wall->SetMeshName( "WallMesh" );
	wall->SetShaderName( "DefaultShader" );
	wall->SetTextureName( "bricks" );
	wall->SetPosition( glm::vec3( 20.0, 0.0, -20.0 ) );
	wall->SetOrientation( glm::quat( glm::vec3(
		ToRadians( -90.f ),
		ToRadians( 180.f ),
		0.0 )
	) );
	world->AddChild( wall );


	// And another wall entity!
	auto secondWall = std::make_shared<Entity>( "WallEntity2" );
	secondWall->SetMeshName( "WallMesh" );
	secondWall->SetTextureName( "bricks" );
	secondWall->SetShaderName( "DefaultShader" );
	secondWall->SetPosition( glm::vec3( -20.0, 0.0, -20.0 ) );
	secondWall->SetOrientation( glm::quat( glm::vec3(
		ToRadians( -90.f ),
		ToRadians( -90.f ),
		0.0 )
	) );
	world->AddChild( secondWall );


	// Create a tree to test textures with alpha layer
	Surface squareSurface( 5, 5 );
	auto squareMesh = squareSurface.GenerateMesh( 2, 2 );
	squareMesh->name = "SquareMesh";
	squareMesh->GenerateGLBuffers( "DefaultShader" );
	meshManager.Add( "SquareMesh", squareMesh );
	auto tree = std::make_shared<Entity>( "Tree" );
	tree->SetMeshName( "SquareMesh" );
	tree->SetTextureName( "tree" );
	tree->SetShaderName( "DefaultShader" );
	tree->SetPosition( glm::vec3( -15.0, 4.9, -10.0 ) );
	tree->SetOrientation( glm::quat( glm::vec3(
		ToRadians( 90.f ),
		ToRadians( 45.f ),
		0.0 )
	) );
	world->AddChild( tree );


	// Generate the head mesh
	Surface headSurface( 2, 2 );
	auto headMesh = headSurface.GenerateMesh( 1, 1 );
	headMesh->name = "HeadMesh";
	headMesh->GenerateGLBuffers( "DefaultShader" );
	meshManager.Add( "HeadMesh", headMesh );


	// Generate the torso mesh
	Surface torsoSurface( 2, 2 );
	auto torsoMesh = torsoSurface.GenerateMesh( 2, 2 );
	torsoMesh->name = "MediumTorsoMesh";
	torsoMesh->GenerateGLBuffers( "DefaultShader" );
	meshManager.Add( "MediumTorsoMesh", torsoMesh );

	// Create a Node to hold position of the light
	light = std::make_shared<Node>( "TestLight" );
	light->SetPosition( glm::vec3( -10.0, 8.0, -10.0 ) );
	world->AddChild( light );
	return true;
}



int main( int argc, char **argv )
{
	GLFWwindow* window;
	windowInfo.width  = 600;
	windowInfo.height = 400;
	windowInfo.UpdateRatio();

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


	window = glfwCreateWindow( windowInfo.width,
	                           windowInfo.height,
	                           "1GAM14-1", NULL, NULL );

	if( !window )
	{
		glfwTerminate();
		std::cerr << "Couldn't create window\n";
		exit( EXIT_FAILURE );
	}

	glfwMakeContextCurrent( window );
	glfwSetKeyCallback( window, GLFWKeyCallback );
	glfwSetFramebufferSizeCallback( window, GLFWFrameBufferSizeCallback );

	glewExperimental = GL_TRUE;
	if( glewInit() != GLEW_OK )
	{
		std::cerr << "Couldn't initialize GLEW\n";
		exit( EXIT_FAILURE );
	}


	// Create the root node
	rootNode = Node( "RootNode" );


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

	// Create the scene
	if( !CreateScene() )
	{
		std::cerr << "Failed to create the scene!\n";
		return -1;
	}

	player = std::make_shared<Character>( "Player" );
	player->SetPosition( glm::vec3( 0.0, 0.0, 0.0 ) );
	player->AddChild( camera );
	player->AddChild( worldCenter );
	world->AddChild( player );

	// Get the needed uniform locations
	auto shaderProgram = shaderManager.Get( "DefaultShader" );
	const GLint viewUniform          = shaderProgram->GetUniform( "V" );
	const GLint projUniform          = shaderProgram->GetUniform( "P" );
	const GLint worldCenterUniform   = shaderProgram->GetUniform( "worldCenter" );
	const GLint lightPositionUniform = shaderProgram->GetUniform( "lightPosition" );

	// List joysticks
	std::unique_ptr<Joystick> joystick;
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
		}
		else
		{
			delete tmpJoystick;
		}
	}


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


	/* Main loop */
	while( !glfwWindowShouldClose( window ) )
	{
		deltaTime = glfwGetTime()-time;
		time = glfwGetTime();
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glUseProgram( shaderProgram->Get() );

		// Move the character, for now I'll let this snippet be here
		glm::vec3 move;
		glm::vec3 newVelocity;

		// Calculate the slowdown from previous velocity
		glm::vec3 slowDown = player->GetVelocity() * ((float)deltaTime*10);


		// Modify the move vector according to the input
		if( keysDown.up )
			move += glm::vec3( 0.0, 0.0, -2.0/2.5 );
		if( keysDown.down )
			move += glm::vec3( 0.0, 0.0, 2.0/2.5 );
		if( keysDown.left )
			move += glm::vec3( -1, 0.0, 0.0 );
		if( keysDown.right )
			move += glm::vec3( 1, 0.0, 0.0 );


		// Bool which we use to check if we have any input from joystick
		bool joystickInput = false;

		// If we have a joystick
		if( joystick )
		{
			// Update joystick and get the axes that count
			joystick->Update();
			float xAxis = joystick->GetAxis( 0 );
			float yAxis = joystick->GetAxis( 1 );

			// Cap the input to zero if it's within the threshold
			const float threshold = 0.2f;
			if( xAxis < threshold && xAxis > -threshold )
				xAxis = 0.f;
			if( yAxis < threshold && yAxis > -threshold )
				yAxis = 0.f;

			// If any of the values is outside of the threshold(!0),
			// we got real input from joystick
			if( xAxis != 0.f || yAxis != 0.f )
			{
				joystickInput = true;
				move = glm::vec3( xAxis, 0.f, -yAxis );
			}
		}

		// If we got movement, we'll normalize it and multiply
		// it by how hastely we want the character to speed up
		if( glm::length( move ) > 0.f )
		{
			// If we have joystick input we need to check that
			// the movement vector has length under 1.f.
			// This is because some joysticks have square movement
			// area, of which corners could otherwise be used to
			// move faster than intended. Same foes for the arrow keys.
			if( joystickInput && glm::length( move ) <= 1.f )
			{
				move *= 4.f;
			}
			else
			{
				move = glm::normalize( move ) * 4.f;
			}
		}

		// Calculate the new velocity
		newVelocity = player->GetVelocity() + move - slowDown;

		// If the velocity is above the maximum speed,
		// we'll cap it to it
		if( glm::length( newVelocity ) > 15.f )
		{
			newVelocity = glm::normalize( newVelocity );
			newVelocity *= 15.f;
		}

		// Set the velocity
		player->SetVelocity( newVelocity );
		player->Update( deltaTime );

		// Update the node tree
		rootNode.UpdateWorldInfo();

		camera->SetTarget( player->GetWorldPosition() );

		// Camera handling and matrix stuff
		camera->SetRatio( windowInfo.ratio );
		auto projectionMat = camera->GetProjectionMatrix();
		auto viewMat = camera->GetViewMatrix();

		// Upload uniforms to GPU
		glUniformMatrix4fv( viewUniform,  1, GL_FALSE, &(*viewMat)[0][0] );
		glUniformMatrix4fv( projUniform,  1, GL_FALSE, &(*projectionMat)[0][0] );
		glUniform3fv( worldCenterUniform, 1, &worldCenter->GetWorldPosition()[0] );
		glUniform3fv( lightPositionUniform, 1, &light->GetWorldPosition()[0] );

		// Render the current scene
		rootNode.Render();

		glfwSwapBuffers( window );
		glfwPollEvents();
	}


    glDisableVertexAttribArray( shaderProgram->GetAttribute( "textureCoord" ) );
    glDisableVertexAttribArray( shaderProgram->GetAttribute( "vertexNormal" ) );
    glDisableVertexAttribArray( shaderProgram->GetAttribute( "vertexPosition" ) );

	glfwDestroyWindow( window );
	glfwTerminate();
	return 0;
}

