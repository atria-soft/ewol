/**
 *******************************************************************************
 * @file ewol/openGL/openGL.cpp
 * @brief ewol openGl abstarction (sources)
 * @author Edouard DUPIN
 * @date 19/09/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#include <ewol/Debug.h>
#include <ewol/openGl.h>


void glOrthoEwol(GLfloat left,
             GLfloat right,
             GLfloat bottom,
             GLfloat top,
             GLfloat nearVal,
             GLfloat farVal)
{
	GLfloat myMatrix[4*4];
	int iii;
	for(iii=0; iii<4*4 ; iii++) {
		myMatrix[iii] = 0;
	}
	myMatrix[0] = 2.0 / (right - left);
	myMatrix[5] = 2.0 / (top - bottom);
	myMatrix[10] = -2.0 / (farVal - nearVal);
#if 1
	myMatrix[3]  = -1*(right + left) / (right - left);
	myMatrix[7]  = -1*(top + bottom) / (top - bottom);
	myMatrix[11] = -1*(farVal + nearVal) / (farVal - nearVal);
#else
	// test if matrix is not corectly instanciate ...
	myMatrix[12]  = -1*(right + left) / (right - left);
	myMatrix[13]  = -1*(top + bottom) / (top - bottom);
	myMatrix[14] = -1*(farVal + nearVal) / (farVal - nearVal);
#endif
	myMatrix[15] = 1;

	glLoadMatrixf(myMatrix);


}

static void printGLString(const char *name, GLenum s)
{
	const char *v = (const char *) glGetString(s);
	EWOL_INFO("GL " << name << " = " << v);
}

static void checkGlError(const char* op)
{
	for (GLint error = glGetError(); error; error = glGetError()) {
		EWOL_INFO("after " << op << "() glError (" << error << ")");
	}
}

static const char gVertexShader[] = 
	"attribute vec4 vPosition;\n"
	"void main() {\n"
	"  gl_Position = vPosition;\n"
	"}\n";

static const char gFragmentShader[] = 
	/*"precision mediump float;\n"*/
	"void main() {\n"
	"  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
	"}\n";

#define LOG_OGL_INTERNAL_BUFFER_LEN    (8192)
static char l_bufferDisplayError[LOG_OGL_INTERNAL_BUFFER_LEN] = "";

GLuint loadShader(GLenum shaderType, const char* pSource)
{
	GLuint shader = glCreateShader(shaderType);
	if (!shader) {
		EWOL_ERROR("glCreateShader return error ...");
		checkGlError("glCreateShader");
	} else {
		glShaderSource(shader, 1, &pSource, NULL);
		glCompileShader(shader);
		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			GLint infoLen = 0;
			l_bufferDisplayError[0] = '\0';
			glGetShaderInfoLog(shader, LOG_OGL_INTERNAL_BUFFER_LEN, &infoLen, l_bufferDisplayError);
			const char * tmpShaderType = "GL_FRAGMENT_SHADER";
			if (shaderType == GL_VERTEX_SHADER){
				tmpShaderType = "GL_VERTEX_SHADER";
			}
			EWOL_ERROR("Could not compile \"" << tmpShaderType << "\": " << l_bufferDisplayError);
		}
	}
	return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource)
{
	EWOL_INFO("Create the VERTEX shader ...");
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
	if (!vertexShader) {
		EWOL_ERROR("VERTEX shader return error ...");
		return 0;
	}
	EWOL_INFO("Create the FRAGMENT shader ...");
	GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
	if (!pixelShader) {
		EWOL_ERROR("FRAGMENT shader return error ...");
		return 0;
	}
	
	EWOL_INFO("Create the Program ...");
	GLuint program = glCreateProgram();
	if (!program) {
		EWOL_ERROR("program creation return error ...");
	
	} else {
		glAttachShader(program, vertexShader);
		checkGlError("glAttachShader");
		glAttachShader(program, pixelShader);
		checkGlError("glAttachShader");
		glLinkProgram(program);
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE) {
			GLint bufLength = 0;
			l_bufferDisplayError[0] = '\0';
			glGetProgramInfoLog(program, LOG_OGL_INTERNAL_BUFFER_LEN, &bufLength, l_bufferDisplayError);
			EWOL_ERROR("Could not compile \"PROGRAM\": " << l_bufferDisplayError);
			glDeleteProgram(program);
			program = 0;
		}
	}
	return program;
}

GLuint gProgram;
GLuint gvPositionHandle;

bool TESTsetupGraphics(int w, int h)
{
	printGLString("Version", GL_VERSION);
	printGLString("Vendor", GL_VENDOR);
	printGLString("Renderer", GL_RENDERER);
	printGLString("Extensions", GL_EXTENSIONS);
	
	EWOL_INFO("setupGraphics("<< w << "," << h);
	gProgram = createProgram(gVertexShader, gFragmentShader);
	if (!gProgram) {
		EWOL_ERROR("Could not create program.");
		return false;
	}
	gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
	checkGlError("glGetAttribLocation");
	EWOL_INFO("glGetAttribLocation(\"vPosition\") = " << gvPositionHandle);
	
	glViewport(0, 0, w, h);
	checkGlError("glViewport");
	
	return true;
}
const GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
//const GLfloat gTriangleVertices[] = { 0.0f, 0.0f, 200.0f, 0.0f, 0.0f, 200.0f };

void TEST_renderFrame(void) {
	static float grey = 0.5;
	
	grey += 0.01f;
	if (grey > 1.0f) {
		grey = 0.0f;
	}
	glClearColor(grey, grey, grey, 1.0f);
	checkGlError("glClearColor");
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	checkGlError("glClear");
	
	glUseProgram(gProgram);
	checkGlError("glUseProgram");
	
	glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
	checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray(gvPositionHandle);
	checkGlError("glEnableVertexAttribArray");
	glDrawArrays(GL_TRIANGLES, 0, 3);
	checkGlError("glDrawArrays");
}