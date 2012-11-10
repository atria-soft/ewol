/**
 *******************************************************************************
 * @file ewol interfaceOpenGL.java
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
//import javax.microedition.khronos.opengl.GLES20;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

// inport the ewol package :
import org.ewol.interfaceJNI;


/**
 * @brief Class : 
 *
 */
public class interfaceOpenGL implements GLSurfaceView.Renderer {
	
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		
		interfaceJNI.RenderInit();
	}
	
	public void onSurfaceChanged(GL10 gl, int w, int h) {
		interfaceJNI.RenderResize(w, h);
	}
	
	public void onDrawFrame(GL10 gl) {
		interfaceJNI.RenderDraw();
	}
}
