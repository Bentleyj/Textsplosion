#version 120

uniform float u_Amount;
uniform sampler2DRect u_Tex;

void main(){
	vec2 pos = gl_TexCoord[0].xy;
	vec4 col = texture2DRect(u_Tex, pos);//gl_Color;
	col.a += u_Amount;
	gl_FragColor = col;    
}