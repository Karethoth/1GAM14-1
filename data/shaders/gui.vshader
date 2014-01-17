attribute vec2 vertexPosition;
attribute vec2 textureCoord;
attribute vec3 vertexNormal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

varying vec2 vTextureCoord;


void main()
{
	gl_Position = P * M * vec4( vertexPosition, 0.0, 1.0 );
	vTextureCoord = textureCoord;
}

