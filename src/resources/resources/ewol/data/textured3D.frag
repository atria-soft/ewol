#version 400 core

#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Input :
uniform sampler2D in_texID;

varying vec2 io_texcoord;
varying vec4 io_color;

void main(void) {
	vec4 map = texture2D(in_texID, io_texcoord);
	gl_FragColor = map * io_color;
	//gl_FragColor = vec4(1.0,1.0,0.2,0.6);
}
