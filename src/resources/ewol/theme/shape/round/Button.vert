#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

struct widgetStateProperty {
	int   stateOld;
	int   stateNew;
	float transition;
};

uniform widgetStateProperty EW_status;

// Input :
attribute vec2 EW_coord2d;
attribute vec2 EW_widgetPropertyPos;
uniform mat4   EW_MatrixTransformation;
uniform vec4   EW_foreground;
uniform vec4   EW_foregroundHover;
uniform vec4   EW_foregroundSelected;
uniform vec4   EW_foregroundPressed;

// output :
varying vec2  v_propPos;
varying vec4  v_colorTansition;

void main(void) {
	gl_Position = EW_MatrixTransformation * vec4(EW_coord2d, 0.0, 1.0);
	v_propPos = EW_widgetPropertyPos;
	
	vec4 colorOld = EW_foreground;
	if(EW_status.stateOld == 1) {
		colorOld = EW_foregroundPressed;
	} else if(EW_status.stateOld == 2) {
		colorOld = EW_foregroundHover;
	} else if(EW_status.stateOld == 3) {
		colorOld = EW_foregroundSelected;
	}
	vec4 colorNew = EW_foreground;
	if(EW_status.stateNew == 1) {
		colorNew = EW_foregroundPressed;
	} else if(EW_status.stateNew == 2) {
		colorNew = EW_foregroundHover;
	} else if(EW_status.stateNew == 3) {
		colorNew = EW_foregroundSelected;
	}
	
	// note : int() is needed for the OpenGL ES platform
	v_colorTansition =   colorOld * (1.0 - EW_status.transition)
	                   + colorNew * EW_status.transition;
}
