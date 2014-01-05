attribute vec3 vertexPosition;
attribute vec3 vertexNormal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 worldCenter;

varying vec3 normal;


/* Credit for this function goes to:
 * http://www.neilmendoza.com/glsl-rotation-about-an-arbitrary-axis/
 */
mat4 rotationMatrix(vec3 axis, float angle)
{
	axis = normalize(axis);
	float s = sin(angle);
	float c = cos(angle);
	float oc = 1.0 - c;

	return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
	            oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
	            oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
	            0.0,                                0.0,                                0.0,                                1.0);
}



void main()
{
	mat4 MVP = P * V * M;
	vec4 v = M * vec4( vertexPosition, 1 );

	float deltaZ = v.z - worldCenter.z;
	float deltaY = worldCenter.y + v.y;
	float rads = deltaZ/deltaY;

	mat4 worldSurfaceRotation = rotationMatrix( vec3( 1.0, 0.0, 0.0 ), rads );

	v = worldSurfaceRotation * v;

	gl_Position = P * V * v;
	normal = (M*vec4(vertexNormal,0) ).xyz;
}

