varying vec3 normal;
varying vec2 vTextureCoord;

uniform vec3 lightDirection;
uniform sampler2D textureSampler;

void main()
{
  vec4 color = texture2D( textureSampler, vTextureCoord );

  vec3 norm = normalize( normal );
  vec3 lightDir = normalize( lightDirection );
  float cosTheta = clamp( dot( norm, lightDir  ), 0.6, 1.0 );

  gl_FragColor = vec4( color.rgb * cosTheta, color.a );
}

