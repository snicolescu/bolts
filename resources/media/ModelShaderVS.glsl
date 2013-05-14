uniform mat4 u_wvp;
uniform mat4 u_w;

attribute vec3 a_posL;
attribute vec3 a_normL;
attribute vec2 a_uv;

varying vec3 v_normW;
varying vec2 v_uv;

void main()
{
	gl_Position = u_wvp * vec4(a_posL, 1.0);
	v_normW = normalize( (u_w * vec4(a_normL,0.0)).xyz );
	v_uv = a_uv;
}
   