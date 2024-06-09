#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform vec4 EW_background;
uniform vec4 EW_foreground;
uniform vec4 EW_border;

// transmit from the vertex shader
varying vec2  v_position; // interpolated position ...
varying vec2  v_propPos;

void main(void) {
	// prevent origin moving ... 
	gl_FragColor = vec4(v_propPos.y, v_propPos.x, 1.0, 1.0);
	if(    v_propPos.x == 1.0
	    && v_propPos.y == 1.0) {
		gl_FragColor = EW_foreground;
	} else if (    v_propPos.x == 0.0
	            || v_propPos.y == 0.0) {
		gl_FragColor = EW_background;
	} else {
		gl_FragColor = EW_border;
	}
}

