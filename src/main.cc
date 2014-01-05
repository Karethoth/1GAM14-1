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
	attributes["vertexNormal"]   = 1;

	// Create the shader and pass the shaders and the attribute list to it.
	ShaderProgram shaderProgram;
	shaderProgram.Load( vShader, fShader, attributes );


	// Get uniform locations
	const GLint modelUniform          = shaderProgram.GetUniform( "M" );
	const GLint viewUniform           = shaderProgram.GetUniform( "V" );
	const GLint projUniform           = shaderProgram.GetUniform( "P" );
	const GLint worldCenterUniform    = shaderProgram.GetUniform( "worldCenter" );
	const GLint lightDirectionUniform = shaderProgram.GetUniform( "lightDirection" );


	Node rootNode( "RootNode" );
	rootNode.SetLocation( glm::vec3( 0, 0.0, 0.0 ) );

	glm::quat rot = glm::quat ( glm::vec3( 0.f, 0.05*glfwGetTime(), 0.f) );


	// Generate Node that acts as the "center" of the world.
	// (vertices that are affected by it, rotate around it)
	auto worldCenter = std::make_shared<Node>( "WorldCenter" );
	worldCenter->SetLocation( glm::vec3( 0.0, -50.0, 0.0 ) );
	rootNode.AddChild( worldCenter );

	// Test mesh generation from a surface
	Surface testSurface( 20, 40 );
	auto testSurfaceMesh = testSurface.GenerateMesh( 5, 5 );

	testSurfaceMesh->SetName( "TestMesh" );
	testSurfaceMesh->GenerateGLBuffers();
	rootNode.AddChild( testSurfaceMesh );

	testSurfaceMesh->SetLocation( glm::vec3( -10.0, 0.0, -20.0 ) );

	glEnableVertexAttribArray( shaderProgram.GetAttribute( "vertexPosition" ) );
	glEnableVertexAttribArray( shaderProgram.GetAttribute( "vertexNormal" ) );
	glBindBuffer( GL_ARRAY_BUFFER, testSurfaceMesh->vbo );

	// Vertex position
	glVertexAttribPointer(
	   shaderProgram.GetAttribute( "vertexPosition" ),
	   3,
	   GL_FLOAT,
	   GL_FALSE,
	   sizeof( VBOData ),
	   (void*)0
	);

	// Vertex normal
	glVertexAttribPointer(
	   shaderProgram.GetAttribute( "vertexNormal" ),
	   3,
	   GL_FLOAT,
	   GL_FALSE,
	   sizeof( VBOData ),
	   (void*)3
	);

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glLineWidth( 5.0 );

	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);


	/* Main loop */
	while( !glfwWindowShouldClose( window ) )
	{
		float ratio;
		int width, height;

		glfwGetFramebufferSize( window, &width, &height );
		ratio = width / (float) height;

		glClear( GL_COLOR_BUFFER_BIT );

		// Matrix stuff
		glm::mat4 projectionMat = glm::perspective( 60.0f, 4.0f / 3.0f, 0.1f, 100.0f );

		glm::mat4 viewMat = glm::lookAt(
			glm::vec3( 0, 5,-15),
			glm::vec3( 0, -4, 0 ),
			glm::vec3( 0, 1, 0)
		);


		testSurfaceMesh->SetRotation( glm::normalize( rot * testSurfaceMesh->GetRotation() ) );
		rootNode.UpdateWorldInfo();

		// Calculate Model matrix and use that to calculate the final MVP matrix
		glm::mat4 modelMat = glm::mat4( 1.0f );
		modelMat = modelMat * glm::toMat4(  testSurfaceMesh->GetWorldRotation() );
		modelMat = glm::translate(  modelMat, testSurfaceMesh->GetWorldLocation() );

		// Use the vertex array object of the mesh
		glBindVertexArray( testSurfaceMesh->vao );
		glUseProgram( shaderProgram.Get() );

		// Upload uniforms to GPU
		glUniformMatrix4fv( modelUniform, 1, GL_FALSE, &modelMat[0][0] );
		glUniformMatrix4fv( viewUniform,  1, GL_FALSE, &viewMat[0][0] );
		glUniformMatrix4fv( projUniform,  1, GL_FALSE, &projectionMat[0][0] );
		glUniform3fv( worldCenterUniform, 1, &worldCenter->GetLocation()[0] );
		glUniform3f( lightDirectionUniform, 1.0, 1.0, 0.0 );

		glDrawElements(
			GL_TRIANGLES,
			testSurfaceMesh->indexBuffer.size(),
			GL_UNSIGNED_INT,
			(void*)0
        );

		glfwSwapBuffers( window );
		glfwPollEvents();
	}

    glDisableVertexAttribArray( shaderProgram.GetAttribute( "vertexNormal" ) );
    glDisableVertexAttribArray( shaderProgram.GetAttribute( "vertexPosition" ) );

	glfwDestroyWindow( window );
	glfwTerminate();
	return 0;
}

