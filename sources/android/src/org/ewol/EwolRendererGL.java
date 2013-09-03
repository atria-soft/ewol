/**
 * @author Edouard DUPIN, Kevin BILLONNEAU
 *
 * @copyright 2011, Edouard DUPIN, all right reserved
 *
 * @license BSD v3 (see license file)
 */

package org.ewol;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

// import the ewol package :
/* no need in same package... */
//import org.ewol.Ewol;

import org.ewol.Ewol;

/**
 * @brief Class : 
 *
 */
public class EwolRendererGL implements GLSurfaceView.Renderer
{
	private Ewol EWOL;
	
	public EwolRendererGL(Ewol ewolInstance)
	{
		EWOL = ewolInstance;
	}
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
