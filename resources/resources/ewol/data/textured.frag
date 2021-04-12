#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Input :
uniform sampler2D in_texID;

varying vec2 io_texcoord;
varying vec4 io_color;

void main(void) {
	gl_FragColor = texture2D(in_texID, io_texcoord) * io_color;
}
