uniform sampler2D textureSampler;
uniform float alpha = 1.f;
varying vec2 textureCoord;


void main()
{
	vec4 color = texture2D( textureSampler, textureCoord );
	gl_FragColor = vec4( color.rgb, color.a * alpha );
}

