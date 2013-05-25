/**
*******************************************************************************
* @file ewol EwolRendererGL.java
* @brief Java openGl interface code.
* @author Edouard DUPIN
* @date 20/04/2012
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

package org.ewol;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

// import the ewol package :
/* no need in same package... */
//import org.ewol.Ewol;

import static org.ewol.Ewol.EWOL;

/**
 * @brief Class : 
 *
 */
public class EwolRendererGL implements GLSurfaceView.Renderer {
	
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		
	EWOL.renderInit();
    }
	
    public void onSurfaceChanged(GL10 gl, int w, int h) {
	EWOL.renderResize(w, h);
    }
	
    public void onDrawFrame(GL10 gl) {
	EWOL.renderDraw();
    }
}
