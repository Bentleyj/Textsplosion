#version 120

varying vec3 N;
varying vec3 v;

uniform vec3 lightSourcePosition;
uniform vec4 diffuse;


void main(){

	vec3 L = normalize(lightSourcePosition - v);
	vec4 Idiff = diffuse * max(dot(N, L), 0.0);
	Idiff = clamp(Idiff, 0.1, 1.0);
	gl_FragColor = vec4(Idiff.xyz, 1.0);//gl_Color;    
}