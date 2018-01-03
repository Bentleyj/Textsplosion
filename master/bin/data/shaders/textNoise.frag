#version 120

uniform float brightnessModifier;
uniform vec4 col1, col2;
varying vec4 colB;

void main(){
	vec2 pos = gl_FragCoord.xy;
	vec4 colH = mix(col1, col2, pos.x / 1920);
	gl_FragColor = mix(colB, colH, brightnessModifier);//gl_Color;    
}