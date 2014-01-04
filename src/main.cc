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


	window = glfwCreateWindow( 640, 480, "1GAM14-1", NULL, NULL );
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

		rootNode.EraseChild( testNode );
	}

	// Test mesh generation from a surface
	Surface testSurface( 1, 5 );
	Mesh testSurfaceMesh = testSurface.GenerateMesh( 1, 0 );

	testSurfaceMesh.SetName( "TestMesh" );
	testSurfaceMesh.GenerateGLBuffers();

	rootNode.AddChild( std::shared_ptr<Node>( &testSurfaceMesh ) );

	/*
	glEnableVertexAttribArray(0);
	glBindBuffer( GL_ARRAY_BUFFER, testSurfaceMesh.vbo );
	glVertexAttribPointer(
	   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   3,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	   (void*)0            // array buffer offset
	);*/

	/* Main loop */
	while( !glfwWindowShouldClose( window ) )
	{
		float ratio;
		int width, height;

		glfwGetFramebufferSize( window, &width, &height );
		ratio = width / (float) height;

		glClear( GL_COLOR_BUFFER_BIT );
		//glRotatef( (float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f );

		rootNode.SetRotation( glm::normalize( rot * rootNode.GetRotation() ) );
		rootNode.UpdateWorldInfo();

		glBegin( GL_TRIANGLES );
			for( auto& ind : testSurfaceMesh.indexBuffer )
			{
				glm::vec3 vec =testSurfaceMesh.GetWorldRotation() * testSurfaceMesh.vertexBuffer[ind].vertex + testSurfaceMesh.GetWorldLocation();
				glVertex3f( vec.x, vec.y, vec.z );
			}
		glEnd();

		glfwSwapBuffers( window );
		glfwPollEvents();
	}

	glfwDestroyWindow( window );
	glfwTerminate();
	exit( EXIT_SUCCESS );
}

