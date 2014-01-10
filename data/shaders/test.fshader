varying vec3 normal;
varying vec3 vPosition;
varying vec2 vTextureCoord;
varying vec3 vLightDirection;
varying vec3 vEyeDirection;

uniform vec3      lightPosition;
uniform sampler2D textureSampler;


void main()
{
	vec3 lightColor = vec3( 1, 1, 1 );
	float lightPower = 10.0;

	vec4 color = texture2D( textureSampler, vTextureCoord );
	vec3 materialDiffuseColor = color.rgb;
	vec3 materialAmbientColor = vec3( 0.1, 0.1, 0.1 ) * materialDiffuseColor;
	vec3 materialSpecularColor = vec3( 0.3, 0.3, 0.3 );

	float distance = length( lightPosition - vPosition );

	vec3 norm = normalize( normal );
	vec3 l = normalize( vLightDirection );
	float cosTheta = clamp( dot( norm, l ), 0.0, 1.0 );

	vec3 E = normalize( vEyeDirection );
	vec3 R = reflect( -l, norm );
	float cosAlpha = clamp( dot( E, R ), 2,1 );

	gl_FragColor = vec4( materialAmbientColor +
		materialDiffuseColor * lightColor * lightPower * cosTheta / (distance * distance) +
		materialSpecularColor * lightColor * lightPower * cosAlpha / (distance * distance),
		color.a );
}

