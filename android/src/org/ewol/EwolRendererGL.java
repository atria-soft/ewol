/**
 * @author Edouard DUPIN, Kevin BILLONNEAU
 *
 * @copyright 2011, Edouard DUPIN, all right reserved
 *
 * @license APACHE v2.0 (see license file)
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
	private Ewol m_ewolNative;
	
	public EwolRendererGL(Ewol _ewolInstance) {
		m_ewolNative = _ewolInstance;
	}
	public void onSurfaceCreated(GL10 _gl, EGLConfig _config) {
		m_ewolNative.renderInit();
	}
	
	public void onSurfaceChanged(GL10 _gl, int _w, int _h) {
		m_ewolNative.renderResize(_w, _h);
	}
	
	public void onDrawFrame(GL10 _gl) {
		m_ewolNative.renderDraw();
	}
}
