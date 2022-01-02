#version 400 core

#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Input:
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_textureCoords;

uniform vec3 in_offsetScaleInside;
uniform vec3 in_offsetScaleOutside;

uniform mat4 in_matrixTransformation;
uniform mat4 in_matrixProjection;
uniform mat4 in_matrixView;

// output:
out vec2 io_textureCoords;

void main(void) {
	float xxx = in_position.x;
	if (abs(xxx) < 10.0) {
		xxx = xxx * in_offsetScaleInside.x * 0.1;
	} else {
		xxx = xxx + sign(xxx)*in_offsetScaleOutside.x;
	}
	float yyy = in_position.y;
	if (abs(yyy) < 10.0) {
		yyy = yyy * in_offsetScaleInside.y * 0.1;
	} else {
		yyy = yyy + sign(yyy)*in_offsetScaleOutside.y;
	}
	float zzz = in_position.z;
	if (abs(zzz) < 10.0) {
		zzz = zzz * in_offsetScaleInside.z * 0.1;
	} else {
		zzz = zzz + sign(zzz)*in_offsetScaleOutside.z;
	}
	vec4 position = vec4(xxx, yyy, zzz, 1.0);
	/*
	 // this is the old mode before checkbox : ==> maybe create 2 shader????
	vec4 position = vec4(in_position.x + sign(in_position.x)*in_offsetScaleOutside.x,
	                     in_position.y + sign(in_position.y)*in_offsetScaleOutside.y,
	                     in_position.z + sign(in_position.z)*in_offsetScaleOutside.z, 1.0);
	*/
	gl_Position = in_matrixProjection * in_matrixView * in_matrixTransformation * position;
	io_textureCoords = in_textureCoords;
}
