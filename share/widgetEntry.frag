#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform float  EW_sizeBorder;
uniform float  EW_sizePadding;
uniform vec2   EW_size;
uniform vec4   EW_posText;
uniform int    EW_state;

// transmit from the vertex shader
varying vec2  v_position; // intermolated position ...

void main(void) {
	float specialBorder = EW_sizeBorder+EW_sizePadding;
	vec2  endStart = EW_size - vec2(EW_sizePadding) - vec2(EW_sizeBorder);
	vec2  endStop  = EW_size - vec2(EW_sizePadding);
	if(    v_position.x>  EW_sizePadding
	    && v_position.y>  EW_sizePadding
	    && v_position.x<= endStop.x
	    && v_position.y<= endStop.y
	  ) {
		if(    v_position.x<= specialBorder
		    || v_position.y<= specialBorder
		    || v_position.x>  endStart.x
		    || v_position.y>  endStart.y
		  ) {
			gl_FragColor = vec4(0.0,0.0,0.0,1.0);
		} else {
			gl_FragColor = vec4(1.0,1.0,1.0,0.8);
		}
	} else {
		gl_FragColor = vec4(0.0);
	}
}
