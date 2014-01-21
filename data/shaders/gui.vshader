attribute vec2 vertexPosition;

uniform mat4 M;
uniform mat4 P;

varying vec2 textureCoord;


void main()
{
	textureCoord = gl_MultiTexCoord0.xy;
	gl_Position = P * M * vec4( vertexPosition, 0.0, 1.0 );
}

