#version 120
#extension GL_EXT_gpu_shader4 : enable

varying vec3 N;

varying vec3 v;

void main(){
	// get original vertex postion

	v = vec3(gl_ModelViewMatrix * gl_Vertex);
	N = normalize(gl_NormalMatrix * gl_Normal);
	//gl_FrontColor = gl_Color;//vec4((gl_Color * percentColor + newCol * (1 - percentColor)).xyz, 0.5 + percentColor);///*gl_Color*/(colorClose * percentColor + colorFar * (1-percentColor));
    //}

		vec4 center = vec4(0, 0, 0, 0);
		vec4 pos = gl_Vertex;

		vec4 dir = normalize(pos - center);

		pos.x += pos.y;

	gl_Position = gl_ModelViewProjectionMatrix * pos;
	
}
