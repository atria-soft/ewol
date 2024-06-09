#version 400 core

#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

varying vec4 io_color;

void main(void) {
	gl_FragColor = io_color;
	//gl_FragColor = vec4(1.0,1.0,1.0,1.0);
}
