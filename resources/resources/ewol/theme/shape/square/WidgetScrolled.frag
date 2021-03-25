#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// transmit from the vertex shader
varying vec2 v_position; // interpolated position ...
varying vec2 v_propPos;
varying vec4 v_colorTansition;
varying vec4 v_colorBorder;
varying vec4 v_colorBackground;

void main(void) {
	// prevent origin moving ... 
	gl_FragColor = vec4(v_propPos.y, v_propPos.x, 1.0, 1.0);
	if(    v_propPos.x == 1.0
	    && v_propPos.y == 1.0) {
		gl_FragColor = v_colorTansition;
	} else if (    v_propPos.x == 0.0
	            || v_propPos.y == 0.0) {
		gl_FragColor = v_colorBackground;
	} else {
		gl_FragColor = v_colorBorder;
	}
}
