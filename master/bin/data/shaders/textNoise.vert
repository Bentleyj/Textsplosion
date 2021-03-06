#version 120
#extension GL_EXT_gpu_shader4 : enable

uniform float timeVal = 1.0;
uniform float distortAmount = 100.0;
uniform vec3 camPosition;// = vec3(0.0, 0.0, 1.0);
uniform vec3 center = vec3(0.0, 0.0, 0.0);
uniform float brightnessModifier = 0.0;
uniform vec4 bcol1;
uniform vec4 bcol2;
uniform vec4 col1;
uniform vec4 col2;
uniform vec4 boundingBox;

//
// Description : Array and textureless GLSL 2D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : stegu
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
//               https://github.com/stegu/webgl-noise
// 

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
  {
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}

float map(float input_value, float input_start, float input_end, float output_start, float output_end) {
	return output_start + ((output_end - output_start) / (input_end - input_start)) * (input_value - input_start);
}



void main(){
	// get original vertex postion
	vec4 pos = gl_Vertex;
    
	float distanceToTarget = length(camPosition - center);

	float noiseAmntZ;

	float cappedDistortAmount = min(distortAmount, distanceToTarget);

    int numPointsPerPoly = 3; //  Change this to 2 for outline and 3 for filled.
	if (gl_VertexID % numPointsPerPoly == 0) {
		noiseAmntZ = snoise(vec2(timeVal + gl_VertexID, 1.0f)) * cappedDistortAmount;
	}
	else if (gl_VertexID % numPointsPerPoly == 1) {
		noiseAmntZ = snoise(vec2(timeVal + gl_VertexID - 1, 1.0f)) * cappedDistortAmount;
	}
	else {
		noiseAmntZ = snoise(vec2(timeVal + gl_VertexID - 2, 1.0f)) * cappedDistortAmount;
	} // Comment this out for lines
    
    

	pos.z += noiseAmntZ;//cos(pos.x * timeVal);//noiseAmntZ;

	float distanceToObject = distanceToTarget - noiseAmntZ;

	float scale = distanceToObject / distanceToTarget;

	pos.x *= scale;
	pos.y *= scale;

	float distToVertexFromCenter = length(pos.xyz - center);

	float normalizedDist =  map(distToVertexFromCenter, 0.0, 300.0/*distanceToTarget*/, 0.0, 1.0);
    
	pos = gl_ProjectionMatrix * gl_ModelViewMatrix * pos;
    
    vec4 hCol = mix(col2, col1, map(pos.x, boundingBox.x, boundingBox.z, 0.0, 1.0));


	vec4 bCol = mix(bcol2, bcol1, normalizedDist);
    
    if (gl_VertexID % 30 == 0 || gl_VertexID % 30 == 1 || gl_VertexID % 30 == 2) {
        bCol = vec4(250.0/255.0, 194.0/255.0, 44.0/255.0, 255);
    }
    
    vec4 finalCol = mix(bCol, hCol, brightnessModifier);

	gl_FrontColor = vec4(finalCol.xyz, 1.0);
	
	gl_Position = pos;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
}
