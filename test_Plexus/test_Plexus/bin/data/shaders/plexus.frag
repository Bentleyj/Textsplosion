#version 120
#define LINEWIDTH 0.003
#define NUM_POINTS 80

uniform vec2 u_Resolution;
uniform vec2 u_PointA;

uniform vec2 u_Points[NUM_POINTS];

float distToLine(vec2 pt1, vec2 pt2, vec2 testPt) {
	vec2 lineDir = pt2 - pt1;
	vec2 perpDir = vec2(lineDir.y, -lineDir.x);
	vec2 dirToPt1 = pt1 - testPt;

	float a = abs(distance(pt1, testPt));
	float b = abs(distance(pt2, testPt));
	float c = abs(distance(pt1, pt2));

	float agtc = sign(a - c);
	float bgtc = sign(b - c);

	// a >= c || b >= c
	float comp = clamp(max(agtc, bgtc), 0.0, 1.0);

	return mix(abs(dot(normalize(perpDir), dirToPt1)), 1000.0, comp);
}

float inLine(vec2 testPoint, vec2 pointA, vec2 pointB) {
	return LINEWIDTH - distToLine(pointA, pointB, testPoint);
}

void main() {
	vec2 pos = gl_FragCoord.xy;

	pos /= u_Resolution;
	vec2 pointA = u_PointA / u_Resolution;
	//vec2 pointB = u_PointB / u_Resolution;

	float brightness = 0.0;
	for (int i = 0; i < NUM_POINTS; i++) {
		vec2 pointI = u_Points[i] / u_Resolution;
		for (int j = 0; j < NUM_POINTS; j++) {
			vec2 pointJ = u_Points[j] / u_Resolution;
			float dist = distance(pointI, pointJ);
			brightness += clamp(inLine(pos, pointI/*u_Points[i]*/, pointJ) * 100, 0.0, 1.0) / (dist * dist * 100);
		}
	}

	//for (int i = 0; i < NUM_POINTS; i++) {
	//	vec2 point = u_Points[i] / u_Resolution;
	//	//if (distance(pos, point) < 0.01)
	//	//	brightness = 1.0;
	//}

	gl_FragColor = vec4(brightness, 0.0, brightness, 1.0);
}