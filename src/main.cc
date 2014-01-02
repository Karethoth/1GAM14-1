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

	std::string fShaderSource = \
	"\n"
	"void main()\n"
	"{\n"
	"  gl_FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );\n"
	"}\n";

	Shader fShader( GL_FRAGMENT_SHADER );
	fShader.Load( fShaderSource );

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

		glBegin( GL_TRIANGLES );
			glColor3f( 1.f, 0.f, 0.f );
			glVertex3f( -0.6f, -0.4f, 0.f );
			glColor3f( 0.f, 1.f, 0.f );
			glVertex3f( 0.6f, -0.4f, 0.f );
			glColor3f( 0.f, 0.f, 1.f );
			glVertex3f( 0.f, 0.6f, 0.f );
		glEnd();

		glfwSwapBuffers( window );
		glfwPollEvents();
	}

	glfwDestroyWindow( window );
	glfwTerminate();
	exit( EXIT_SUCCESS );
}

