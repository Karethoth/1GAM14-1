varying vec3 normal;

uniform vec3 lightDirection;

void main()
{
  vec3 color = vec3( 1.0, 0.0, 0.0 );

  vec3 norm = normalize( normal );
  vec3 lightDir = normalize( lightDirection );
  float cosTheta = clamp( dot( norm, lightDir  ), 0.1, 0.9 );

  cosTheta = cosTheta + 0.1;

  gl_FragColor = vec4( color*cosTheta, 1 );
}

