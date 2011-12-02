
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "importgl.h"

#include "app.h"


// Called from the app framework.
void appInit()
{
	// initialisation of the application : 
	// glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);

}


// Called from the app framework.
void appDeinit()
{
	// close the application ... 
}



#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "====> EWOL", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "====> EWOL", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "====> EWOL", __VA_ARGS__))

void glOrtho(GLfloat left,
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



GLfloat gTriangleVertices[] = { 0.0f, 0.0f, 200.0f, 0.0f, 0.0f, 200.0f };
GLfloat gTriangleVertices5[] = { 200.0f, 200.0f, 100.0f, 200.0f, 200.0f, 100.0f,
                                 200.0f, 200.0f, 300.0f, 200.0f, 200.0f, 300.0f };

void appMove(double x, double y)
{
	gTriangleVertices5[0] = x;
	gTriangleVertices5[1] = y;
	gTriangleVertices5[2] = x - 100.0f;
	gTriangleVertices5[3] = y;
	gTriangleVertices5[4] = x;
	gTriangleVertices5[5] = y - 100.0f;
	gTriangleVertices5[6] = x;
	gTriangleVertices5[7] = y;
	gTriangleVertices5[8] = x + 100.0f;
	gTriangleVertices5[9] = y;
	gTriangleVertices5[10] = x;
	gTriangleVertices5[11] = y + 100.0f;
	//LOGI("move To ... (%f,%f)",x,y);
}


void appRender(long timestamp, int width, int height)
{

	if (!gAppAlive) {
		return;
	}
	// to terminate application : 
	// set : gAppAlive = true; and return ...

	//EWOL_DEBUG("Drow on (" << m_size.x << "," << m_size.y << ")");
	// set the size of the open GL system
	glViewport(0,0,width,height);
	
	// Clear the screen with transparency ...
	glClearColor(0.0,0.0,0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(0., width, 0., -height, 1., 20.);
	glOrtho(-width/2, width/2, height/2, -height/2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//glTranslatef(0, -height/2, -5);
	glTranslatef(-width/2, -height/2, -1.0);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);
	
	glEnableClientState( GL_VERTEX_ARRAY );
	
	//LOGI("engine_draw_frame (%d,%d)",width,height);
	
	glColor4f(0.0, 1.0, 1.0, 1.0);
	glVertexPointer(2, GL_FLOAT, 0, gTriangleVertices5 );
	glDrawArrays( GL_TRIANGLES, 0, 6);
	static int vallllll = 0;
	static float transparency = 0.0;
	if (vallllll <= 1) {
		transparency +=0.025;
		if (transparency >= 1.0) {
			vallllll++;
			transparency = 0.0;
			glColor4f(1.0, 0.0, 0.0, 1.0);
		} else {
			glColor4f(1.0, 0.0, 0.0, transparency);
		}
	} else if (vallllll <= 2) {
		transparency +=0.025;
		if (transparency >= 1.0) {
			vallllll++;
			transparency = 0.0;
			glColor4f(1.0, 1.0, 0.0, 1.0);
		} else {
			glColor4f(1.0, 1.0, 0.0, transparency);
		}
	} else if (vallllll <= 3) {
		transparency +=0.025;
		if (transparency >= 1.0) {
			vallllll++;
			transparency = 0.0;
			glColor4f(0.0, 1.0, 0.0, 1.0);
		} else {
			glColor4f(0.0, 1.0, 0.0, transparency);
		}
	} else if (vallllll <= 4) {
		transparency +=0.025;
		if (transparency >= 1.0) {
			vallllll++;
			transparency = 0.0;
			glColor4f(0.0, 1.0, 1.0, 1.0);
		} else {
			glColor4f(0.0, 1.0, 1.0, transparency);
		}
	} else if (vallllll <= 5) {
		transparency +=0.025;
		if (transparency >= 1.0) {
			vallllll++;
			transparency = 0.0;
			glColor4f(0.0, 0.0, 1.0, 1.0);
		} else {
			glColor4f(0.0, 0.0, 1.0, transparency);
		}
	} else {
		transparency +=0.025;
		if (transparency >= 1.0) {
			vallllll = 0;
			transparency = 0.0;
			glColor4f(1.0, 0.0, 1.0, 1.0);
		} else {
			glColor4f(1.0, 0.0, 1.0, transparency);
		}
	}
	glVertexPointer(2, GL_FLOAT, 0, gTriangleVertices );
	glDrawArrays( GL_TRIANGLES, 0, 3);
	
	glDisableClientState( GL_VERTEX_ARRAY );

	glDisable(GL_BLEND);
	glFlush();
}
