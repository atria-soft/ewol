#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform float  EW_sizeBorder;
uniform float  EW_sizePadding;
uniform vec2   EW_size;
uniform vec4   EW_posText;
uniform int    EW_state;
/*
struct displayProperty {
	vec4  colorBackGround;
	vec4  colorForeGround;
	vec4  colorBorder;
	float sizeBorder;
	vec2  sizePaddingOut;
	vec2  sizePaddingIn;
	
};
uniform displayProperty EW_buttonProperty;
*/
// transmit from the vertex shader
varying vec2  v_position; // interpolated position ...
// internal static define
vec4  S_colorBg = vec4(0.0);
vec4  S_colorFg = vec4(0.5,0.5,0.5,0.8);
vec4  S_colorBorder = vec4(0.0,0.0,0.0,1.0);


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
			gl_FragColor = S_colorBorder;
		} else {
			gl_FragColor = S_colorFg;
		}
	} else {
		gl_FragColor = S_colorBg;
	}
}

