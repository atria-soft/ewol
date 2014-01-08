#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Input :
attribute vec3 EW_coord3d;
attribute vec2 EW_texture2d;
attribute vec4 EW_color;
uniform mat4 EW_MatrixTransformation;

// output :
varying vec4 f_color;
varying vec2 f_texcoord;

void main(void) {
	gl_Position = EW_MatrixTransformation * vec4(EW_coord3d, 1.0);
	// set texture output coord
	f_texcoord = EW_texture2d;
	// set output color :
	f_color = EW_color;
}


/*
// Distance map contour texturing according to Green (2007),
// implementation by Stefan Gustavson 2009.
// This code is in the public domain.

uniform sampler2D gradtexture, reftexture;
uniform float texw, texh;
varying float onestepu, onestepv;
varying vec2 st;

void main( void )
{
  // Get the texture coordinates
  st = gl_MultiTexCoord0.xy;
  onestepu = 1.0 / texw; // Saves a division in the fragment shader
  onestepv = 1.0 / texh;
  gl_Position = ftransform();
}
*/
