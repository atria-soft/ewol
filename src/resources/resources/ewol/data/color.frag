#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

varying vec4 io_color;

void main(void) {
  gl_FragColor = io_color;
}
