#include "main.hh"
#include "../deb/glm/gtc/matrix_transform.hpp"


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

	// Get MVP uniform location
	const GLint mvpUniform = shaderProgram.GetUniform( "MVP" );

	// Use dat program
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

		// Matrix stuff
		glm::mat4 Projection = glm::perspective( 60.0f, 4.0f / 3.0f, 0.1f, 100.0f );

		glm::mat4 View = glm::lookAt(
			glm::vec3(0,0,-10),
			glm::vec3(0,0,0),
			glm::vec3(0,1,0)
		);

		// Rotate the world, because that's how stuff works
		rootNode.SetRotation( glm::normalize( rot * rootNode.GetRotation() ) );
		rootNode.UpdateWorldInfo();

		// Calculate Model matrix and use that to calculate the final MVP matrix
		glm::mat4 Model = glm::translate( glm::mat4( 1.0f ), testSurfaceMesh.GetWorldLocation() );
		glm::mat4 MVP        = Projection * View * Model;

		// Upload the MVP matrix to GPU
		glUniformMatrix4fv( mvpUniform, 1, GL_FALSE, &MVP[0][0] );

		// Draw the triangles, in old fashion for now
		glBegin( GL_TRIANGLES );
			for( auto& ind : testSurfaceMesh.indexBuffer )
			{
				glm::vec3 vec =testSurfaceMesh.GetWorldRotation() * testSurfaceMesh.vertexBuffer[ind].vertex;
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

