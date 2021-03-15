#ifdef GL_ES
#extension GL_OES_standard_derivatives : enable
precision mediump float;
precision mediump int;
#endif

// Input :
uniform sampler2D EW_texID;
uniform float     EW_SoftEdgeMin;
uniform float     EW_SoftEdgeMax;
uniform int       EW_SoftEdge;

varying vec2 f_texcoord;
varying vec4 f_color;


void main(void) {
	vec4 color = texture2D(EW_texID, f_texcoord );
	float dist  = color.r;
	float width = fwidth(dist);
	float alpha = smoothstep(0.5-width, 0.5+width, dist);
	
	// Smooth
	gl_FragColor = vec4(f_color[0], f_color[1], f_color[2], f_color[3]*alpha);
}
