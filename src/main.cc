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


	// Construct list of attributes and their locations
	std::map<std::string, GLuint> attributes;
	attributes["vertexPosition"] = 0;
	attributes["vertexNormal"]   = 1;


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
	camera->SetLocation( glm::vec3( 0.f, 8.f, -10.f ) );
	camera->SetTarget( glm::vec3( 0.0, 1.0, 0.0 ) );
	glm::vec3 cameraRot( 0.0, 0.0, 0.0 );
	camera->SetRotation( glm::normalize( glm::quat( cameraRot ) ) );
	camera->SetRatio( windowInfo.ratio );
	camera->SetFOV( 60.f );
	rootNode.AddChild( camera );


	// Just something I'm temporarely using to have the terrain rotating.
	glm::quat rot = glm::quat( glm::vec3( 0.f, 0.05*glfwGetTime(), 0.f) );


	// Generate Node that acts as the "center" of the world.
	// (vertices that are affected by it, rotate around it)
	auto worldCenter = std::make_shared<Node>( "WorldCenter" );
	worldCenter->SetLocation( glm::vec3( 0.0, -70.0, 0.0 ) );
	rootNode.AddChild( worldCenter );


	// Test mesh generation from a surface
	Surface groundSurface( 40, 40 );
	auto groundSurfaceMesh = groundSurface.GenerateMesh( 10, 10 );

	// Give name to the mesh and generate
	// the OpenGL arrays and buffers
	groundSurfaceMesh->name = "GroundSurfaceMesh";
	groundSurfaceMesh->GenerateGLBuffers();

	// Give the mesh to the mesh manager
	meshManager.Add( "GroundSurfaceMesh", groundSurfaceMesh );

	// Generate entity and set it to use the ground surface mesh
	// and the wanted shader
	auto ground = std::make_shared<Entity>( "GroundSurfaceEntity" );
	ground->SetMeshName( "GroundSurfaceMesh" );
	ground->SetShaderName( "TestShader" );
	ground->SetLocation( glm::vec3( -20.0, 0.0, -20.0 ) );

	// Give the entity to the root node
	rootNode.AddChild( ground );


	glEnableVertexAttribArray( shaderProgram->GetAttribute( "vertexPosition" ) );
	glEnableVertexAttribArray( shaderProgram->GetAttribute( "vertexNormal" ) );
	glBindBuffer( GL_ARRAY_BUFFER, meshManager.Get( "GroundSurfaceMesh" )->vbo );

	// Vertex position
	glVertexAttribPointer(
	   shaderProgram->GetAttribute( "vertexPosition" ),
	   3,
	   GL_FLOAT,
	   GL_FALSE,
	   sizeof( VBOData ),
	   (void*)0
	);

	// Vertex normal
	glVertexAttribPointer(
	   shaderProgram->GetAttribute( "vertexNormal" ),
	   3,
	   GL_FLOAT,
	   GL_TRUE,
	   sizeof( VBOData ),
	   (void*)sizeof(glm::vec3)
	);

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glLineWidth( 2.0 );

	//glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );


	/* Main loop */
	while( !glfwWindowShouldClose( window ) )
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glUseProgram( shaderProgram->Get() );

		// Camera handling and matrix stuff
		camera->SetRatio( windowInfo.ratio );
		auto projectionMat = camera->GetProjectionMatrix();
		auto viewMat = camera->GetViewMatrix();


		// Get the mesh from manager. Testing the manager....
		auto mesh = meshManager.Get( "GroundSurfaceMesh" );
		ground->SetRotation( glm::normalize( rot * ground->GetRotation() ) );


		// Update the node tree
		rootNode.UpdateWorldInfo();

		// Upload uniforms to GPU
		glUniformMatrix4fv( viewUniform,  1, GL_FALSE, &(*viewMat)[0][0] );
		glUniformMatrix4fv( projUniform,  1, GL_FALSE, &(*projectionMat)[0][0] );
		glUniform3fv( worldCenterUniform, 1, &worldCenter->GetLocation()[0] );
		glUniform3fv( lightDirectionUniform, 1, &glm::normalize( glm::vec3( 0.6, 1.0, 1.0 ) )[0] );


		// Let's test how rendering the same entity in multiple places works:
		auto defLoc = ground->GetLocation();

		// Render the current scene
		rootNode.Render();

		// Modify location of the ground entity and render the scene again
		ground->SetLocation( defLoc + glm::vec3( 0.f, 5.f, 40.f ) );
		rootNode.Render();

		// Set the ground back to it's default location
		ground->SetLocation( defLoc );


		glfwSwapBuffers( window );
		glfwPollEvents();
	}

    glDisableVertexAttribArray( shaderProgram->GetAttribute( "vertexNormal" ) );
    glDisableVertexAttribArray( shaderProgram->GetAttribute( "vertexPosition" ) );

	glfwDestroyWindow( window );
	glfwTerminate();
	return 0;
}

