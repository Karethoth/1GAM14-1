#include "main.hh"
#include "../deb/glm/gtc/matrix_transform.hpp"
#include "../deb/glm/gtx/quaternion.hpp"


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


	// Construct list of attributes and their locations
	std::map<std::string, GLuint> attributes;
	attributes["vertexPosition"] = 0;

	// Create the shader and pass the shaders and the attribute list to it.
	ShaderProgram shaderProgram;
	shaderProgram.Load( vShader, fShader, attributes );


	// Get MVP uniform location
	const GLint mvpUniform = shaderProgram.GetUniform( "MVP" );


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

	glEnableVertexAttribArray( shaderProgram.GetAttribute( "vertexPosition" ) );
	glBindBuffer( GL_ARRAY_BUFFER, testSurfaceMesh.vbo );

	glVertexAttribPointer(
	   shaderProgram.GetAttribute( "vertexPosition" ),
	   3,
	   GL_FLOAT,
	   GL_FALSE,
	   sizeof( VBOData ),
	   (void*)0
	);


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
		glm::mat4 projectionMat = glm::perspective( 60.0f, 4.0f / 3.0f, 0.1f, 100.0f );

		glm::mat4 viewMat = glm::lookAt(
			glm::vec3(0,0,-10),
			glm::vec3(0,0,0),
			glm::vec3(0,1,0)
		);

		// Rotate the world, because that's how stuff works
		rootNode.SetRotation( glm::normalize( rot * rootNode.GetRotation() ) );
		rootNode.UpdateWorldInfo();

		// Calculate Model matrix and use that to calculate the final MVP matrix
		glm::mat4 modelMat = glm::mat4( 1.0f );
		modelMat = modelMat * glm::toMat4(  testSurfaceMesh.GetWorldRotation() );
		modelMat = glm::translate(  modelMat, testSurfaceMesh.GetWorldLocation() );
		glm::mat4 mvpMat   = projectionMat * viewMat * modelMat;

		// Use the vertex array object of the mesh
		glBindVertexArray( testSurfaceMesh.vao );
		glUseProgram( shaderProgram.Get() );

		// Upload the MVP matrix to GPU
		glUniformMatrix4fv( mvpUniform, 1, GL_FALSE, &mvpMat[0][0] );

		glDrawElements(
			GL_TRIANGLES,
			testSurfaceMesh.indexBuffer.size(),
			GL_UNSIGNED_INT,
			(void*)0
        );

		glfwSwapBuffers( window );
		glfwPollEvents();
	}

    glDisableVertexAttribArray( shaderProgram.GetAttribute( "vertexPosition" ) );

	glfwDestroyWindow( window );
	glfwTerminate();
	exit( EXIT_SUCCESS );
}

