precision mediump float;
const float c_ambFactor = 0.3;
const float c_omambFactor = 0.7;

uniform vec3 u_lightDir;
uniform vec3 u_ambientColor;
uniform sampler2D u_diffTex;

varying vec3 v_normW;
varying vec2 v_uv;

void main()
{
	vec3 norm = normalize(v_normW);
	float diff = max( dot( -norm, u_lightDir), 0.0)* c_omambFactor;
	gl_FragColor = vec4( (u_ambientColor*c_ambFactor + diff) * texture2D( u_diffTex, v_uv).xyz, 1.0);
}
