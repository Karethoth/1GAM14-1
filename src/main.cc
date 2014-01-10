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


// Global managers, at least for now
MeshManager meshManager;
TextureManager textureManager;
ShaderProgramManager shaderManager;



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
}


static void GLFWFrameBufferSizeCallback( GLFWwindow* window, int width, int height )
{
	windowInfo.width  = width;
	windowInfo.height = height;

	glViewport( 0, 0, width, height );
	windowInfo.UpdateRatio();
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

	/* Shader testing */
	Shader vShader( GL_VERTEX_SHADER );
	Shader fShader( GL_FRAGMENT_SHADER );

	if( !vShader.LoadFromFile( "data/shaders/test.vshader" ) )
	{
		std::cerr << "Failed to load vertex shader!\n";
	}

	if( !fShader.LoadFromFile( "data/shaders/test.fshader" ) )
	{
		std::cerr << "Failed to load fragment shader!\n";
	}

	if( !textureManager.Load( "data/images/pebbles_Diffuse.png" ) )
	{
		std::cerr << "Failed to load image!\n";
	}

	if( !textureManager.Load( "data/images/redbrick_Diffuse.png" ) )
	{
		std::cerr << "Failed to load image!\n";
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
		return 1;
	}

	// Give the shaderProgram to the manager
	shaderManager.Add( "TestShader", shaderProgram );


	// Get uniform locations
	const GLint viewUniform           = shaderProgram->GetUniform( "V" );
	const GLint projUniform           = shaderProgram->GetUniform( "P" );
	const GLint worldCenterUniform    = shaderProgram->GetUniform( "worldCenter" );
	const GLint lightDirectionUniform = shaderProgram->GetUniform( "lightDirection" );


	// Create the root node
	Node rootNode( "RootNode" );


	// Create the camera
	auto camera = std::make_shared<Camera>();
	camera->SetPosition( glm::vec3( 0.f, 8.f, -20.f ) );
	camera->SetTarget( glm::vec3( 0.0, 1.0, 0.0 ) );
	glm::vec3 cameraRot( 0.0, 0.0, 0.0 );
	camera->SetRotation( glm::normalize( glm::quat( cameraRot ) ) );
	camera->SetRatio( windowInfo.ratio );
	camera->SetFOV( 60.f );
	rootNode.AddChild( camera );


	// Generate Node that acts as the "center" of the world.
	// (vertices that are affected by it, rotate around it)
	auto worldCenter = std::make_shared<Node>( "WorldCenter" );
	worldCenter->SetPosition( glm::vec3( 0.0, -40.0, 0.0 ) );
	rootNode.AddChild( worldCenter );


	// Test mesh generation from a surface
	Surface groundSurface( 40, 40 );
	auto groundSurfaceMesh = groundSurface.GenerateMesh( 10, 10 );

	// Give name to the mesh and generate
	// the OpenGL arrays and buffers
	groundSurfaceMesh->name = "GroundSurfaceMesh";
	groundSurfaceMesh->GenerateGLBuffers( "TestShader" );

	// Give the mesh to the mesh manager
	meshManager.Add( "GroundSurfaceMesh", groundSurfaceMesh );

	// Generate entity and set it to use the ground surface mesh
	// and the wanted shader
	auto ground = std::make_shared<Entity>( "GroundSurfaceEntity" );
	ground->SetMeshName( "GroundSurfaceMesh" );
	ground->SetTextureName( "data/images/pebbles_Diffuse.png" );
	ground->SetShaderName( "TestShader" );
	ground->SetPosition( glm::vec3( -20.0, 0.0, -20.0 ) );

	// Give the entity to the root node
	rootNode.AddChild( ground );


	// Repeat process for a wall
	Surface wallSurface( 40, 10 );
	glm::vec2 corners[] =
	{
		glm::vec2( 5.0, 1.0 ),
		glm::vec2( 0.0, 1.0 ),
		glm::vec2( 5.0, 0.0 ),
		glm::vec2( 0.0, 0.0 )
	};
	wallSurface.SetTextureCorners( corners );
	auto wallMesh = wallSurface.GenerateMesh( 10, 5 );
	wallMesh->name = "WallMesh";
	wallMesh->GenerateGLBuffers( "TestShader" );
	meshManager.Add( "WallMesh", wallMesh );
	auto wall = std::make_shared<Entity>( "WallEntity" );
	wall->SetMeshName( "WallMesh" );
	wall->SetShaderName( "TestShader" );
	wall->SetTextureName( "data/images/redbrick_Diffuse.png" );
	wall->SetPosition( glm::vec3( 0.0, 0.0, 0.0 ) );
	wall->SetRotation( glm::quat( glm::vec3(
		ToRadians( -90.f ),
		ToRadians( -90.f ),
		0.0 )
	) );
	ground->AddChild( wall );


	// And another wall Entity!
	auto secondWall = std::make_shared<Entity>( "WallEntity2" );
	secondWall->SetMeshName( "WallMesh" );
	secondWall->SetTextureName( "data/images/redbrick_Diffuse.png" );
	secondWall->SetShaderName( "TestShader" );
	secondWall->SetPosition( glm::vec3( 0.0, 0.0, -10.0 ) );
	secondWall->SetRotation( glm::quat( glm::vec3(
		ToRadians( 90.f ),
		0.0,
		0.0 )
	) );
	ground->AddChild( secondWall );


	// Test texture manager
	{
		// Generate the default texture
		auto pixelArray = NoiseArray( 3, 4 );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

		auto texture = std::make_shared<Texture>( "DefaultTexture" );

		glBindTexture( GL_TEXTURE_2D, texture->textureId );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB,
					  GL_FLOAT, &pixelArray.get()[0][0] );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		textureManager.Add( "DefaultTexture", texture );
	}
	{
		auto pixelArray = NoiseArray( 3, 9*9 );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

		auto texture = std::make_shared<Texture>( "TestTexture" );

		glBindTexture( GL_TEXTURE_2D, texture->textureId );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 9, 9, 0, GL_RGB,
					  GL_FLOAT, &pixelArray.get()[0][0] );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		textureManager.Add( "TestTexture", texture );
	}


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


	glClearColor( 0.3f, 0.3f, 1.0f, 1.0f );

	/* Main loop */
	while( !glfwWindowShouldClose( window ) )
	{
		deltaTime = glfwGetTime()-time;
		time = glfwGetTime();
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glUseProgram( shaderProgram->Get() );

		// Camera handling and matrix stuff
		camera->SetRatio( windowInfo.ratio );
		auto projectionMat = camera->GetProjectionMatrix();
		auto viewMat = camera->GetViewMatrix();

		// Rotate the ground
		ground->SetRotation( glm::normalize( glm::quat( glm::vec3( 0.f, deltaTime, 0.f) ) * ground->GetRotation() ) );

		// Update the node tree
		rootNode.UpdateWorldInfo();

		// Upload uniforms to GPU
		glUniformMatrix4fv( viewUniform,  1, GL_FALSE, &(*viewMat)[0][0] );
		glUniformMatrix4fv( projUniform,  1, GL_FALSE, &(*projectionMat)[0][0] );
		glUniform3fv( worldCenterUniform, 1, &worldCenter->GetPosition()[0] );
		glUniform3fv( lightDirectionUniform, 1, &glm::normalize( glm::vec3( 0.0, 1.0, 1.0 ) )[0] );


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

