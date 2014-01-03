#include "main.hh"


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


	window = glfwCreateWindow( 640, 480, "Simple example", NULL, NULL );
	if( !window )
	{
		glfwTerminate();
		std::cerr << "Couldn't create window\n";
		exit( EXIT_FAILURE );
	}

	glfwMakeContextCurrent( window );
	glfwSetKeyCallback( window, GLFWKeyCallback );

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

	ShaderProgram shaderProgram;
	shaderProgram.Load( vShader, fShader );

	glUseProgram( shaderProgram.Get() );


	Node rootNode( "RootNode" );
	rootNode.SetLocation( glm::vec3( 0, 0.5, 0.0 ) );

	glm::quat rot = glm::quat ( glm::vec3( 0.01, 0.02, 0.02) );

	// These are for testing the Node system
	{
		auto testNode = std::make_shared<Node>( "TestNode" );
		rootNode.AddChild( testNode );

		auto secNode = std::make_shared<Node>( "SecNode" );
		testNode->AddChild( secNode );

		rootNode.UpdateWorldInfo();

		rootNode.EraseChild( testNode );
	}

	// Test mesh generation from a surface
	Surface testSurface( 10, 5 );
	Mesh testSurfaceMesh = testSurface.GenerateMesh( 1, 0 );
	for( auto& ind : testSurfaceMesh.indexBuffer )
	{
		std::cout << "TESTSURFACE INDEX: " << ind << "\n";
	}

	/* Main loop */
	while( !glfwWindowShouldClose( window ) )
	{
		float ratio;
		int width, height;

		glfwGetFramebufferSize( window, &width, &height );
		ratio = width / (float) height;

		glViewport( 0, 0, width, height );
		glClear( GL_COLOR_BUFFER_BIT );
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glOrtho( -ratio, ratio, -1.f, 1.f, 1.f, -1.f );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glRotatef( (float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f );

		rootNode.SetRotation( glm::normalize( rot * rootNode.GetRotation() ) );

		glBegin( GL_TRIANGLES );
			glm::vec3 vec = glm::vec3( 1.f, 0.f, 0.f ) * rootNode.GetRotation() + rootNode.GetLocation();
			glVertex3f( vec.x, vec.y, vec.z );
			vec = glm::vec3( 0.6f, -0.4f, 0.f ) * rootNode.GetRotation() + rootNode.GetLocation();
			glVertex3f( vec.x, vec.y, vec.z );
			vec = glm::vec3( 0.f, 0.6f, 0.f ) * rootNode.GetRotation() + rootNode.GetLocation();
			glVertex3f( vec.x, vec.y, vec.z );
		glEnd();

		glfwSwapBuffers( window );
		glfwPollEvents();
	}

	glfwDestroyWindow( window );
	glfwTerminate();
	exit( EXIT_SUCCESS );
}

