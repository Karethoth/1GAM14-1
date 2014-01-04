attribute vec3 vertexPosition;
attribute vec3 vertexNormal;

uniform mat4 MVP;

varying vec3 normal;

void main()
{
    vec4 v = vec4( vertexPosition, 1 );
    gl_Position = MVP * v;
	normal = ( MVP * vec4(vertexNormal,1)).xyz;
}

