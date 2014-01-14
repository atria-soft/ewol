#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Input :
uniform sampler2D EW_texID;
uniform float     EW_SoftEdgeMin;
uniform float     EW_SoftEdgeMax;
uniform int       EW_SoftEdge;
uniform int       EW_texHeight;
uniform int       EW_texWidth;

varying vec2 f_texcoord;
varying vec4 f_color;
varying float f_onestepu;
varying float f_onestepv;

const float glyph_center   = 0.50;
const float outline_center = 0.55;
const float glow_center    = 1.25;

vec3 glyph_color    = vec3(1.0,1.0,1.0);
vec3 outline_color  = vec3(0.0,0.0,0.0);
vec3 glow_color     = vec3(0.0,1.0,0.0);

void main(void) {
	
	// Scale texcoords to range ([0,texw], [0,texh])
	vec2 uv = f_texcoord * vec2(EW_texWidth, EW_texHeight);
	
	// Compute texel-local (u,v) coordinates for the four closest texels
	vec2 uv00 = floor(uv - vec2(0.5)); // Lower left corner of lower left texel
	vec2 uvthis = floor(uv); // Lower left corner of texel containing (u,v)
	vec2 uvlerp = uv - uv00 - vec2(0.5); // Texel-local lerp blends [0,1]
	
	// Perform explicit texture interpolation of D coefficient.
	// This works around the currently very bad texture interpolation
	// precision in ATI hardware.
	
	// Center st00 on lower left texel and rescale to [0,1] for texture lookup
	vec2 st00 = (uv00  + vec2(0.5)) * vec2(f_onestepu, f_onestepv);
	
	// Compute g_u, g_v, D coefficients from four closest 8-bit RGBA texels
	vec4 rawtex00 = texture2D(EW_texID, st00);
	vec4 rawtex10 = texture2D(EW_texID, st00 + vec2(0.5*f_onestepu, 0.0));
	vec4 rawtex01 = texture2D(EW_texID, st00 + vec2(0.0, 0.5*f_onestepv));
	vec4 rawtex11 = texture2D(EW_texID, st00 + vec2(0.5*f_onestepu, 0.5*f_onestepv));
	
	// Restore the value for D from its 8-bit encoding
	vec2 D00_10 = 16.0*(vec2(rawtex00.r, rawtex10.r)-0.50196);
	vec2 D01_11 = 16.0*(vec2(rawtex01.r, rawtex11.r)-0.50196);
	
	// Interpolate D between four closest texels
	vec2 uvlocal = fract(uv)-0.5; // Texel-local uv coordinates [-0.5,0.5]
	// Interpolate along v
	vec2 D0_1 = mix(D00_10, D01_11, uvlerp.y);
	// Interpolate along u
	float D = mix(D0_1.x, D0_1.y, uvlerp.x);
	
	// Perform anisotropic analytic antialiasing (fwidth() is slightly wrong)
	float aastep = length(vec2(dFdx(D), dFdy(D)));
	
	// 'pattern' is 1 where D>0, 0 where D<0, with proper AA around D=0.
	float pattern = smoothstep(-aastep, aastep, D);
	
	// Final fragment color
	gl_FragColor = vec4(f_color[0], f_color[1], f_color[2], f_color[3]*pattern);
	
}
#if 0
// Distance map contour texturing, Stefan Gustavson 2009
// A re-implementation of Green's method, with an
// 8-bit distance map but explicit texel interpolation.
// This code is in the public domain.

uniform sampler2D disttexture, reftexture;
uniform float texw, texh;
varying float onestepu, onestepv;
varying vec2 st;

void main( void )
{
  // Scale texcoords to range ([0,texw], [0,texh])
  vec2 uv = st * vec2(texw, texh);

  // Compute texel-local (u,v) coordinates for the four closest texels
  vec2 uv00 = floor(uv - vec2(0.5)); // Lower left corner of lower left texel
  vec2 uvthis = floor(uv); // Lower left corner of texel containing (u,v)
  vec2 uvlerp = uv - uv00 - vec2(0.5); // Texel-local lerp blends [0,1]

  // Perform explicit texture interpolation of D coefficient.
  // This works around the currently very bad texture interpolation
  // precision in ATI hardware.

  // Center st00 on lower left texel and rescale to [0,1] for texture lookup
  vec2 st00 = (uv00  + vec2(0.5)) * vec2(onestepu, onestepv);

  // Compute g_u, g_v, D coefficients from four closest 8-bit RGBA texels
  vec4 rawtex00 = texture2D(disttexture, st00);
  vec4 rawtex10 = texture2D(disttexture, st00 + vec2(0.5*onestepu, 0.0));
  vec4 rawtex01 = texture2D(disttexture, st00 + vec2(0.0, 0.5*onestepv));
  vec4 rawtex11 = texture2D(disttexture, st00 + vec2(0.5*onestepu, 0.5*onestepv));

  // Restore the value for D from its 8-bit encoding
  vec2 D00_10 = 16.0*(vec2(rawtex00.r, rawtex10.r)-0.50196);
  vec2 D01_11 = 16.0*(vec2(rawtex01.r, rawtex11.r)-0.50196);

  // Interpolate D between four closest texels
  vec2 uvlocal = fract(uv)-0.5; // Texel-local uv coordinates [-0.5,0.5]
  // Interpolate along v
  vec2 D0_1 = mix(D00_10, D01_11, uvlerp.y);
  // Interpolate along u
  float D = mix(D0_1.x, D0_1.y, uvlerp.x);

  // Perform anisotropic analytic antialiasing (fwidth() is slightly wrong)
  float aastep = length(vec2(dFdx(D), dFdy(D)));
  
  // 'pattern' is 1 where D>0, 0 where D<0, with proper AA around D=0.
  float pattern = smoothstep(-aastep, aastep, D);

  // 'bitmap' is a regular grayscale texture with AA for comparison.
  vec2 uvoffset = uvthis - uv00; // 0 or 1 depending on (u,v) quadrant
  float bitmap = texture2D(reftexture, st00+uvoffset*vec2(onestepu, onestepv)).r;

  // Final fragment color
  gl_FragColor = vec4(bitmap, pattern, bitmap, 1.0);
}
#endif
